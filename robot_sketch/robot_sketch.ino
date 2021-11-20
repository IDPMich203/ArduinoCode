#include <arduino-timer.h>

#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Robot.h"
#include "Driver.h"
#include "DistanceSensor.h"
#include "Communications.h"

#define LEFT_MOTOR 1
#define RIGHT_MOTOR 2

#define echoPin 2 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 3 //attach pin D3 Arduino to pin Trig of HC-SR04
#define LEFT_SENSOR 4
#define RIGHT_SENSOR 5

Robot robot(LEFT_SENSOR, RIGHT_SENSOR);
Driver driver(LEFT_MOTOR, RIGHT_MOTOR);


void setup() {
  // Enable Bluetooth
  pinMode(NINA_RESETN, OUTPUT);         
  digitalWrite(NINA_RESETN, LOW);
  // ultrasound = DistanceSensor(echoPin, trigPin);
  Serial.begin(9600);
  SerialNina.begin(115200);
  // robot = Robot(LEFT_SENSOR, RIGHT_SENSOR);
  robot.setState(RobotState::LEAVING_TO_SEARCH_AREA);
}


void loop() {
  robot.timer.tick();
  // // put your main code here, to run repeatedly:
  // robot.line_following(driver);
  // driver.stop();
  //once man detected and within distance to pick up
//  robot.pick_up();
//  robot.id_dummy();
//  //take man to box position
//  //locate next man with camera
//  //go to man position
//  if(getDistance() <= 5){
//    driver.stop();
//    robot.pick_up();
//  }
//  robot.id_dummy();
  //take man to box position
}
