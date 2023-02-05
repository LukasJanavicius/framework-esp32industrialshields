#ifndef __EXPANDED_GPIO_H__
#define __EXPANDED_GPIO_H__

#include <Arduino.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void initExpandedGpio();

void analogWrite(uint32_t pin, uint16_t value);

#ifdef __cplusplus
}
#endif

#endif
