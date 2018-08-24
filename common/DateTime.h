
#ifndef DATETIME_H
#define DATETIME_H

#include "Timer.h"
#include <pebble.h>

void DateTime_construct(TextLayer* dateLayer, TextLayer* timeLayer);

void DateTime_destroy();

TimerWatcher DateTime_getDateWatcher();

TimerWatcher DateTime_getTimeWatcher(); // 1 minute refresh

TimerWatcher DateTime_getSecondsWatcher();


#endif