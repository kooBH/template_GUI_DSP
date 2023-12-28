#include "app.h"

app::app():
    widget_disp(640,256,512,400)
  {
  setStyleSheet("\
			QWidget{background:rgb(212, 251, 206);}\
      \
      ");

    /* top widget */{
    layout_top.addWidget(&btn_play);
    btn_play.setText("Play");

    // Param Edits
    /*
    https://doc.qt.io/qt-6/qdoublevalidator.html
    QDoubleValidator(double bottom, double top, int decimals, QObject *parent = nullptr)
    */
   
    layout_main.addLayout(&layout_top);
    layout_top.setAlignment(Qt::AlignLeft);

    QObject::connect(&btn_play, &QPushButton::pressed, this, &app::slot_btn_play);
    QObject::connect(&proc, &processor::signal_update, &widget_disp, &KPlotStreamer::slot_update);
    /* Processor */
    QObject::connect(&btn_play, &QPushButton::pressed, &proc, &processor::slot_toggle);
  }

  /* main widget */{
    widget_main.setStyleSheet("\
			QWidget{background:rgb(233, 250, 206);}\
      \
      ");

    widget_main.addTab(&widget_disp, "display");


    widget_main.addTab(&widget_config, "parameters");
    widget_config.setStyleSheet("\
			QWidget{background:rgb(255,255,255);}\
			QLineEdit{background:rgb(238, 238, 238);}\
			QComboBox{background:rgb(238, 238, 238);}\
      \
      ");
    

    widget_config.Add("Input/Output", "../config/io.json");
    layout_main.addWidget(&widget_main);
  }

  setMinimumSize(640,480);
  setLayout(&layout_main);
};

app::~app() {

};

void app::slot_btn_play() {
  // Stop
  if (is_playing) {
    btn_play.setText("Play");
  }
  // Start
  else {
    setProcParam();
    btn_play.setText("Stop");
  }
  is_playing = !is_playing;
  std::cout << is_playing << std::endl;
}


 double app::get(QString a, string b) {
  double val;
   json j = widget_config[a.toStdString()][b]["value"];
   switch (j.type()) {
    //case json::value_t::object :
      //break;
    case json::value_t::boolean :
      return static_cast<double>(j.get<bool>());
    case json::value_t::number_float :
      return j.get<double>();
    case json::value_t::number_unsigned :
      return static_cast<double>(j.get<int>());
    case json::value_t::number_integer :
      return static_cast<double>(j.get<int>());
    default:
      printf("ERROR::unsupported json type:%d\n",j.type());
    }
 }

 void app::setProcParam() {
   proc.device_in = static_cast<int>(get("Input/Output", "input_device"));

 }

