#include "handlers.h"

#include "servo.h"
#include "gatts_table.h"
#include "utils.h"
#include "esp_log.h"
#include "pins.h"

const char *TAG = "handlers";

static servo_handle_t servo;

void app_init()
{
    servo_config_t config = servo_default_config();
    config.pin = SERVO_PULSE_GPIO;
    servo = servo_init(&config);
}

void handle_rctrl_servo(gatts_write_evt_param_t value)
{
    int16_t angle = bytes_to_int16(value.value);
    ESP_LOGI(TAG, "Servo angle: %d", angle);
    if (angle > 90)
    {
        angle = 90;
    }
    else if (angle < -90)
    {
        angle = -90;
    }
    servo_set_angle(servo, angle);
}

// Motor A is the motor for wheels
void handle_rctrl_motor_a(gatts_write_evt_param_t value)
{
    int16_t speed = bytes_to_int16(value.value);
    ESP_LOGI(TAG, "Motor A speed: %d", speed);
}

// Motor B is the motor for transforming
void handle_rctrl_motor_b(gatts_write_evt_param_t value)
{
    int16_t speed = bytes_to_int16(value.value);
    ESP_LOGI(TAG, "Motor B speed: %d", speed);
}
