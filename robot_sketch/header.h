#ifndef HEADER_H // include guard
#define HEADER_H
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"
#include "Driver.h"
class Robot{
  public:
    void line_following(Driver driver);
    void pick_up();
    void id_dummy();
};
#endif
