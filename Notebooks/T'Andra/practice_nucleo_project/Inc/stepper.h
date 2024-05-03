#ifndef STEPPER_H
#define STEPPER_H

#include "stm32f4xx_hal.h"
#include "main.h"

void motorDelay (uint32_t delay);
void stepCV (int steps, float speed);
void stepCCV (int steps, float speed);
void motorOff ();
void motorOn ();

#endif /* STEPPER_H */
