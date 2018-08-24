
#ifndef TIMER_H
#define TIMER_H

#include <pebble.h>

typedef void (*TimerHandler)();

typedef struct TimerWatcher
{
  TimerHandler handler;
  uint32_t updateInterval;
  TimeUnits updateUnits;  
} TimerWatcher;

void Timer_construct(struct TimerWatcher* watchers, uint numWatchers);

void Timer_destroy();


//bool Timer_addWatcher(TimerWatcher handler, uint32_t updateInterval, TimeUnits updateUnits);

#endif