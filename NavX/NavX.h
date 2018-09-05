#include <Wire.h>
#include "Utilities.h"
#include "AHRSProtocol.h"

#define NAVX_ADDRESS 0x32

byte low_byte = 0;
byte high_byte = 0;

//Gives an array of bytes that the navX gives based on what information you ask it (navRegisters can be found in IMURegisters.h
signed int getNavXSensorReading(int navRegister) {
  Wire.beginTransmission(NAVX_ADDRESS);//Start a transmission tot he navx
  Wire.write(navRegister);//Tell the navX what data the user wants back (readingType)
  Wire.write(2);//Tell the navX how many bytes we want back
  Wire.endTransmission();
  //Get the navX's answer
  Wire.beginTransmission(NAVX_ADDRESS);
  Wire.requestFrom(NAVX_ADDRESS, 2);//Tell the navX we are requesting 2 bytes from it
  //Store the response in an array of bytes
  byte lower = Wire.read();
  byte higher = Wire.read();
  Wire.endTransmission();

  return twoBytesToSignedInt(higher, lower);//Combine the two bytes to a single number and give it to the caller
}

class NavX 
{ 
    signed int headingOrigin;//The angle that is considered '0' for the heading
    signed int rollOrigin;
    signed int pitchOrigin;

    
  
public:
      NavX() {
         headingOrigin = 0;
         rollOrigin = 0;
         pitchOrigin = 0;
      }

      //This function will keep the angle between -180 and +180 after applying the origin subtraction
      static signed int normalizeAngle(signed int angle) {
        if (angle < -18000) {
          angle += 36000;
        }else if (angle > 18000) {
          angle -= 36000;
        }
        return angle;
      }

      
      
      //Returns the current heading in hundredths of degrees(100=1 degree) angle is between -180 and +180 degrees
      signed int getHeading() {
        signed int angle = getNavXSensorReading(NAVX_REG_YAW_L);//Get the data we want from gyro
        
        //signed int angle = twoBytesToSignedInt(high_byte, low_byte);//convert the two bytes into a signed int
        angle -= headingOrigin;//Make the angle relative to the set origin
        angle = normalizeAngle(angle);//Make sure the angle doesnt go farther than +180 or -180

        return angle;
    }

    //Returns the current roll of the of NavX in hundredths of degrees(100=1 degree) angle is between -180 and +180 degrees
    signed int getRoll() {
      signed int angle = getNavXSensorReading(NAVX_REG_ROLL_L);//Get the roll from the navX

      
      angle -= rollOrigin;//Make the angle relative to the origin
      angle = normalizeAngle(angle);//Keep the angle between +180 and -180

      return angle;
    }

    //Gets the current roll of the NavX independant of the set origin point
    signed int getRollRaw() {
      return getNavXSensorReading(NAVX_REG_ROLL_L);
    }

    //Returns the current pitch of the NavX in hundredths of degrees (100 = 1 degree) angle is between -180 and +180
    signed int getPitch() {
      signed int angle = getNavXSensorReading(NAVX_REG_PITCH_L);//Get the pitch from the navX

      
      angle -= pitchOrigin;//Make the angle relative to the origin
      angle = normalizeAngle(angle);//Keep the angle between +180 and -180

      return angle;
    }

    //Returns the current pitch of the NavX independant of the set origin
    signed int getPitchRaw() {
      return getNavXSensorReading(NAVX_REG_PITCH_L);
    }

    //Returns the current heading in hundredths of degrees independant of the origin point
    signed int getHeadingRaw() {
      return getNavXSensorReading(NAVX_REG_YAW_L);
    }

    //Sets the current heading as the NavX's heading origin 
    void resetHeadingOrigin() {
      headingOrigin = getHeadingRaw();
    }

    //Sets the current roll as the NavX's roll origin
    void resetRollOrigin() {
      rollOrigin = getRollRaw();
    }

    //Sets the current pitch as the NavX's pitch origin
    void resetPitchOrigin() {
      pitchOrigin = getPitchRaw();
    }
};

