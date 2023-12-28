#include "processor.h"


processor::processor() {
  atomic_thread.store(false);
  buf_in = new short[in_channels * n_hop];
}

processor::~processor() {
  delete[] buf_in;
}

void processor::Process() {
  if (atomic_thread.load()) {
    printf("Still Processing!!\n");
    return;
  }

  atomic_thread.store(true);

  rt_input = new RtInput(device_in, in_channels, sr, n_hop, n_fft);
  printf("==OpenAudioStream==\ndevice : %d\n_channelsannels : %d\nsamplesr : %d\n", device_in, in_channels, sr);

  rt_input->Start();

  while (rt_input->IsRunning()) {
    if (rt_input->data.stock.load() > n_hop) {
      rt_input->GetBuffer(buf_in);

      // scaling
      for (int i = 0; i < n_hop * in_channels; i++) {
        buf_in[i] = buf_in[i] * 15;
      }
      

      emit(signal_update(buf_in));
    }
    else {
      SLEEP(10);
    }
  }
  atomic_thread.store(false);
}

void processor::slot_toggle() { 

  if (atomic_thread.load()) {
    printf("STOP\n");
    rt_input->Stop();
  }
  else {
    printf("START\n");
    Run();
  }
}

void processor::Run(){
  
  if (atomic_thread.load()) {
    printf("Warnning::Process thread is still running");
    return;
  }
  else {
    if (thread_process) {
      delete thread_process;
      thread_process = nullptr;
    }
  }

  thread_process = new std::thread([=] {this->Process(); });
  thread_process->detach();

}

