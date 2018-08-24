
#pragma once 

#include <pebble.h>

Window* Window_construct(WindowHandlers handlers);

void Window_setBGColour(Window* window, GColor colour);

void Window_destroy(Window* window);

//void Window_addTextBox(TextLayer* textLayer);
