#include "handlers.h"

#include "servo.h"
#include "motor.h"
#include "gatts_table.h"
#include "utils.h"
#include "esp_log.h"
#include "pins.h"
#include "led.h"

const char *TAG = "handlers";

static led_strip_handle_t led_strip;
static servo_handle_t servo;
static motor_handle_t motor_a, motor_b;
static motor_drv_stby_handle_t motor_drv_stby;

void app_init()
{
    led_strip = configure_led();

    servo_config_t config = servo_default_config();
    config.pin = SERVO_PULSE_GPIO;
    servo = servo_init(&config);

    motor_config_t motor_config = motor_default_config();
    motor_config.pin_a = MOTOR_A_PIN1;
    motor_config.pin_b = MOTOR_A_PIN2;
    motor_a = motor_init(&motor_config);

    motor_config.pwm_group_id = 1;
    motor_config.pin_a = MOTOR_B_PIN1;
    motor_config.pin_b = MOTOR_B_PIN2;
    motor_b = motor_init(&motor_config);

    motor_drv_stby = motor_drv_stby_init(MOTOR_DRV_STBY_GPIO);
    motor_drv_stby_add(motor_drv_stby, motor_a);
    motor_drv_stby_add(motor_drv_stby, motor_b);
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
    motor_set_speed(motor_a, speed);
}

// Motor B is the motor for transforming
void handle_rctrl_motor_b(gatts_write_evt_param_t value)
{
    int16_t speed = bytes_to_int16(value.value);
    ESP_LOGI(TAG, "Motor B speed: %d", speed);
    motor_set_speed(motor_b, speed);
}
