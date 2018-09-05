
#include <Wire.h>
#define BAUD_RATE 19200
#define CHAR_BUF 128

class OpenMVCam {
  
 public:

    OpenMVCam() {
      
    }
    
    int32_t temp = 0;
    char buff[CHAR_BUF] = {0};
    
    char* msg() {
      Wire.requestFrom(0x12, 2);
      if(Wire.available() == 2) { // got length?
    
        temp = Wire.read() | (Wire.read() << 8);
        delay(1); // Give some setup time...
    
        Wire.requestFrom(0x12, temp);
        if(Wire.available() == temp) { // got full message?
    
          temp = 0;
          while(Wire.available()) buff[temp++] = Wire.read();
    
        } else {
          while(Wire.available()) Wire.read(); // Toss garbage bytes.
        }
      } else {
        while(Wire.available()) Wire.read(); // Toss garbage bytes.
      }
      return buff;
	  delay(1);
    }
};