#include "servo.h"

#include "esp_log.h"

static const char *TAG = "servo";

#define SERVO_MIN_PULSEWIDTH_US 1000 // Minimum pulse width in microsecond
#define SERVO_MAX_PULSEWIDTH_US 2500 // Maximum pulse width in microsecond
#define SERVO_MIN_DEGREE -90         // Minimum angle
#define SERVO_MAX_DEGREE 90          // Maximum angle

#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000 // 1MHz, 1us per tick
#define SERVO_TIMEBASE_PERIOD 20000          // 20000 ticks, 20ms

servo_config_t servo_default_config(void)
{
    return (servo_config_t){
        .pwm_group_id = 0,
        .min_pulsewidth_us = SERVO_MIN_PULSEWIDTH_US,
        .max_pulsewidth_us = SERVO_MAX_PULSEWIDTH_US,
        .min_degree = SERVO_MIN_DEGREE,
        .max_degree = SERVO_MAX_DEGREE,
        .timebase_resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .timebase_period = SERVO_TIMEBASE_PERIOD,
    };
}

static inline uint32_t servo_angle_to_pulsewidth_us(int angle, servo_config_t *config)
{
    if (unlikely(angle < config->min_degree))
        angle = config->min_degree;
    if (unlikely(angle > config->max_degree))
        angle = config->max_degree;
    return (angle - config->min_degree) * (config->max_pulsewidth_us - config->min_pulsewidth_us) / (config->max_degree - config->min_degree) + config->min_pulsewidth_us;
}

servo_handle_t servo_init(servo_config_t *config)
{
    ESP_LOGI(TAG, "Initialize servo");
    servo_handle_t servo = (servo_handle_t)malloc(sizeof(servo_t));
    servo->config = *config;

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
    servo->timer = timer;

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = config->pwm_group_id, // operator must be in the same group to the timer
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));
    servo->operator= oper;

    ESP_LOGI(TAG, "Connect timer and operator");
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");
    mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));
    servo->comparator = comparator;

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = config->pin,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));
    servo->generator = generator;

    // set the initial compare value, so that the servo will spin to the center position
    servo_set_angle(servo, config->initial_angle);

    ESP_LOGI(TAG, "Set generator action on timer and compare event");
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));

    return servo;
}

void servo_set_angle(servo_handle_t servo, int angle)
{
    ESP_LOGI(TAG, "Angle of rotation: %d", angle);
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(
        servo->comparator,
        servo_angle_to_pulsewidth_us(angle, &servo->config)));
}

void servo_free(servo_handle_t servo)
{
    ESP_LOGI(TAG, "Free servo");
    mcpwm_del_timer(servo->timer);
    mcpwm_del_operator(servo->operator);
    mcpwm_del_comparator(servo->comparator);
    mcpwm_del_generator(servo->generator);
    free(servo);
}
