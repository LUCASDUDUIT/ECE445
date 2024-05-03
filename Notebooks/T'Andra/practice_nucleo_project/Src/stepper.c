#include "stm32f4xx_it.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include <stdio.h>
#include <string.h>

extern TIM_HandleTypeDef htim3;
extern int stepNumber;          // which step the motor is on

void motorDelay (uint32_t delay) //MICRO SECONDS "DELAY", THIS IS MULTITASKING 'CAUSE HAL_GetTick(); it's implicit.
{

	if (delay < 60000)
	    {
	        __HAL_TIM_SET_COUNTER(&htim3, 0);
	        while (__HAL_TIM_GET_COUNTER(&htim3) < delay);
	    }
	    else
	    {
	        HAL_Delay(delay/1000);
	    }
}

void motorOn() // HALF DRIVE SECUENCY
{
switch (stepNumber){
case 1:
	HAL_GPIO_WritePin(GPIOB, IN0_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, IN1_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, IN2_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, IN3_Pin, 1);
	HAL_Delay(5);
break;

case 2:

	HAL_GPIO_WritePin(GPIOB, IN0_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, IN1_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, IN2_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, IN3_Pin, 0);
	HAL_Delay(5);
break;

case 3:

	HAL_GPIO_WritePin(GPIOB, IN0_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, IN1_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, IN2_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, IN3_Pin, 0);
	HAL_Delay(5);

break;

case 4:

	HAL_GPIO_WritePin(GPIOB, IN0_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, IN1_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, IN2_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, IN3_Pin, 1);
	HAL_Delay(5);
break;
default:
	break;
}}



void motorOff()
{
    // Switch off the idle current to the motor
    // Otherwise L298N module will heat up
    HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_RESET); // IN1
    HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_RESET); // IN2
    HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_RESET); // IN3
    HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_RESET); // IN4
}

void stepCV (int steps, float speed) // CV - Clockwise
{
  uint32_t delay = 60*1000*1000/200/speed;
  for(int x=0; x<steps; x=x+1)
  {
    // Step to the next step
    switch (stepNumber)
    {
      case 1:
        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_SET);   // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_RESET); // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_SET);   // IN3
        HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_RESET); // IN4
        motorDelay(delay);
        stepNumber = 2;
        break;
      case 2:
        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_RESET); // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_SET);   // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_SET);   // IN3
        HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_RESET); // IN4
        motorDelay(delay);
        stepNumber = 3;
        break;
      case 3:

        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_RESET); // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_SET);   // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_RESET); // IN3
        motorDelay(delay);
        stepNumber = 4;
        break;
      case 4:
        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_SET);   // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_RESET); // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_RESET); // IN3
        HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_SET);   // IN4
        motorDelay(delay);
        stepNumber = 1;
        break;
      default:
        break;
    }
  }
}
void stepCCV (int steps, float speed) // CCV - Counter Clockwise
{
  uint32_t delay = 60*1000*1000/200/speed;
  for(int x=0; x<steps; x=x+1)
  {
    // Step to the previous step
    switch (stepNumber)
    {
      case 1:
        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_RESET); // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_SET);   // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_RESET); // IN3
        HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_SET);   // IN4
        motorDelay(delay);
        stepNumber = 4;
        break;
      case 2:
        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_SET);   // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_RESET); // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_RESET); // IN3
        HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_SET);   // IN4
        motorDelay(delay);
        stepNumber = 1;
        break;
      case 3:
        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_SET);   // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_RESET); // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_SET);   // IN3
        HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_RESET); // IN4
        motorDelay(delay);
        stepNumber = 2;
        break;
      case 4:
        HAL_GPIO_WritePin(GPIOB, IN0_Pin, GPIO_PIN_RESET); // IN1
        HAL_GPIO_WritePin(GPIOB, IN1_Pin, GPIO_PIN_SET);   // IN2
        HAL_GPIO_WritePin(GPIOB, IN2_Pin, GPIO_PIN_SET);   // IN3
        HAL_GPIO_WritePin(GPIOB, IN3_Pin, GPIO_PIN_RESET); // IN4
        motorDelay(delay);
        stepNumber = 3;
        break;
      default:
        break;
    }
  }}
