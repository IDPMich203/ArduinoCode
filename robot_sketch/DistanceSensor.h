#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H

uint8_t echoPin, trigPin;
void setupSensor(uint8_t eP, uint8_t tP)
{
  echoPin = eP;
  trigPin = tP;
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT);  // Sets the echoPin as an INPUT
}
float getDistance()
{
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH, 3000);
  if (duration == 0)
  {
    duration = 1000000;
  }
  // Calculating the distance
  float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //return distance;
  return distance;
}

#endif
