// Accelerometer
#pragma once
#include "utils.h"
#include <pebble.h>

// net scalar accel of p_acc vs g, in m/s/s
float Accelerometer_netScalar();

// net scalar horizonal accel of p_acc vs g, in m/s/s. Assumes -g in z direction
float Accelerometer_netScalarH();

void Accelerometer_construct(AccelSamplingRate rate, int samplesPerUpdate, Layer* updateFrame);

void Accelerometer_destroy();

const Vector3i* Accelerometer_accel(); // in milligs

float Accelerometer_deltaT();

float Accelerometer_mms2();