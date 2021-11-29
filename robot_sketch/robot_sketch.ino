
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h>

#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "header.h"
#include "Driver.h"
#include "DistanceSensor.h"
#include "Dummy.h"
#include "DelayAsync.h"
#include "Communications.h"


Robot robot;
#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D3 Arduino to pin Trig of HC-SR04

#define AMBER_PIN 11
#define RED_PIN 12
#define GREEN_PIN 13

#define BUTTON 7
//Timer<1> timer;



//Driver driver(1,2);
Driver driver;
void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(AMBER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(AMBER_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  //  driver = Driver(1,2);
  driver.begin(1, 2);
  //  driver.Init();
  setupSensor(echoPin, trigPin);
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

}

float x, y, z;
bool ran = false;
bool beforeramp = true;
bool running = false;

bool amberstate = true;
bool blink() {
  Serial.println("Blink");
  digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
  amberstate = !amberstate;
  return true;
}

void loop() {
  auto task = MainTimer.every(500, blink);
  while(1){
    MainTimer.tick();
    Serial.println(detectDummy());
  }
  //while(1){
  ////  driver.rearServo.write(ARM_DOWN);
  ////  delay(1000);
  ////  driver.rearServo.write(ARM_UP);
  ////  delay(1000);`
  //////  driver.gripperServo.write(GRIPPER_OPEN);
  ////  continue;
  //driver.raise_back();
  //driver.close_gripper();
  //
  //driver.lower_back();
  //driver.open_gripper();
  //delay(1000);
  //}
  //Loop has already run.
  if (ran) {
    return;
  }

  //while(true){
  //  Serial.println("Blink");
  //  digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
  //  digitalWrite(GREEN_PIN, amberstate ? HIGH : LOW);
  //  digitalWrite(RED_PIN, amberstate ? HIGH : LOW);
  //  amberstate = !amberstate;
  //  delay(500);
  //  return true;
  //}

  //Wait for button press
  while (!running) {
    running = digitalRead(BUTTON);
  }

  Serial.println("RUnning");
  //Set up blink task
  task = MainTimer.every(500, blink);

  //Follow line before ramp, keep an eye out for inclination change (does not work)
  while (beforeramp) {
    //Update blink timer.
    MainTimer.tick();
    delay(10);
    Serial.println("Line following");
    robot.line_following(driver);
    if (IMU.accelerationAvailable()) {
      IMU.readAcceleration(x, y, z);
    }

    if (z < 1.0) { //Check this.
      beforeramp = false;
    }
  }

  //After ramp, check if dummy is around.
  while (getDistance() > 5) {
    Serial.println("Moving");
    //Update blink timer
    MainTimer.tick();
    robot.line_following(driver);
    delay(10);
  }

  //No longer moving
  driver.stop();
  MainTimer.cancel(task);

  Serial.println("Stopped");



  float avg_x = 0;
  float avg_y = 0;
  float avg_z = 0;
  float x, y, z;

  bool calibrated = false;
  unsigned long prev = micros();
  float rot = 0;
  float target_rot = 180.0;
  for (int i = 0; i < 500; i++) {
    while (!IMU.gyroscopeAvailable());
    IMU.readGyroscope(x, y, z);
    avg_x += x;
    avg_y += y;
    avg_z += z;
  }
  avg_x /= 100.0;
  avg_y /= 100.0;
  avg_z /= 100.0;
  calibrated = true;
  prev = micros();

  driver.start_move(-1.0);
  delay(500);
  driver.stop();
  driver.start_turn(1.0);
  while (true) {
    MainTimer.tick();
    if (IMU.gyroscopeAvailable()) {
      IMU.readGyroscope(x, y, z);
      x -= avg_x;
      y -= avg_y;
      z -= avg_z;


      unsigned long dt = micros() - prev;
      prev = micros();
      rot += z * dt / 1000000.0;
      Serial.println(rot);
      MainTimer.tick();
      if (rot > 160 || rot < -160 ) {
        driver.stop();
        break;
      }
    }
  }


  driver.open_gripper(true);
  driver.start_move(-1.0);
  delay(1000);
  driver.stop();


  driver.raise_back();
  driver.close_gripper();
  driver.lower_back();



  //Detect dummy and turn on corresponding LED.
  int dummy_no = detectDummy();
  if (dummy_no == 0) {
    for (int i = 0; i < 10; i++) {
      Serial.println("Blink");
      digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
      digitalWrite(GREEN_PIN, amberstate ? LOW : HIGH);
      digitalWrite(RED_PIN, amberstate ? HIGH : LOW);
      amberstate = !amberstate;
      delay(500);
      //  if(digitalRead(BUTTON)) return;
    }

  } else if (dummy_no == 1) {
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
  } else if (dummy_no == 2) {
    digitalWrite(RED_PIN, HIGH);
  } else if (dummy_no == 3) {
    digitalWrite(GREEN_PIN, HIGH);
  }

  //After ramp, check if dummy is around.
  while (getDistance() > 5) {
    Serial.println("Moving");
    //Update blink timer
    MainTimer.tick();
    robot.line_following(driver);
    delay(10);
  }
  driver.stop();
  while (true) {
    Serial.println("Blink");
    digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
    digitalWrite(GREEN_PIN, amberstate ? HIGH : LOW);
    digitalWrite(RED_PIN, amberstate ? HIGH : LOW);
    amberstate = !amberstate;
    delay(500);
    if (digitalRead(BUTTON)) return;

  }


}
