#include "OpenMVCam.h"

OpenMVCam cam;
String data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Returns -1 if no coordinate can be given
  Coordinate blobCoordinate = cam.getColorBlobCoordinate();//Gets the coordinate of the object the camera has recognized
  Serial.print("Coordinate: ");
  Serial.print(blobCoordinate.X);
  Serial.print(", ");
  Serial.println(blobCoordinate.Y);
  
}
