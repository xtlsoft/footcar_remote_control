#pragma once

#include "driver/mcpwm_prelude.h"

struct motor_config
{
    int pwm_group_id;
    int timebase_resolution_hz;
    int timebase_period;
    int pin_a;
    int pin_b;
};
typedef struct motor_config motor_config_t;

struct motor
{
    motor_config_t config;
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t operator;
    mcpwm_cmpr_handle_t comparator_a;
    mcpwm_cmpr_handle_t comparator_b;
    mcpwm_gen_handle_t generator_a;
    mcpwm_gen_handle_t generator_b;
};
typedef struct motor motor_t;
typedef motor_t *motor_handle_t;

motor_config_t motor_default_config(void);

motor_handle_t motor_init(motor_config_t *config);
void motor_set_speed(motor_handle_t motor, int speed);
void motor_brake(motor_handle_t motor);
void motor_free(motor_handle_t motor);

void motor_set_percentage_cmpr_a(motor_handle_t motor, uint32_t percentage);
void motor_set_percentage_cmpr_b(motor_handle_t motor, uint32_t percentage);
