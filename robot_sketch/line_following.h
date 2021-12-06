#ifndef FOLLOW_LINE_H
#define FOLLOW_LINE_H
#include "Driver.h"

#define LEFT_SENSOR 4
#define RIGHT_SENSOR 5


//Setup pins
void BeginLineFollowing(){
  pinMode(LEFT_SENSOR, INPUT);
  pinMode(RIGHT_SENSOR, INPUT);
}

//Follow line, return integer based on how many lines are seen
int FollowLine(Driver driver){
    bool right = digitalRead(RIGHT_SENSOR);
    bool left  = digitalRead(LEFT_SENSOR);

    //Both white
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

    //Both black
    else{
     driver.start_move(1);
     return 3;
     }
}

#endif
