#include "NavX.h"//Include navx library
#include <Wire.h>//Include Wire (NavX library requires I2C to communicate with NavX)

NavX navX;//Init an object of the class

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();//Make sure to start the Wire bus so the NavX library can use it

  //Sets the current value of the Heading/Roll/Pitch as its Origin point for that axis (zero)
  navX.resetHeadingOrigin();
  navX.resetRollOrigin();
  navX.resetPitchOrigin();
}

void loop() {
  // put your main code here, to run repeatedly:
  signed int heading = navX.getHeading();//Gets the current heading in the range of -180 to +180 (Positive is clockwise)
  signed int roll = navX.getRoll();
  signed int pitch = navX.getPitch();

  Serial.print("Heading: ");
  Serial.print(heading);
  Serial.print(" Roll: ");
  Serial.print(roll);
  Serial.print(" Pitch: ");
  Serial.println(pitch);
}
