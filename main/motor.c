#include "motor.h"

#include "esp_log.h"

static const char *TAG = "motor";

motor_config_t motor_default_config(void)
{
    return (motor_config_t){
        .pwm_group_id = 0,
        .timebase_resolution_hz = 1000000,
        .timebase_period = 20000,
    };
}

motor_handle_t motor_init(motor_config_t *config)
{
    ESP_LOGI(TAG, "Initialize motor");
    motor_handle_t motor = (motor_handle_t)malloc(sizeof(motor_t));
    motor->config = *config;

    ESP_LOGI(TAG, "Create timer and operator");
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = config->pwm_group_id,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = config->timebase_resolution_hz,
        .period_ticks = config->timebase_period,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));
    motor->timer = timer;

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = config->pwm_group_id, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));
    motor->operator= oper;

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    mcpwm_cmpr_handle_t comparator_a = NULL, comparator_b = NULL;
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator_a));
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator_b));
    motor->comparator_a = comparator_a;
    motor->comparator_b = comparator_b;

    mcpwm_gen_handle_t generator_a = NULL, generator_b = NULL;
    mcpwm_generator_config_t generator_config_a = {
        .gen_gpio_num = config->pin_a,
    };
    mcpwm_generator_config_t generator_config_b = {
        .gen_gpio_num = config->pin_b,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config_a, &generator_a));
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config_b, &generator_b));
    motor->generator_a = generator_a;
    motor->generator_b = generator_b;

    // Set initial speeds
    motor_set_percentage_cmpr_a(motor, 0);
    motor_set_percentage_cmpr_b(motor, 0);

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator_a,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator_a,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator_a, MCPWM_GEN_ACTION_LOW)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator_b,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator_b,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator_b, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));

    return motor;
}

void motor_set_percentage_cmpr_a(motor_handle_t motor, uint32_t percentage)
{
    uint32_t value = motor->config.timebase_period * percentage / 100;
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(motor->comparator_a, value));
}

void motor_set_percentage_cmpr_b(motor_handle_t motor, uint32_t percentage)
{
    uint32_t value = motor->config.timebase_period * percentage / 100;
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(motor->comparator_b, value));
}

void motor_set_speed(motor_handle_t motor, int speed)
{
    if (unlikely(speed > 100))
        speed = 100;
    if (unlikely(speed < -100))
        speed = -100;
    if (speed > 0)
    {
        motor_set_percentage_cmpr_a(motor, speed);
        motor_set_percentage_cmpr_b(motor, 0);
    }
    else if (speed < 0)
    {
        motor_set_percentage_cmpr_a(motor, 0);
        motor_set_percentage_cmpr_b(motor, -speed);
    }
    else
    {
        motor_set_percentage_cmpr_a(motor, 0);
        motor_set_percentage_cmpr_b(motor, 0);
    }
}

void motor_brake(motor_handle_t motor)
{
    motor_set_percentage_cmpr_a(motor, 100);
    motor_set_percentage_cmpr_b(motor, 100);
}

void motor_free(motor_handle_t motor)
{
    ESP_LOGI(TAG, "Free motor");
    mcpwm_del_generator(motor->generator_a);
    mcpwm_del_generator(motor->generator_b);
    mcpwm_del_comparator(motor->comparator_a);
    mcpwm_del_comparator(motor->comparator_b);
    mcpwm_del_operator(motor->operator);
    mcpwm_del_timer(motor->timer);
    free(motor);
}
