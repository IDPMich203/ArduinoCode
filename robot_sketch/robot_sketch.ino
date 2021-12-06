#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
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


bool running = false;

Driver driver;
Communications communications;

bool amberstate = true;
void blinkAmber()
{
  digitalWrite(AMBER_PIN, amberstate ? HIGH : LOW);
  amberstate = !amberstate;
  return true;
}

void setup()
{

  //Setup LED and Button pins
  pinMode(BUTTON, INPUT);
  pinMode(AMBER_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(AMBER_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  Serial.begin(9600);

  //Initialise parts of the system
  driver.begin(1, 2);
  communications.begin("SSID", "password");
  setupSensor(echoPin, trigPin);
  BeginLineFollowing();
  InitIMU();


  //Asynchronous blinking for the movement indicator
  MainTimer.every(250, blinkAmber);


  //Display status lights to indicate ready to run
  digitalWrite(AMBER_PIN, HIGH);
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  delayAsync(1000);
  digitalWrite(AMBER_PIN, LOW);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
}

void loop()
{
  //Wait for start button to be pressed
  while (!running)
  {
    running = digitalRead(BUTTON);
  }


  //Setup and energise servos
  driver.start();


  communications.debug("Started program, following line");


  while (getDistance() > 5.0)
  {
    FollowLine(driver);
    //Yield control to allow program to do other things (blink)
    delayAsync(10);
  }


  communications.debug("Found dummy, preparing to pick up");

  //Required 5 second delay
  driver.stop();
  delayAsync(5000);

  //Turn 180 degrees so gripper faces the dummy
  communications.debug("Turning");
  delayAsync(200);
  turn_angle(driver, 180);
  driver.open_gripper(true);

  //Move up to the dummy (simply a move with delay with no feedback)
  communications.debug("Approaching dummy");
  driver.start_move(-1.0);
  delayAsync(1150);

  //Pick up
  driver.stop();
  driver.pick_up();


  communications.debug("Picked up dummy, identifying");

  //Detect dummy and turn on corresponding LED.
  int dummy_no = detectDummy();
  if (dummy_no == 0)
  {
    communications.debug("No dummy");
  }

  else if (dummy_no == 1)
  {
    communications.debug("Dummy 1");
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
  }

  else if (dummy_no == 2)
  {
    communications.debug("Dummy 2");
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
  }
  else if (dummy_no == 3)
  {
    communications.debug("Dummy 3");
    digitalWrite(GREEN_PIN, HIGH);
    digitalWrite(RED_PIN, LOW);
  }

  //Determine what to do with the dummy
  if (dummy_no == 1)
  {
    //Continue to white square
    communications.debug("Going to white square");
    turn_angle(driver, 180);
    while (true)
    {
      int i = FollowLine(driver);

      //If both sides detect white
      if (i == 0)
      {
        communications.debug("Detected lines");

        //Manouver to drop off position
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

        //Break from loop to run returning code
        break;
      }
      delayAsync(10);
    }
  }
  else if (dummy_no == 0)
  {
    //Return home

  }
  else
  {

    communications.debug("Going back home :)");
    while (true)
    {
      int i = FollowLine(driver);

      if (i == 0)
      {
        communications.debug("Detected lines");
        driver.stop();
        driver.start_move(1.0);
        delayAsync(1000);
        driver.stop();
        if (dummy_no == 2)
        {
          turn_angle(driver, 90);
        }
        else
        {
          turn_angle(driver, -90);
        }

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
    //End of program
    while (1);

  }

  //Return home from opposite side of arena
  //Check for white then black then white
  //Armed state becomes 1 when white is seen for the first time, then progresses to 2 when black is seen
  //When armed state is 2 the robot will stop in the box
  int armed_state = 0;
  while (true)
  {
    int i = FollowLine(driver);
    //We've seen white after seeing white then black
    if (armed_state >= 2 && i == 0)
    {
      driver.stop();
      driver.start_move(1.0);
      delayAsync(1800);
      driver.stop();
      //End of program
      while (1);
    }

    //Seen white for the first time
    else if (i == 0 && armed_state == 0)
    {
      armed_state++;
    }
    //Seen both black after seeing white
    else if (i == 3 && armed_state == 1)
    {
      armed_state++;
    }
    delayAsync(20);
  }


  running = false;
  return;

}
