#ifndef DRIVER_H
#define DRIVER_H
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>
#include "DelayAsync.h"
#define abs(x) ((x)>0?(x):-(x))

#define SERVO_MIN 25
#define SERVO_MAX 165

#define GRIPPER_CLOSED SERVO_MIN
#define GRIPPER_OPEN SERVO_MAX

#define ARM_UP SERVO_MIN
#define ARM_DOWN SERVO_MAX

class Driver{
  public:
  Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
  Adafruit_DCMotor leftMotor;
  Adafruit_DCMotor rightMotor;

  Servo gripperServo;
  Servo rearServo;

  int leftSpeed = 0;
  int rightSpeed = 0;
  void begin(int leftPin, int rightPin){
      AFMS.begin();
      leftMotor = *AFMS.getMotor(leftPin);
      rightMotor = *AFMS.getMotor(rightPin);
      leftMotor.setSpeed(leftSpeed);
      rightMotor.setSpeed(rightSpeed);

      gripperServo.attach(9);
      rearServo.attach(10);
      lower_back(true);
      open_gripper(true);
  }
  void raise_back(){
    for (int i=ARM_UP; i<ARM_DOWN; i++) {
    rearServo.write(i);
    delayAsync(30);
 }
  }
  void open_gripper(bool fast=false){
    if(fast){
      gripperServo.write(GRIPPER_OPEN);
      return;
    }
    for (int i=GRIPPER_CLOSED; i<GRIPPER_OPEN; i++) {
    gripperServo.write(i);
    delayAsync(10);
  }
  }
  void close_gripper(bool fast=false){
      if(fast){
      gripperServo.write(GRIPPER_OPEN);
      return;
    }
    for (int i=180; i>0; i--) {
    gripperServo.write(i);
    delayAsync(30);
  }
  }
  void lower_back(bool fast = false){
    if(fast){
      rearServo.write(ARM_UP);
      return;
    }
    for (int i=ARM_DOWN; i>ARM_UP; i--) {
    rearServo.write(i);
    delayAsync(30);
  }
  }
  void setSpeed(int speed){
    leftMotor.setSpeed(speed);
    leftSpeed = speed;
    rightMotor.setSpeed(speed);
    rightSpeed = speed;
  }

  void start_turn(float turnspeed){
    setSpeed((int) abs(turnspeed) * 255.0);
    if(turnspeed < 0){

      rightMotor.run(FORWARD);
      leftMotor.run(BACKWARD);
    }else{
      
      rightMotor.run(BACKWARD);
      leftMotor.run(FORWARD);
    }
  }
  void start_move(float movespeed){
    setSpeed((int) abs(movespeed) * 255.0);
    if(movespeed < 0){

      rightMotor.run(BACKWARD);
      leftMotor.run(BACKWARD);
    }else{
      
      rightMotor.run(FORWARD);
      leftMotor.run(FORWARD);
    }
  }


  void add_turn(float movespeed){
    leftSpeed += (int)(movespeed * 255.0);
    if(leftSpeed < 0){
      leftMotor.run(BACKWARD);
    }
    leftMotor.setSpeed(leftSpeed);
    rightSpeed -= (int)(movespeed * 255.0);
    if(rightSpeed < 0){
      rightMotor.run(BACKWARD);
    }
    rightMotor.setSpeed(rightSpeed);
  }
  void stop(){
    leftMotor.run(RELEASE);
    rightMotor.run(RELEASE);
  }

  void pick_up(){
    open_gripper(true);
    raise_back();
    close_gripper();
    lower_back();
  }
  void drop(){
    raise_back();
    open_gripper();
    lower_back();
  }

  void roll(){
    //I had to put this in
  }
};

#endif
