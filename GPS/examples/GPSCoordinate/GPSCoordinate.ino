#include "GPS.h"
GPS gps;//Create an object of the gps

//The GPS.h library uses the Serial Port (TX, RX) Pins to communicate with the GPS
//Pin Assignment
//GPS        Arduino
//TX -------> RX
//RX -------> TX
//VIN ------> 5v or 3.3v
//GND ------> Gnd

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);//Start serial on 115200 so we can read gps fast enough
  gps.agps.begin(9600);//Serial monitor is on 9600
 
  Serial.println("Started");
}

void loop() {
  gps.setOrigin();//Sets the current coordinate as the origin point(0, 0);
  
  GPSCoordinateInfo coordinate = gps.getCoordinate();//Gets the information about the current coordinate
  Serial.print("Latitude: ");
  Serial.print(coordinate.latitude, 6);//Latitude in Decimal Degrees
  Serial.print(" ");
  Serial.print(coordinate.direction_North_South);//North or South
  Serial.print(" Longitude: ");
  Serial.print(coordinate.longitude, 6);//Longitude in Decimal Degrees
  Serial.print(" ");
  Serial.println(coordinate.direction_East_West);//East or West
  Serial.print("Fix: ");
  Serial.println(gps.getFix());//If the GPS is getting a reading from the satelites (1 if there is communication)
  Serial.print("Distance from Origin Latitude: ");
  Serial.print(coordinate.distanceFromOriginFeetLatitude);//Distance from the Origin in Feet on the latitude axis
  Serial.println("ft");
  Serial.print("Distance from Origin Longitude: ");
  Serial.print(coordinate.distanceFromOriginFeetLongitude);//Distance from the Origin in Feet on the longitude axis
  Serial.println("ft");
}

