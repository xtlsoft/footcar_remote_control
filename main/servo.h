#pragma once

#include "driver/mcpwm_prelude.h"

struct servo_config
{
    int min_pulsewidth_us;
    int max_pulsewidth_us;
    int min_degree;
    int max_degree;
    int timebase_resolution_hz;
    int timebase_period;
    int initial_angle;
    int pin;
};
typedef struct servo_config servo_config_t;

struct servo
{
    servo_config_t config;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t operator;
};
typedef struct servo servo_t;
typedef servo_t *servo_handle_t;

servo_config_t servo_default_config(void);

servo_handle_t servo_init(servo_config_t *config);
void servo_set_angle(servo_handle_t servo, int angle);
void servo_free(servo_handle_t servo);
