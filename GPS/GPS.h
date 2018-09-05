#include <Adafruit_GPS.h>
#define KilometersToFeet 3280.4

//This class uses the Serial Port (TX, RX) Pins to communicate with the GPS
//Pin Assignment
//GPS        Arduino
//TX -------> RX
//RX -------> TX
//VIN ------> 5v
//GND ------> Gnd

//Converts decimal degrees to feet
double coordinateToFeet(double decimalDegrees) {
  //Convert to kilometers
  double km = decimalDegrees * (10000/90);

  //convert to feet and return
  return km * KilometersToFeet;
}

//A response that 'GetCoordinate' function returns
struct GPSCoordinateInfo {
  //Coordinates are in decimal degrees
  double latitude;//The latitude in Decimal Degrees
  double longitude;//The longitude in Decimal Degrees
  char direction_North_South;//North or South
  char direction_East_West;//East or West
  /////////////////////////////////////
  double distanceFromOriginFeetLatitude;//Feet from origin on latitude axis
  double distanceFromOriginFeetLongitude;//Feet from origin on longitude axis
  double latitudeFeet;//Latitude in feet
  double longitudeFeet;//Longitude in feet
};

class GPS {
  double originLatitudeFeet = 0.0;
  double originLongitudeFeet = 0.0;
 
public:
  Adafruit_GPS agps = Adafruit_GPS(&Serial);
  GPS() {
    agps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  }

  int getFix() {
    return agps.fix;
  }

  char getLastCharSent() {
    return agps.read();
  }

  //Sets the current coordinate at the origin point
  void setOrigin() {
    GPSCoordinateInfo coord = getCoordinate();
    originLatitudeFeet = coord.latitudeFeet;
    originLongitudeFeet = coord.longitudeFeet;
  }

  //Gets the origin feet on Latitude axis
  double getOriginLatitudeFeet() {
    return originLatitudeFeet;
  }

  //Gets the origin feet on Longitde axis
  double getOriginLongitudeFeet() {
    return originLongitudeFeet;
  }

  GPSCoordinateInfo getCoordinate() {
    //Get all the bytes that the GPS is sending until we get the whole message
    while (!agps.newNMEAreceived()) {
      agps.read();
    }
 
    //Parse the message and if it cannot be parsed then return null
    if (!agps.parse(agps.lastNMEA())) {
      return;
    }
    //Lat and long coordinates
    double latitude = agps.latitudeDegrees;
    double longitude = agps.longitudeDegrees;
    //Get the the coordinates in feet
    double latFeet = coordinateToFeet(latitude);
    double longFeet = coordinateToFeet(longitude);

    //Get the direction
    char directionNS = agps.lat;
    char directionEW = agps.lon;

    double distanceLatFeet = latFeet - originLatitudeFeet;
    double distanceLongFeet = longFeet - originLongitudeFeet;

    //Set up the response
    GPSCoordinateInfo response;
    response.latitude = latitude;
    response.longitude = longitude;
    response.direction_North_South = directionNS;
    response.direction_East_West = directionEW;
    response.distanceFromOriginFeetLatitude = distanceLatFeet;
    response.distanceFromOriginFeetLongitude = distanceLongFeet;
    response.latitudeFeet = latFeet;
    response.longitudeFeet = longFeet;
  
    return response;  
  }
};


