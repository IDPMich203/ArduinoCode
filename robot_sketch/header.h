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
    void box(Driver driver);
   
    //Returns 0 for no dummy, 1,2,3 order on moodle
    int detectDummy(){
      int qsdPin = A1;
      int qsdVal = 0;
      float qsdAvg = 0;
      long int itt = 0;
      int tsopPin = A0;
      int tsopVal = 0;
      float tsopAvg = 0;
      for(itt =0; itt < 10000; itt++){
      
        itt = itt + 1;
        qsdVal = analogRead(qsdPin);
        qsdAvg = qsdAvg + qsdVal;
        tsopVal = analogRead(tsopPin);
        tsopAvg = tsopAvg + tsopVal;
      }
    
      qsdAvg = qsdAvg/10000;
      tsopAvg = tsopAvg/10000;
      if(qsdVal>900){
        //No dummy
        return 0;
      }
      if(tsopAvg < 920){
        //38kHz only
        return 1;
      }            
      else if(tsopAvg > 1000){
        //600us only
        return 2;
      }
      else{
        //Half and half
        return 3;
      }
      
    }
    void pick_up(Driver driver){
      driver.raise();
      driver.close_gripper();
      driver.lower();
    }
    void drop(Driver driver){
      driver.raise();
      driver.open_gripper();
      driver.lower();
    }
    
};
#endif
