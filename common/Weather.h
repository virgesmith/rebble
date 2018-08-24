// Location.h

#ifndef WEATHER_H
#define WEATHER_H

#include "Timer.h"
#include <pebble.h>

enum Keys {
  KEY_CONNECTION_FAILURE,
  KEY_STATUS,
	KEY_TIME,
  KEY_TEMPERATURE,
  KEY_PRESSURE,
	KEY_HUMIDITY,
	KEY_LAN_STATUS
};

//void Weather_handler();

void Weather_construct(TextLayer* weatherLayer, TextLayer* statusLayer, BitmapLayer* connStatusLayer, int iconIds[2]);

void Weather_destroy();

TimerWatcher Weather_getWatcher();

#endif