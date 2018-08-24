
#ifndef TOKEN_H
#define TOKEN_H

#include <pebble.h>
#include "Timer.h"

void Token_construct(TextLayer* textLayer);

void Token_destroy();

TimerWatcher Token_getWatcher();

#endif