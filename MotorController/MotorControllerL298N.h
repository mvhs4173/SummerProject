//A class that can be used to communicate with the L298N motor controller
class MotorControllerL298N {
  int motorA, motorB;
  int in1, in2, in3, in4;
public:
    //Provide the pins the motor controller is wired to
    /* All these pins are required in order to control both motors correctly
     * motorAPWM: The PWM pin that the ENA pin on motor controller is wired to on arduino 
     * motorBPWM: The PWM pin that the ENB pin on motor controller is wired to on arduino
     * in1Pin: The digital I/O pin that the IN1 pin on motor controller is wired to on arduino
     * in2Pin: The digital I/O pin that the IN2 pin on motor controller is wired to on arduino
     * in3Pin: The digital I/O pin that the IN3 pin on motor controller is wired to on arduino
     * in4Pin: The digital I/O pin that the IN4 pin on motor controller is wired to on arduino
     */
    MotorControllerL298N(int motorAPWM, int motorBPWM, int in1Pin, int in2Pin, int in3Pin, int in4Pin) {
      motorA = motorAPWM;
      motorB = motorBPWM;
      in1 = in1Pin;
      in2 = in2Pin;
      in3 = in3Pin;
      in4 = in4Pin;
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      pinMode(in3, OUTPUT);
      pinMode(in4, OUTPUT);
      
    }

    //Sets the speed of motor A, accepts values between -255 and +255 where negative values are backward and positive is forward
    void setMotorASpeed(int motorSpeed) {
      //Set the direction the motor should go
      if (motorSpeed > 0) {
        digitalWrite(in1, LOW);
        digitalWrite(in2, HIGH);
      }else if (motorSpeed < 0) {
        digitalWrite(in1, HIGH);
        digitalWrite(in2, LOW);
      }
      //motorSpeed = abs(motorSpeed);//Make sure the motor speed positive because analogWrite only accepts values between 0 and +255
      //Set the voltage of the motor
	  motorSpeed = abs(motorSpeed);
      analogWrite(motorA, motorSpeed);
    }

    //Sets the speed of motor B, accepts values between -255 and +255 where negative values are backward and positive is forward
    void setMotorBSpeed(int motorSpeed) {
      //Set the direction the motor should go
      if (motorSpeed > 0) {
        digitalWrite(in3, LOW);
        digitalWrite(in4, HIGH);
      }else if (motorSpeed < 0) {
        digitalWrite(in3, HIGH);
        digitalWrite(in4, LOW);
      }
      motorSpeed = abs(motorSpeed);
      //Set the voltage of the motor
      analogWrite(motorB, motorSpeed);
    }
};
