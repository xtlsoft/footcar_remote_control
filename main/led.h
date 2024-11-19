#pragma once

#include "led_strip.h"

led_strip_handle_t configure_led(void);
void led_set_color(led_strip_handle_t led_strip, uint8_t r, uint8_t g, uint8_t b);
