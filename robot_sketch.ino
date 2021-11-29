#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "header.h"
#include "Driver.h"
#include "DistanceSensor.h"
#include "communications.h"
#include <math.h>
#include "IMU.h"

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
//motor 1 should be on the left, motor 2 on the right
Adafruit_DCMotor *motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *motor2 = AFMS.getMotor(2);
Robot robot;
#define echoPin 3 // attach pin D3 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D2 Arduino to pin Trig of HC-SR04
int dummies = 0;
bool started = false;
int value = 0;
Driver driver;
Communications communications;
void setup() {
  // put your setup code here, to run once:
  pinMode(NINA_RESETN, OUTPUT);         
  //digitalWrite(NINA_RESETN, LOW);
  pinMode(7, INPUT_PULLUP);
  driver = Driver();
  driver.Init();
  setupSensor(echoPin, trigPin);
  Serial.begin(9600);
  SerialNina.begin(115200);
  AFMS.begin();
  Init();
  
}

void loop() {
  while(started==false){
    value = digitalRead(7);
    if(value==LOW){
      started=true;
    }
  }
  
  // put your main code here, to run repeatedly:
  bool insearch = communications.IsInSearchArea();
  int iteration = 0;
  while(getDistance() > 5 || insearch==false){
    robot.line_following(driver);
    delay(10);
    if(getDistance()<=5 && iteration%5==0){
      bool insearch = communications.IsInSearchArea();
      iteration+=1;
    }
  }
  //once it's in the search area AND within 5cm of a dummy
  driver.stop();
  //once man detected and within distance to pick up
  //turn 180 degrees
  robot.pick_up(driver);
  int pattern = robot.detectDummy();
  SerialNina.write("robotx");
  String robotstringx = SerialNina.readString();
  int robotx = robotstringx.toInt();
  SerialNina.write("roboty");
  String robotstringy = SerialNina.readString();
  int roboty = robotstringy.toInt();
  int boxx;
  int boxy;
  if (pattern==1){
    SerialNina.write("boxwx");
    String boxstringx = SerialNina.readString();
    boxx = boxstringx.toInt();
    SerialNina.write("boxwy");
    String boxstringy = SerialNina.readString();
    boxy = boxstringy.toInt();
  }
  else if (pattern==2){
    SerialNina.write("boxrx");
    String boxstringx = SerialNina.readString();
    boxx = boxstringx.toInt();
    SerialNina.write("boxry");
    String boxstringy = SerialNina.readString();
    boxy = boxstringy.toInt();
  }
  else if (pattern==3){
    SerialNina.write("boxbx");
    String boxstringx = SerialNina.readString();
    boxx = boxstringx.toInt();
    SerialNina.write("boxby");
    String boxstringy = SerialNina.readString();
    boxy = boxstringy.toInt();
  }
  if (pattern==1){
    
    int x = boxx-robotx; 
    int y = boxy-roboty;
    float boxdirection = atan(y/x);
    SerialNina.write("robotr");
    String robotstringr = SerialNina.readString();
    float robotr = robotstringr.toFloat();
    if (x<0){
     float rotate = boxdirection+robotr-3*M_PI/4;
    }
  }
  //IF BOX IS WHITE
   //if x<0, angle to rotate = direction + robot rotation -3pi/4; if x>0, angle to rotate = direction + robot rotation + pi/4
// if x=0 and y> 0, angle to rotate = direction + 3pi/4; if x=0 and y<0, angle to rotate = direction -pi/4
//IF BOX IS RED OR BLUE: go back over the ramp, then do same as if white
//forwards, stop, get robot coords, chec if they are equal, keep going forwards if not, stop when they are equal
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
