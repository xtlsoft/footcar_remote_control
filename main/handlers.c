#include "handlers.h"

#include "servo.h"
#include "gatts_table.h"
#include "utils.h"
#include "esp_log.h"

const char *TAG = "handlers";

static mcpwm_cmpr_handle_t servo_comparator;

void app_init()
{
    servo_comparator = servo_init();
}

void handle_rctrl_servo(gatts_write_evt_param_t value)
{
    int16_t angle = bytes_to_int16(value.value);
    ESP_LOGI(TAG, "Servo angle: %d", angle);
    servo_set_angle(servo_comparator, angle);
}

void handle_rctrl_motor_a(gatts_write_evt_param_t value)
{
    int16_t speed = bytes_to_int16(value.value);
    ESP_LOGI(TAG, "Motor A speed: %d", speed);
}

void handle_rctrl_motor_b(gatts_write_evt_param_t value)
{
    int16_t speed = bytes_to_int16(value.value);
    ESP_LOGI(TAG, "Motor B speed: %d", speed);
}
