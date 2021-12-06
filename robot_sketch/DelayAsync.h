#ifndef DELAY_ASYNC_H
#define DELAY_ASYNC_H
#include <arduino-timer.h>


//Timer that allows us to schedule tasks psuedo-asynchronously (tasks run during tick() calls)
Timer<10> MainTimer;

void delayAsync(unsigned long duration)
{

   // Get snapshot of time
   unsigned long startTime = millis();

   //Tick timer while waiting
   while ((unsigned long)(millis() - startTime) <= duration)
   {
      MainTimer.tick();
   }
}

#endif
