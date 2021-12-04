#ifndef FOLLOW_LINE_H
#define FOLLOW_LINE_H
#include "Driver.h"

#define LEFT_SENSOR 4
#define RIGHT_SENSOR 5

void BeginLineFollowing(){
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
}
int FollowLine(Driver driver){
  //currently assumes 2 sensors, one left and one right
  //if both sensors detect white line--need to know what inputs the reflective object sensors are connected to and how to interface with them before I can actually write this conditional
    bool right = digitalRead(RIGHT_SENSOR);
    bool left  = digitalRead(LEFT_SENSOR);
//    Serial.print(left);
//    Serial.print(",");
//    Serial.println(right);
    if(left && right){
      driver.start_move(1.0);
      return 0;
    }
    else if(right){
      driver.leftMotor.setSpeed(255);
      driver.rightMotor.setSpeed(125);
      return 1;
      }
    else if(left){
      driver.leftMotor.setSpeed(125);
      driver.rightMotor.setSpeed(255);
      return 2;
      }
    else{
     driver.start_move(1);
     return 3;
     }
}

#endif
