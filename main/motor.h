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
    int speed;
    motor_drv_stby_handle_t stby;
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

struct motor_drv_stby
{
    int pin;
    int total_motors;
    motor_t *motors[2];
};
typedef struct motor_drv_stby motor_drv_stby_t;
typedef motor_drv_stby_t *motor_drv_stby_handle_t;

enum motor_drv_stby_status
{
    // Turn off the motor driver (low power mode)
    MOTOR_DRV_STBY_STATUS_OFF = 0,
    // Turn on the motor driver (normal operation)
    MOTOR_DRV_STBY_STATUS_ON = 1,
};
typedef enum motor_drv_stby_status motor_drv_stby_status_t;

motor_drv_stby_handle_t motor_drv_stby_init(int pin);
void motor_drv_stby_add(motor_drv_stby_handle_t stby, motor_handle_t motor);
void motor_drv_stby_set(motor_drv_stby_handle_t stby, motor_drv_stby_status_t status);
void motor_drv_stby_free(motor_drv_stby_handle_t stby);

void motor_drv_stby_notify(motor_drv_stby_handle_t stby);
