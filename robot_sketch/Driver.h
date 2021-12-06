#ifndef DRIVER_H
#define DRIVER_H
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "DelayAsync.h"

//Floating point abs() as arduino doesn't support abs for floats
#define abs(x) ((x) > 0 ? (x) : -(x))

#define SERVO_MIN 25
#define SERVO_MAX 165

#define GRIPPER_CLOSED SERVO_MIN
#define GRIPPER_OPEN SERVO_MAX

#define ARM_UP SERVO_MIN + 10
#define ARM_DOWN SERVO_MAX

class Driver
{
  public:
    Adafruit_MotorShield AFMS = Adafruit_MotorShield();
    Adafruit_DCMotor leftMotor;
    Adafruit_DCMotor rightMotor;
    Servo gripperServo;
    Servo rearServo;
    int leftSpeed = 0;
    int rightSpeed = 0;

    //Setup motors
    void begin(int leftPin, int rightPin)
    {
      AFMS.begin();
      leftMotor = *AFMS.getMotor(leftPin);
      rightMotor = *AFMS.getMotor(rightPin);
      leftMotor.setSpeed(leftSpeed);
      rightMotor.setSpeed(rightSpeed);
    }


    //Setup and energise servos
    void start()
    {
      gripperServo.attach(9);

      //Rear servo is unstable so don't energise this until we have to
      //      rearServo.attach(10);

      //Ensure arm is up and gripper is open (can be fast)
      lower_back(true);
      open_gripper(true);
    }


    void raise_back()
    {
      rearServo.attach(10);

      //Move servo to the arm up position
      for (int i = ARM_UP; i < ARM_DOWN; i++)
      {
        rearServo.write(i);
        delayAsync(10);
      }
    }


    void open_gripper(bool fast = false)
    {


      //Move gripper servo to the open position
      if (fast)
      {
        gripperServo.write(GRIPPER_OPEN);
        return;
      }


      for (int i = GRIPPER_CLOSED; i < GRIPPER_OPEN; i++)
      {
        gripperServo.write(i);
        delayAsync(10);
      }
    }


    void close_gripper(bool fast = false)
    {


      //Move gripper servo to the closed position
      if (fast)
      {
        gripperServo.write(GRIPPER_CLOSED);
        return;
      }
      for (int i = GRIPPER_OPEN; i > GRIPPER_OPEN; i--)
      {
        gripperServo.write(i);
        delayAsync(30);
      }
    }


    void lower_back(bool fast = false)
    {

      //Attach rear servo
      rearServo.attach(10);

      //Move servo to arm up position and then write 0 to its pin to de-energise it
      if (fast)
      {
        rearServo.write(ARM_UP);
        rearServo.detach();
        analogWrite(10, 0);
        return;
      }
      for (int i = ARM_DOWN; i > ARM_UP; i--)
      {
        rearServo.write(i);
        delayAsync(30);
      }
      rearServo.detach();
      analogWrite(10, 0);
    }

    //Set speed for both motors
    void setSpeed(int speed)
    {
      leftMotor.setSpeed(speed);
      leftSpeed = speed;
      rightMotor.setSpeed(speed);
      rightSpeed = speed;
    }


    //Start turning with a speed between -1.0 and 1.0
    void start_turn(float turnspeed)
    {
      setSpeed((int)abs(turnspeed) * 255.0);
      if (turnspeed < 0)
      {

        rightMotor.run(FORWARD);
        leftMotor.run(BACKWARD);
      }
      else
      {

        rightMotor.run(BACKWARD);
        leftMotor.run(FORWARD);
      }
    }

    //Start moving with speed between -1.0 and 1.0
    void start_move(float movespeed)
    {
      setSpeed((int)abs(movespeed) * 255.0);
      if (movespeed < 0)
      {

        rightMotor.run(BACKWARD);
        leftMotor.run(BACKWARD);
      }
      else
      {

        rightMotor.run(FORWARD);
        leftMotor.run(FORWARD);
      }
    }


    //Add an incremental turning component to the motion (not used)
    void add_turn(float movespeed)
    {
      leftSpeed += (int)(movespeed * 255.0);
      if (leftSpeed < 0)
      {
        leftMotor.run(BACKWARD);
      }
      leftMotor.setSpeed(leftSpeed);
      rightSpeed -= (int)(movespeed * 255.0);
      if (rightSpeed < 0)
      {
        rightMotor.run(BACKWARD);
      }
      rightMotor.setSpeed(rightSpeed);
    }

    //Stop all motors
    void stop()
    {
      leftMotor.run(RELEASE);
      rightMotor.run(RELEASE);
    }

    //Routine to pick up an object
    void pick_up()
    {
      open_gripper(true);
      raise_back();
      close_gripper();
      lower_back();
    }

    //Routine to put down an object
    void drop()
    {
      raise_back();
      open_gripper();
      lower_back();
    }


    //Function that only exists to allow the robot to stop, drop and roll
    void roll()
    {
      //I had to put this in
    }
};

#endif
