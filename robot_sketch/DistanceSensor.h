#ifndef DISTANCE_SENSOR_H
#define DISTANCE_SENSOR_H


class DistanceSensor{
  public:
      uint8_t echoPin, trigPin;
      DistanceSensor(uint8_t echoPin, uint8_t trigPin) {
          echoPin = echoPin;
          trigPin = trigPin;
          pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
          pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
      }
      long getDistance() {
        // Clears the trigPin condition
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);

        // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        // Reads the echoPin, returns the sound wave travel time in microseconds
        long duration = pulseIn(echoPin, HIGH);

        if(duration == 0){
          duration = 1000000;
        }
        // Calculating the distance
        float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
        // Displays the distance on the Serial Monitor
      return distance;
    }

};
#endif
