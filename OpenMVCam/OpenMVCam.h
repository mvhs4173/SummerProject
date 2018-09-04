#include <Wire.h>
#define BAUD_RATE 19200
#define CHAR_BUF 128
#define CAMERA_ADDR 0x12

enum Color {RED, BLUE};

struct Coordinate {
       int X;
       int Y;
};

class OpenMVCam {
 
 boolean sendData(char *data) {
    Wire.beginTransmission(CAMERA_ADDR);
    Wire.write(data);
    Wire.endTransmission();
    return true;
 }

 
 boolean getPayload(int expectedPayloadSizeBytes, char *payload) {
    //Now receive the payload
    Wire.requestFrom(CAMERA_ADDR, expectedPayloadSizeBytes);
    //Put the payload into the payload buffer
    for (int index = 0; index < expectedPayloadSizeBytes; index++) {
      payload[index] = Wire.read();
    }
    return true;
 }

 Coordinate coord;
 
 public:
    
    OpenMVCam() {
      Wire.begin();
    }

    //If the camera is alive it will send 'ok'
    boolean isAlive() {
        int expectedPayloadSize = 2;//How large we expect the response to be in bytes
        sendData("_ok");//Tell the camera we want to know if everything is ok
        char payload[expectedPayloadSize];
        getPayload(expectedPayloadSize, payload);

        if (strncmp("ok", payload, 2) == 0) {
          return true;
        }
      return false;
    }

    //Gets the coordinate of the blob the camera is currently recognizing
    //Returns -1 if coordinate cannot be given
    Coordinate getColorBlobCoordinate() {
      int expectedPayloadSize = 6;//We are expecting three bytes back
      char coordChar[3] = "000";
      char payload[expectedPayloadSize] = "000000";
      sendData("cbc");//Send the command to get the coordinate of the color blob
      getPayload(expectedPayloadSize, payload);

      coordChar[0] = payload[0];
      coordChar[1] = payload[1];
      coordChar[2] = payload[2];

      int xCoord = atoi(coordChar);

      coordChar[0] = payload[3];
      coordChar[1] = payload[4];
      coordChar[2] = payload[5];

      int yCoord = atoi(coordChar);

      Coordinate coord;
      coord.X = xCoord;
      coord.Y = yCoord;

      return coord;
    }


    //Tells the camera to recognize a certain color
    //Returns true if the command was run successfully
    boolean recognizeColor(Color color) {
      int expectedPayloadSize = 3;
      if (color == RED) {
        sendData("rrb");//Tell the camera to recognize red blobs
      }else if (color == BLUE) {
        sendData("rbb");//Tell the camera to recognize the blue blobs
      }

      char payload[expectedPayloadSize];
      //Check if the camera has recieved the command and it has done the command
      getPayload(expectedPayloadSize, payload);

      if (strncmp("set", payload, expectedPayloadSize) == 0) {
        return true;
      }else {
        return false;
      }
    }

    //Tell the camera to look for april tags
    boolean recognizeAprilTags() {
      int expectedPayloadSize = 5;
      sendData("apr");//Tell the camera to recognize april tags
      char payload[expectedPayloadSize];

      getPayload(expectedPayloadSize, payload);

      //Check if the camera got our command successfully
      if (strncmp("april", payload, expectedPayloadSize) == 0) {
        return true;
      }else {
        return false;
      }
    }

    //Tells the camera to give the id the current April tag that it sees
    //If it doesnt not see any april tag the function will return -1
    int getAprilTagId() {
      int expectedPayloadSize = 3;
      sendData("cat");//Tell the camera to give the id of the Current April Tag

      char payload[expectedPayloadSize];
      getPayload(expectedPayloadSize, payload);

      int tagId = atoi(payload);//Convert the string to an integer

      return tagId;
    }

    //Get the X coordinate of the current april tag
    //If there is no april tag then it returns -1
    Coordinate getAprilTagCoordinate() {
      char coordChar[3] = "000";      
      
      int expectedPayloadSize = 6;
      sendData("apc");//Get the coordinate of the april tag
      char payload[expectedPayloadSize] = "000000";
      getPayload(expectedPayloadSize, payload);
      
      coordChar[0] = payload[0];
      coordChar[1] = payload[1];
      coordChar[2] = payload[2];
      int xCoord = atoi(coordChar);
      
      coordChar[0] = payload[3];
      coordChar[1] = payload[4];
      coordChar[2] = payload[5];
      
      int yCoord = atoi(coordChar);
      
      coord.X = xCoord;
      coord.Y = yCoord;
      return coord;
    }
};
