// Accelerometer functionality

#include "Accelerometer.h"
#include "TextBox.h"
#include <pebble.h>

// accelerations are in "milli-gs" i.e. 1000 = 9.80665m/s/s (range is -4000 to 4000)
static Vector3i p_a;

static Layer* p_frame = 0;

static float p_deltaT; // the unscaled dt between samples

// convert to mm/s/s
float Accelerometer_mms2(int milligs)
{
  return (float)milligs * 9.80655f; // mm/s/s
}

void Accelerometer_handler(AccelData *data, uint32_t nSamples)
{
  p_a.x = p_a.y = p_a.z = 0;
  for (uint32_t i = 0; i < nSamples; ++i)
  {
    p_a.x += data[i].x;
    p_a.y -= data[i].y;
    p_a.z += data[i].z;   
  }
  p_a.x /= (int)nSamples;
  p_a.y /= (int)nSamples;
  p_a.z /= (int)nSamples;
  
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "p_a.x=%d", p_a.x);
//  APP_LOG(APP_LOG_LEVEL_DEBUG, "p_a.y=%d", p_a.y);
  // tell OS to call draw ball callback
  if (p_frame)
		layer_mark_dirty(p_frame);
}

void Accelerometer_construct(AccelSamplingRate rate, int samplesPerUpdate, Layer* updateFrame)
{
	p_frame = updateFrame;

  AccelData data;
  int result = accel_service_peek(&data);
  if (result == -1)
  {
    APP_LOG(APP_LOG_LEVEL_INFO, "Accelerometer is not running.");
  }
  else if (result == -2)
  {
    APP_LOG(APP_LOG_LEVEL_WARNING, "Accelerometer is already subscribed to.");
  }
  
  // get the initial data - single sample
  Accelerometer_handler(&data, 1);

  /*int*/ accel_service_set_sampling_rate(rate/*AccelSamplingRate default is ACCEL_SAMPLING_25HZ*/);		

  // then set the callback
  accel_data_service_subscribe(samplesPerUpdate, Accelerometer_handler);

  // TODO dt seems slightly too short - not realtime, need to add cpu overhead?
  switch(rate)
  {
  case ACCEL_SAMPLING_10HZ:
    p_deltaT = 0.1 * samplesPerUpdate;
    break;
  case ACCEL_SAMPLING_50HZ:
    p_deltaT = 0.02 * samplesPerUpdate;
    break;
  case ACCEL_SAMPLING_100HZ:
    p_deltaT = 0.01 * samplesPerUpdate;
    break;
  case ACCEL_SAMPLING_25HZ:
  default:
    p_deltaT = 0.04 * samplesPerUpdate;
  }
}

void Accelerometer_destroy()
{
  accel_data_service_unsubscribe();
}

const Vector3i* Accelerometer_accel()
{
  return &p_a;
}

// scalar acceleration net of g, in mm/s/s
float Accelerometer_netScalar()
{
  return Accelerometer_mms2(abs(utils_isqrt(p_a.x * p_a.x + p_a.y * p_a.y + p_a.z * p_a.z) - 1000));
}

// scalar horizonal acceleration in mm/s/s assumes -g is on z axis
float Accelerometer_netScalarH()
{
  // TODO log this to check for bad values
  //APP_LOG(APP_LOG_LEVEL_INFO, "%d %d %d", p_a.x, p_a.y, a);
  return Accelerometer_mms2(utils_isqrt(p_a.x * p_a.x + p_a.y * p_a.y));
}

float Accelerometer_deltaT()
{
  return p_deltaT;
}
