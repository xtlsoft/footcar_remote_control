/*
 * SPDX-FileCopyrightText: 2021 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Attributes State Machine */
enum
{
    IDX_SVC_RCTRL,
    IDX_CHAR_RCTRL_SERVO,
    IDX_CHAR_VAL_RCTRL_SERVO,

    IDX_CHAR_RCTRL_MOTOR_A,
    IDX_CHAR_VAL_RCTRL_MOTOR_A,

    IDX_CHAR_RCTRL_MOTOR_B,
    IDX_CHAR_VAL_RCTRL_MOTOR_B,

    RCTRL_IDX_NB,
};
