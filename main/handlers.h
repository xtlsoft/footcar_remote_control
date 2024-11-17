#pragma once

#include "esp_gatts_api.h"

void app_init();

typedef struct gatts_write_evt_param gatts_write_evt_param_t;

void handle_rctrl_servo(gatts_write_evt_param_t value);
void handle_rctrl_motor_a(gatts_write_evt_param_t value);
void handle_rctrl_motor_b(gatts_write_evt_param_t value);
