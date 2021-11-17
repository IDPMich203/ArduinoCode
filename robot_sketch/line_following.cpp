#include "header.h"
#include "Driver.h"
#define LEFT_SENSOR 4
#define RIGHT_SENSOR 5
void Robot::line_following(Driver driver){
  //currently assumes 2 sensors, one left and one right
  //if both sensors detect white line--need to know what inputs the reflective object sensors are connected to and how to interface with them before I can actually write this conditional
  while(true){
    bool right = digitalRead(RIGHT_SENSOR);
    bool left  = digitalRead(LEFT_SENSOR);
    if(left && right){
      driver.start_move(0.2);
    }
    else if(right){
      driver.add_turn(0.2);
      }
    else if(left){
      driver.add_turn(-0.2);
      }
    else{
     driver.start_move(1);
     }
     delay(100);
  }
  
  
  
}
