
#include "Timer.h"

static TimerWatcher* s_watchers;
static uint s_numWatchers;
static TimeUnits s_timeUnits;

void Timer_handler(struct tm* tick_time, TimeUnits unitsChanged) 
{
  for (uint i = 0; i < s_numWatchers; ++i)
  {
    if (s_watchers[i].updateUnits & unitsChanged)
    {
      int* p = 0;
      switch(s_watchers[i].updateUnits)
      {
        case SECOND_UNIT:
          p = &tick_time->tm_sec;
          break;
        case MINUTE_UNIT:
          p = &tick_time->tm_min;
          break;
        case HOUR_UNIT:
          p = &tick_time->tm_hour;
          break;
        case DAY_UNIT:
          p = &tick_time->tm_yday; // day of year also mday/wday
          break;
        case MONTH_UNIT:
          p = &tick_time->tm_mon;
          break;
        default: // by default update infrequently
        case YEAR_UNIT:
          p = &tick_time->tm_year;
          break;
      }
      if (*p % s_watchers[i].updateInterval == 0) 
      {
        //APP_LOG(APP_LOG_LEVEL_INFO, "Invoking Timer watcher %u at event %d", i, unitsChanged); 
        s_watchers[i].handler();    
      }
    }
  }
}

void Timer_construct(struct TimerWatcher* watchers, uint numWatchers)
{  
//  APP_LOG(APP_LOG_LEVEL_INFO, "Timer_construct()");

  s_watchers = watchers; 
  s_numWatchers = numWatchers;
  for (uint i = 0; i < numWatchers; ++i)
  {
  //  APP_LOG(APP_LOG_LEVEL_INFO, "Added Timer watcher %d: %p %d", i, watchers[i].handler, watchers[i].updateUnits);
    s_timeUnits = s_timeUnits | watchers[i].updateUnits;
  }
  //APP_LOG(APP_LOG_LEVEL_INFO, "registering with timer service: %d", s_timeUnits);
  
  // Register with TickTimerService
  tick_timer_service_subscribe(s_timeUnits, Timer_handler);
}

void Timer_destroy()
{
//  APP_LOG(APP_LOG_LEVEL_INFO, "Timer_destroy()");
  tick_timer_service_unsubscribe();
}

TimeUnits getTimeUnits()
{
  return s_timeUnits;
}

