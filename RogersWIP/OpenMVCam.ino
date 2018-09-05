#include <OpenMVCam.h>

OpenMVCam cam;
String data;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(BAUD_RATE);
  Wire.begin();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(cam.msg());
  if(cam.msg() == "left"){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);
  } else if (cam.msg() == "forward") {
    digitalWrite(LED_BUILTIN, LOW);
  }
}
