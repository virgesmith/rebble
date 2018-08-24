// Location.h

#ifndef LOCATION_H
#define LOCATION_H

#include "Timer.h"
#include <pebble.h>

enum Keys {
  KEY_LOCATION,
  KEY_TEMPERATURE,
  KEY_CONDITIONS,
};

//void Location_handler();

void Location_construct(TextLayer* s_locLayer, TextLayer* s_weatherLayer);

void Location_destroy();

TimerWatcher Location_getWatcher();

#endif