
#pragma once

#include <pebble.h>

TextLayer* TextBox_construct(Window* window, GRect coords, GColor bgColour, GColor fgColour, GFont font, 
      GTextAlignment align);

void TextBox_destroy(TextLayer* textLayer);

void TextBox_write(TextLayer* textLayer, const char* txt);

void TextBox_setFGColour(TextLayer* textLayer, GColor colour);

void TextBox_setBGColour(TextLayer* textLayer, GColor colour);