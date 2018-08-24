
#include "TextBox.h"

TextLayer* TextBox_construct(Window* window, GRect coords, GColor bgColour, GColor fgColour, GFont font, GTextAlignment align)
{
  TextLayer* textLayer = text_layer_create(coords);
  text_layer_set_background_color(textLayer, bgColour);
  text_layer_set_font(textLayer, font /*fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD)*/);
  text_layer_set_text_color(textLayer, fgColour);
  text_layer_set_text_alignment(textLayer, align);
  if (window)
  {
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(textLayer));
  }
  //APP_LOG(APP_LOG_LEVEL_INFO, "Text added to window: %p", (void*)window);

  return textLayer;
}


void TextBox_destroy(TextLayer* textLayer)
{
  text_layer_destroy(textLayer);
}

void TextBox_write(TextLayer* layer, const char* txt)
{
  text_layer_set_text(layer, txt);
}

void TextBox_setFGColour(TextLayer* textLayer, GColor colour)
{
  text_layer_set_text_color(textLayer, colour); 
}

void TextBox_setBGColour(TextLayer* textLayer, GColor colour)
{
  text_layer_set_background_color(textLayer, colour); 
}
