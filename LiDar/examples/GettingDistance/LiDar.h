#include <SoftwareSerial.h>
#include <TFMini.h>

class LiDar {
  
  public:
    TFMini sensor;
    LiDar() {
      
    }

    boolean start(SoftwareSerial serial) {
      serial.begin(TFMINI_BAUDRATE);
      return true;
    }

    uint16_t getDistanceCm() {
      return sensor.getDistance();
    }
};

