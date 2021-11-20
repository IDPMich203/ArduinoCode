#include "Robot.h"

//Redefine here because C++ is a pain
DistanceSensor Robot::ultrasound(2,3);
Communications Robot::comms;
Driver Robot::driver(1,2);
Timer<10> Robot::timer;

Robot::Robot(uint8_t leftSensor, uint8_t rightSensor){
  leftSensor = leftSensor;
  rightSensor = rightSensor;
  ultrasound = DistanceSensor(2,3);
  driver = Driver(1,2);
  isinArea = true;

}

void Robot::line_following(Driver driver){
    bool right = digitalRead(leftSensor);
    bool left  = digitalRead(rightSensor);
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
}

void Robot::pick_up(){
  driver.raise();
  driver.close_gripper();
  driver.lower();
}

void Robot::drop(){
  driver.raise();
  driver.open_gripper();
  driver.lower();
}

void Robot::setState(RobotState newstate){
  if(state == newstate){
    return;
  }
  state = newstate;
  switch(newstate){
    case RobotState::LEAVING_TO_SEARCH_AREA:
        timer.every(100, FollowLine);
  }

}


bool Robot::checkInArea(bool &is){
  is = comms.IsInSearchArea();
  return false; //Do not repeat
}

bool Robot::FollowLine(){
  if(state == RobotState::LEAVING_TO_SEARCH_AREA){
    if (ultrasound.getDistance() < 5.0 && isinArea){
      driver.stop();
      //Check if in search area and store that
      checkInArea(isinArea);
      if(isinArea){
        setState(RobotState::FINDING_DUMMY);
        return false;
      }else{
        //resume
        driver.setSpeed(driver.leftSpeed);
      }
    }
    return true; //Repeat
  }
  return false; //Cancel task
}