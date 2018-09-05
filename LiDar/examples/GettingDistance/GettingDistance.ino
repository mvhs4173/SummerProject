#include <SoftwareSerial.h>
#include "LiDar.h"

SoftwareSerial sensorSerial(10, 11);//Start a serial port for sensor (RX, TX);
LiDar lidar;

void setup() {
  //Init serial ports for lidar
  Serial.begin(115200);//Serial port for serial monitor
  sensorSerial.begin(115200);//Serial port for communication with sensor
  lidar.sensor.begin(&sensorSerial);//Give serial port to sensor
}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t cm = lidar.getDistanceCm();//Get the distance in centimeters
  Serial.print("Centimeters: ");
  Serial.println(cm);
}
