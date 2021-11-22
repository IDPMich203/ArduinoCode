#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "header.h"
#include "Driver.h"
#include "DistanceSensor.h"
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//motor 1 should be on the left, motor 2 on the right
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Robot robot;
#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D3 Arduino to pin Trig of HC-SR04

Driver driver;
void setup() {
  // put your setup code here, to run once:
  pinMode(NINA_RESETN, OUTPUT);         
  digitalWrite(NINA_RESETN, LOW);
  driver = Driver();
  driver.Init();
  setupSensor(echoPin, trigPin);
  Serial.begin(9600);
  SerialNina.begin(115200);
  AFMS.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
   while(getDistance() > 5){
     robot.line_following(driver);
     delay(10);
   }

//  Serial.println(getDistance());
  driver.stop();
  //once man detected and within distance to pick up
//  robot.pick_up();
//  robot.id_dummy();
//  //take man to box position: driver.stop(); Serial.write("robot coordinates"); locate robot; Serial.write("box [boxname] coordinates"); locate box; get path from robot to box; move along that path
//  for (int i=0; i<2; i++){
//  //locate next man with camera: driver.stop(); Serial.write("robot coordinates"); Serial.write("dummy coordinates"); locate robot; locate man; get path from robot to man; move along that path
//  if(getDistance() <= 5){
//    driver.stop();
//    robot.pick_up();
//  }
//  robot.id_dummy();
  //take man to box position:  driver.stop(); locate robot; locate box; get path from robot to box
//}

}
