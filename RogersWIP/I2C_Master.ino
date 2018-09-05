#include <MotorControllerL298N.h>

#include <Wire.h>
#define BAUD_RATE 19200
#define CHAR_BUF 128

#define stricmp(a, b) strcasecmp(a, b)

//Pins that the motor controller are plugged into
#define MOTOR_A_PIN 5//MUST be a PWM pin
#define MOTOR_B_PIN 6//MUST be a PWM pin
#define IN1 3
#define IN2 4
#define IN3 7
#define IN4 8

MotorControllerL298N motorController = MotorControllerL298N(MOTOR_A_PIN, MOTOR_B_PIN, IN1, IN2, IN3, IN4);//Give the class the pins that the motor controller is plugged into

void setup() {
  Serial.begin(BAUD_RATE);
  Wire.begin();
  delay(1000); // Give the OpenMV Cam time to bootup.
}

void loop() {
  int32_t temp = 0;
  char buff[CHAR_BUF] = {0};

  Wire.requestFrom(0x12, 2);
  if(Wire.available() == 2) { // got length?

    temp = Wire.read() | (Wire.read() << 8);
    delay(1); // Give some setup time...

    Wire.requestFrom(0x12, temp);
    if(Wire.available() == temp) { // got full message?

      temp = 0;
      while(Wire.available()) buff[temp++] = Wire.read();
    }
  }
  Serial.print(buff);
  Serial.println(stricmp("stop", buff));
  if (stricmp("forward", buff) == -10){
    motorController.setMotorASpeed(155);
    motorController.setMotorBSpeed(155);
  } else if (stricmp("left", buff) == -10) {
    motorController.setMotorASpeed(-80);
    motorController.setMotorBSpeed(80);
  } else if (stricmp("stop", buff) == -10){
    motorController.setMotorASpeed(0);
    motorController.setMotorBSpeed(0);    
  } else if (stricmp("right", buff) == -10) {
    motorController.setMotorASpeed(80);
    motorController.setMotorBSpeed(-80);
  }
  delay(100); // Don't loop to quickly.
}

