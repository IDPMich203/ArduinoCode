#ifndef ROBOT_H
#define ROBOT_H
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Driver.h"
class Robot{
  public:
    Driver driver;
    void begin();
    void line_following(Driver driver);
    void pick_up();
    void drop();
    void stop();
    
};
#endif
