#include <QObject>

#include "STFT.h"
#include "RtInput.h"

#include <vector>
#include <atomic>
#include <thread>

#include <time.h>

using std::vector;

class processor : public QObject {
  Q_OBJECT
private:
  bool isRunning = false;

  /* base modules */
  RtInput* rt_input = nullptr;

  short* buf_in = nullptr;

  std::thread* thread_process = nullptr;
  std::atomic<bool> atomic_thread; // is thread running ?

  void Process();

public:
  int device_in;

  processor();
  ~processor();

  /* param */

  int in_channels = 2;
  int sr = 16000;
  int n_fft = 2048;
  int n_hop = 512;

  int cnt = 0;
  double scale_factor = 10; //UMA-8

  void Run();

  void slot_toggle();

signals:
  void signal_update(short*);
  void signal_process_done(const char*);


};
