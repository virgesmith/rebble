// Compass.h
#ifndef COMPASS_H
#define COMPASS_H

#include <pebble.h>

int Compass_heading();

CompassStatus Compass_status();

const char* Compass_direction();

void Compass_construct(int angularSensDegrees);

void Compass_destroy();

void Compass_update();

#endif