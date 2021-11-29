#ifndef DRIVER_H
#define DRIVER_H
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

#define abs(x) ((x)>0?(x):-(x))
#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

class Driver{
  public:
  Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
  Adafruit_DCMotor leftMotor;
  Adafruit_DCMotor rightMotor;
  Servo gripperServo;
  Servo rearServo;
  int leftSpeed = 0;
  int rightSpeed = 0;
  void Init(){
      AFMS.begin();
      leftMotor = *AFMS.getMotor(LEFT_MOTOR);
      rightMotor = *AFMS.getMotor(RIGHT_MOTOR);
      leftMotor.setSpeed(255);
      rightMotor.setSpeed(255);
  }
  void turn(float angle){
    
  }
  void fwd(float distance){
    
  }
  void setSpeed(int speed){
    leftMotor.setSpeed(speed);
    leftSpeed = speed;
    rightMotor.setSpeed(speed);
    rightSpeed = speed;
  }
  void start_turn(float turnspeed){
    setSpeed((int) (abs(turnspeed) * 255.0));
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
  void raise(){
    for (int i=0; i<180; i++) {
      rearServo.write(i);
      delay(30);
    }
  }
  void open_gripper(){
    for (int i=0; i<180; i++) {
      gripperServo.write(i);
      delay(30);
    }
  }
  void close_gripper(){
    for (int i=180; i>0; i--) {
      gripperServo.write(i);
      delay(30);
    }
  }
  void lower(){
    for (int i=180; i>0; i--) {
      rearServo.write(i);
      delay(30);
    }
  }
};

#endif