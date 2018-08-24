

#ifndef BATTERY_H
#define BATTERY_H

#include <pebble.h>

void Battery_construct(BitmapLayer* display, int iconIds[4]);

void Battery_destroy();

#endif