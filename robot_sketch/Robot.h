#include <arduino-timer.h>

#ifndef ROBOT_H // include guard
#define ROBOT_H
#include <arduino-timer.h>
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Driver.h"
#include "DistanceSensor.h"
#include "Communications.h"

enum RobotState{
  IDLE,
  LEAVING_TO_SEARCH_AREA,
  FINDING_DUMMY,
  APPROACHING_DUMMY,
  GRABBING_DUMMY,
  RETURNING_TO_START
};

class Robot{
  public:
    Robot(uint8_t leftSensor, uint8_t rightSensor);
    inline static bool isinArea; //Assume in area until proven otherwise
    inline static RobotState state;

    inline static int leftSensor, rightSensor;
    static Driver driver;
    static Timer<10> timer;
    //Need to abstract pin numbers
    static DistanceSensor ultrasound;
    static Communications comms;


    static void line_following(Driver driver);
    static bool FollowLine();
    static bool checkInArea(bool &is);
    static void pick_up();
    static void id_dummy();
    static void drop();
    static void setState(RobotState state);
    
};
#endif
