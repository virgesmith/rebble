// Location.h

#pragma once 

#include "Timer.h"
#include <pebble.h>

enum Keys {
  KEY_NAMES,
  KEY_IPS,
  KEY_OSVERS,
  KEY_UPTIMES,
  KEY_LOADS,
  KEY_MEMS,
  KEY_DISKS,
  KEY_TEMPS,
	KEY_SIZE	
};

#define VALUE_SIZE 32

typedef struct {
 char data[KEY_SIZE][VALUE_SIZE];
} DeviceInfo;


//void Weather_handler();
void Monitor_construct(DeviceInfo* devices, int nDevices);

void Monitor_destroy();


