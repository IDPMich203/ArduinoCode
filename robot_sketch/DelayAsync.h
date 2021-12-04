#ifndef DELAY_ASYNC_H
#define DELAY_ASYNC_H
#include <arduino-timer.h>

Timer<10> MainTimer;

void delayAsync(unsigned long duration)
{

   // Get snapshot of time
   unsigned long startTime = millis();

   while ((unsigned long)(millis() - startTime) <= duration)
   {
      MainTimer.tick();
   }
}

#endif
