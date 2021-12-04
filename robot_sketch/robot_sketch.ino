#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Robot.h"
#include "Driver.h"
#include "DistanceSensor.h"
#include "Communications.h"
#include "line_following.h"
#include "Dummy.h"
#include <math.h>
#include "IMU.h"

#define deg(x) (x * 180.0 / 3.14159)

#define echoPin 3 // attach pin D3 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D2 Arduino to pin Trig of HC-SR04
#define BUTTON 7


#define AMBER_PIN 12
#define RED_PIN 11
#define GREEN_PIN 13


int dummies = 0;
bool running = false;
int value = 0;
Driver driver;
Communications communications;

bool amberstate = true;
void blinkAmber() {
//  Serial.println("Blink");
  digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
  amberstate = !amberstate;
  return true;
}

void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(AMBER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(AMBER_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  Serial.begin(9600);
  Serial.println("Hello");
  driver.begin(1, 2);
//  communications.begin("Walker", "bruhbruhbruh");

  setupSensor(echoPin, trigPin);


//  driver.open_gripper(true);

  BeginLineFollowing();
  InitIMU();
  //Wait for any transients to settle?
  delayAsync(500);
  MainTimer.every(500, blinkAmber);
//  calibrateIMU();
  digitalWrite(AMBER_PIN, HIGH);
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  delayAsync(1000);
  digitalWrite(AMBER_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  
}

void loop() {
//  turn_angle(driver,90);
/*
 * // Gripper testing
driver.start();
  while(1){
    delayAsync(1000);
    driver.pick_up();
    delayAsync(1000);
    driver.drop();
  }
  */
  /*
  //Line follower testing 
   while(1){
//    Serial.println(FollowLine(driver));
    FollowLine(driver);
    delayAsync(10);
   }
   */
  /*
  //Distance sensor testing
  while(1){
    Serial.println(getDistance());
  }
  */
  //Wifi testing stuff
  /*char boxes[3] = {'b', 'r', 'w'};
  int x, y;
  for (int i = 0; i < 3; i++) {
    int j = i % 3;
    Serial.print("Box: ");
    Serial.println(j);
    Serial.print(communications.getBoxCoords(boxes[j], x, y));
    Serial.print(",");
    Serial.print(x);
    Serial.print(",");
    Serial.println(y);
    delayAsync(1000);
  }
  x = 0; y = 0;
  Serial.print("Robot: ");
  Serial.print(communications.getRobotCoords(x, y));
  Serial.print(",");
  Serial.print(x);
  Serial.print(",");
  Serial.println(y);
  delayAsync(1000);
  float r;
  Serial.print("Rot: ");
  Serial.print(communications.getRobotRotation(r));
  Serial.print(",");
  Serial.println(r);
  delayAsync(1000);
  return;
  while (1);

  */
  while (!running) {
    running = digitalRead(BUTTON);
  }

  driver.start();

  communications.debug("Started program, following line");
  driver.start_move(1.0);
  while (getDistance() > 5.0) {
    FollowLine(driver);
    
    delayAsync(10);
  }
//  driver.stop();
  
  communications.debug("Found dummy, preparing to pick up");
  //Pick up
  driver.stop();
  delayAsync(5000);

/*
  communications.debug("Reversing");
  driver.start_move(-1.0);
  delayAsync(500);
  driver.stop();
*/
  communications.debug("Turning");
  delayAsync(200);
  turn_angle(driver, 180);
  driver.open_gripper(true);

  communications.debug("Approaching dummy");
  driver.start_move(-1.0);
  delayAsync(1150);
  driver.stop();
  driver.pick_up();

  communications.debug("Picked up dummy, identifying");
  //Detect dummy and turn on corresponding LED.
  int dummy_no = detectDummy();
  if (dummy_no == 0) {
    communications.debug("No dummy");
    /*
    for (int i = 0; i < 10; i++) {

      Serial.println("Blink");
      digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
      digitalWrite(GREEN_PIN, amberstate ? LOW : HIGH);
      digitalWrite(RED_PIN, amberstate ? HIGH : LOW);
      amberstate = !amberstate;
      delayAsync(500);
      //  if(digitalRead(BUTTON)) return;
    }
    */

  } else if (dummy_no == 1) {
    communications.debug("Dummy 1");
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
  } else if (dummy_no == 2) {
    communications.debug("Dummy 2");
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
  } else if (dummy_no == 3) {
    communications.debug("Dummy 3");
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
  }

  /*
  float last_distance = getDistance();
  while(1){
    turn_angle(20);
    
    if(last_distance - getDistance() > 7.0){
       break;
    }
  }
  */
  if (dummy_no == 1) {
    //COntinue to white square
    communications.debug("Going to white square");
    turn_angle(driver, 180);
    while (true) {
      int i = FollowLine(driver);
      
      if (i == 0) {
        communications.debug("Detected lines");
        driver.stop();
        turn_angle(driver, 180);
        driver.start_move(-1.0);
        delayAsync(1000);
        driver.stop();
        driver.drop();
        driver.roll();
        //Make sure gripper is open
        driver.open_gripper(true);
        driver.start_move(1.0);
        delayAsync(1000);
        driver.stop();
        //Check for white then black then white
        int armed_state = 0;
        while (true) {
          int i = FollowLine(driver);
          if(armed_state >= 2 && i == 0){
            driver.stop();
            driver.start_move(1.0);
            delayAsync(1800);
            driver.stop();
            driver.start_turn(1.0);
            while(1);
          }else if(i == 0){
            armed_state ++;
            
          }else if(i == 3 && armed_state == 1){
            armed_state++;
          }
          delayAsync(20);
        }
      }
      delayAsync(10);
    }
  } else if (dummy_no == 0) {
    //Oh god what do we do
  } else {

    communications.debug("Going back home :)");
    while (true) {
      int i = FollowLine(driver);

      if (i == 0) {
        communications.debug("Detected lines");
        driver.stop();
        driver.start_move(1.0);
        delayAsync(1000);
        driver.stop();
        if (dummy_no == 2) {
//          turn_angle(driver, 180 + 45);
            turn_angle(driver, 90);
        } else {
//          turn_angle(driver, 180 - 45);
          turn_angle(driver, -90);
        }
//        driver.start_move(-1.0);
//        delayAsync(1650);
        driver.stop();
        driver.drop();
        driver.roll();
        driver.start_move(1.0);
        delayAsync(1650);
        driver.stop();
        break;
      }
      delayAsync(10);
    }
  }

  running = false;
  return;

/*
  bool insearch;
  while (!communications.IsInSearchArea(insearch));
  int iteration = 0;

  while (getDistance() > 5 || !insearch) {
    FollowLine(driver);
    delay(10);
    if (getDistance() <= 5 && iteration % 5 == 0) {

      while (!communications.IsInSearchArea(insearch));
      iteration += 1;
    }
  }

  //once it's in the search area AND within 5cm of a dummy
  driver.stop();
  //once man detected and within distance to pick up
  //turn 180 degrees
  driver.pick_up();
  int pattern = detectDummy();
  */
  /*
  int robotx, roboty;
  while (!communications.getRobotCoords(robotx, roboty));
  int dummyx;
  int dummyy;
  while (!communications.getDummyCoords(dummyx, dummyy));


  int x = dummyx - robotx;
  int y = dummyy - roboty;
  float dummydirection = atan(y / x);
  float robotr;
  while (!communications.getRobotRotation(robotr));
  dummydirection -= 3.0 * M_PI / 4.0;

  turn_angle(deg(dummydirection));
  
  //White box
  if (pattern == 1) {
    while (!communications.getBoxCoords('w', boxx, boxy));
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
  }
  //Red box
  else if (pattern == 2) {
    while (!communications.getBoxCoords('r', boxx, boxy));
    digitalWrite(RED_PIN, HIGH);
  }
  //Blue box
  else if (pattern == 3) {
    while (!communications.getBoxCoords('b', boxx, boxy));
    digitalWrite(GREEN_PIN, HIGH);
  }
  if (pattern == 1) {

    int x = boxx - robotx;
    int y = boxy - roboty;
    float boxdirection = atan(y / x);
    float robotr;
    while (!communications.getRobotRotation(robotr));

    if (x < 0) {
      float rotate = boxdirection + robotr - 3 * M_PI / 4;
      rotate = deg(rotate);
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
  */

}
