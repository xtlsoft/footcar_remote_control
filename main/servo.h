#pragma once

#include "driver/mcpwm_prelude.h"

mcpwm_cmpr_handle_t servo_init(void);
void servo_set_angle(mcpwm_cmpr_handle_t comparator, int angle);
