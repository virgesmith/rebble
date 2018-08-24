
#ifndef CONNECTION_H
#define CONNECTION_H

#include "Timer.h"
#include <pebble.h>

void Connection_construct(BitmapLayer* layer, int iconIds[2]);

void Connection_destroy();

//void Connection_handler()

TimerWatcher Connection_getWatcher();

#endif

