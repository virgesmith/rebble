
#pragma once

// Index.h

//#ifndef WEATHER_H
//#define WEATHER_H

#include "Timer.h"
#include <pebble.h>

enum Keys {
  KEY_CONNECTION_STATUS,
  KEY_CCY,
  KEY_PRICE,
};

//void Weather_handler();

void Index_construct(TextLayer* indexLayer, TextLayer* indexLayer2, Layer* graphLayer);

void Index_destroy();

TimerWatcher Index_getWatcher();
