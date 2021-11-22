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
    Serial.print(",");
    Serial.println(right);
    if(left && right){
      driver.start_move(0.5);
    }
    else if(right){
      // driver.add_turn(0.2);
      driver.leftMotor.setSpeed(255);
      driver.rightMotor.setSpeed(125
      );
      }
    else if(left){
      // driver.add_turn(-0.2);
      driver.leftMotor.setSpeed(125);
      driver.rightMotor.setSpeed(255);
      }
    else{
     driver.start_move(1);
     }
  
  
  
}

// void Robot::shit_line_following(Driver driver){
//   bool right = digitalRead(RIGHT_SENSOR);
//   bool left  = digitalRead(LEFT_SENSOR);
//   Serial.print(left);
//   Serial.print(",");
//   Serial.println(right);
//   if(left && right){
//   motor1->setSpeed(0);
//   motor2->setSpeed(0);
//   }
//   else if (right){
//       //else if right sensor detects white line
//   motor1->setSpeed(150);
//   motor2->setSpeed(0);
//   motor1->run(FORWARD);
//   }

//   else if(left){
//   motor1->setSpeed(0);
//   motor2->setSpeed(150);
//   motor2->run(FORWARD);
//   }
//   else{
//   motor1->setSpeed(150);
//   motor2->setSpeed(150);
//   motor1->run(FORWARD);
//   motor2->run(FORWARD);
//   }

// }
