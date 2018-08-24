
#ifndef CLICK_H
#define CLICK_H

#include <pebble.h>

typedef void (*ClickHandler)(ClickRecognizerRef recognizer, void *context);

typedef struct ClickHandlers
{
  ClickHandler up;
  ClickHandler down;
  ClickHandler select;
	ClickHandler back;
  ClickHandler selectLong;
  ClickHandler selectMulti;
  ClickHandler selectRelease;
} ClickHandlers;

void Click_construct(Window* window, ClickHandlers handlers);

void Click_destroy();

#endif