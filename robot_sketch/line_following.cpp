#include "header.h"
#include "Driver.h"
#define LEFT_SENSOR 4
#define RIGHT_SENSOR 5
void Robot::line_following(Driver driver){
  //currently assumes 2 sensors, one left and one right
  //if both sensors detect white line--need to know what inputs the reflective object sensors are connected to and how to interface with them before I can actually write this conditional
    bool right = digitalRead(RIGHT_SENSOR);
    bool left  = digitalRead(LEFT_SENSOR);
    Serial.print(left);
    Serial.println(right);
    if(left && right){
      driver.start_move(0.5);
    }
    else if(right){
      driver.leftMotor.setSpeed(255);
      driver.rightMotor.setSpeed(125);
      }
    else if(left){
       driver.leftMotor.setSpeed(125);
      driver.rightMotor.setSpeed(255);;
      }
    else{
     driver.start_move(1);
     }
     
  
  
  
}

//same as line-following but instead of keeping going and crossing a white horizontal line, it stops when it gets to it, to put the robot down
void Robot::box(Driver driver){
  bool right = digitalRead(RIGHT_SENSOR);
    bool left  = digitalRead(LEFT_SENSOR);
    Serial.print(left);
    Serial.println(right);
    if(left && right){
      driver.stop();
    }
    else if(right){
      driver.leftMotor.setSpeed(255);
      driver.rightMotor.setSpeed(125);
      }
    else if(left){
       driver.leftMotor.setSpeed(125);
      driver.rightMotor.setSpeed(255);;
      }
    else{
     driver.start_move(1);
     }
}
