#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include <Arduino_LSM6DS3.h>
#include <arduino-timer.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "header.h"
#include "Driver.h"
#include "DistanceSensor.h"
#include "Dummy.h"

Robot robot;
#define echoPin 3 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 2 //attach pin D3 Arduino to pin Trig of HC-SR04

#define AMBER_PIN A2
#define RED_PIN A1
#define GREEN_PIN A0

#define BUTTON 7
Timer<1> timer;

Driver driver;
void setup() {
  pinMode(BUTTON, INPUT);
  pinMode(AMBER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(AMBER_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  
  driver = Driver();
  driver.Init();
  setupSensor(echoPin, trigPin);
  Serial.begin(9600);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

}

float x,y,z;
bool ran = false;
bool beforeramp = true;
bool running = false;

bool amberstate = true;
bool blink(){
  digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
  amberstate = !amberstate;
  return true;
}

void loop() {

//Loop has already run.
if(ran){
  return;
}

//Wait for button press
while(!running){
  running = digitalRead(BUTTON);
}

//Set up blink task
auto task = timer.every(500, blink);

//Follow line before ramp, keep an eye out for inclination change (does not work)
while(beforeramp){
  //Update blink timer.
  timer.tick();
  delay(10);
  robot.line_following(driver);
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
  }
  
  if(z < 1.0){ //Check this
    beforeramp = false;
  }
}

//After ramp, check if dummy is around.
while(getDistance() > 5){
     //Update blink timer
     timer.tick();
     robot.line_following(driver);
     delay(10);
}

//No longer moving
  driver.stop();
  timer.cancel(task);


  //Detect dummy and turn on corresponding LED.
  int dummy_no = detectDummy();
  if(dummy_no == 0){
    Serial.println(":(");
  }else if(dummy_no == 1){
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
  }else if(dummy_no == 2){
    digitalWrite(RED_PIN, HIGH);
  }else if(dummy_no == 3){
    digitalWrite(GREEN_PIN, HIGH);
  }

  //Ensure we do not run again.
  ran = true;

}
