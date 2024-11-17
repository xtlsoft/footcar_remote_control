#pragma once

#include <stdint.h>

inline uint16_t bytes_to_uint16(void *ptr)
{
    return ((uint8_t *)ptr)[0] << 8 | ((uint8_t *)ptr)[1];
}

inline int16_t bytes_to_int16(void *ptr)
{
    int8_t low = ((int8_t *)ptr)[0], high = ((int8_t *)ptr)[1];
    return (high << 8) | low;
}
