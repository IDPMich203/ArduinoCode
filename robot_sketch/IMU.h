#ifndef IDP_IMU_H
#define IDP_IMU_H
#include <Arduino_LSM6DS3.h>
#include "Driver.h"
#include "DelayAsync.h"
void Init() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}
float avg_x = 0;
float avg_y = 0;
float avg_z = 0;
float x, y, z;
void calibrate(int num=500) {
  for (int i = 0; i < num; i++) {
    while (!IMU.gyroscopeAvailable()) yield();
    IMU.readGyroscope(x, y, z);
    avg_x += x;
    avg_y += y;
    avg_z += z;
  }
  avg_x /= 100.0;
  avg_y /= 100.0;
  avg_z /= 100.0;
}
void turn_angle(Driver driver, float angle, unsigned long timeout = 10000){
float rot = 0;
if(angle < 0){
  driver.start_turn(-1.0);
}else if(angle > 0){
  driver.start_turn(1.0);
  
}else{
  return;
}
unsigned long prev = micros();
unsigned long start_time = micros();
while (true) {
  MainTimer.tick();
  
  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
    x -= avg_x;
    y -= avg_y;
    z -= avg_z;
    //Account for error
    z = abs(z) * 1.125;
    unsigned long dt = micros() - prev;
    if(micros() - start_time >= timeout * 1000){
      return;
    }
    prev = micros();
    rot += z * dt / 1000000.0;
    if (rot > abs(angle)) {
      driver.stop();
      return;
    }
  }
}
}
#endif
