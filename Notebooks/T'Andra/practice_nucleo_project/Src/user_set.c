#include "main.h"
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include <user_set.h>
#include "u8g2.h"
#include "ssd1306.h"
#include "fonts.h"
#include "stepper.h"

extern uint8_t set_hour, run_time;
extern uint8_t set_min;
extern int feed_amount;
//UART_HandleTypeDef huart2;
int rotationDirection;
uint8_t TurnsDetected;
char buffer_init[10];
int set_hour_ ;
int feed_amount_;
int set_min_ , run, previouscount;

uint8_t user_set_hour(){

	sprintf(buffer_init, "%02d:%02d", 00, 00);
	SSD1306_GotoXY (0, 30);
	SSD1306_Puts ("             ", &Font_11x18, 1);
	//SSD1306_UpdateScreen();
	SSD1306_GotoXY (37, 30);
	SSD1306_Puts (buffer_init, &Font_11x18, 1);
	SSD1306_UpdateScreen();
	HAL_Delay (1000);

	previouscount =0;

	while (HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin)){
		char buffer_hour[10];

		if(TurnsDetected){
		  TurnsDetected = 0;

		if (rotationDirection && set_hour_ != 23){
			set_hour_+=1;
		}
		else if (rotationDirection && set_hour_ ==23){
			set_hour_ = 0;
		}
		else if (!rotationDirection && set_hour_ !=0){
			set_hour_-=1;
		}
		else if (!rotationDirection && set_hour_ ==0){
			set_hour_= 23;
		}

		sprintf(buffer_hour, "%02d:%02d", set_hour_, 00);
		SSD1306_GotoXY (0, 30);
		SSD1306_Puts ("             ", &Font_11x18, 1);
		//SSD1306_UpdateScreen();
		SSD1306_GotoXY (37, 30);
		SSD1306_Puts (buffer_hour, &Font_11x18, 1);
		SSD1306_UpdateScreen();
//		HAL_Delay (500);

//		if(set_hour_!= previouscount ){
//			SSD1306_GotoXY (37, 30);
//			SSD1306_Puts (buffer_hour, &Font_11x18, 1);
////			SSD1306_UpdateScreen();
//		}
//		previouscount = set_hour_;

		}

	}

		if(!HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin)){
			set_hour = set_hour_;
//			char buffer_test[10];
//			sprintf(buffer_test, "%02d:%02d\r\n", 00, 00);
//			HAL_UART_Transmit(&huart2, (uint8_t *)buffer_test, strlen(buffer_test), HAL_MAX_DELAY);
			HAL_Delay(1000);
			user_set_min();

		}
		set_hour_ = 0;
		return 0;
}

uint8_t user_set_min(){
	previouscount =0;

	while (HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin))
	{
		char buffer_min[10];
		if(TurnsDetected)
		{
		  TurnsDetected = 0;
			if (rotationDirection && set_min_ !=59){
				set_min_+=1;
			}
			else if (rotationDirection && set_min_ ==59){
				set_min_ = 0;
			}
			else if (!rotationDirection && set_min_ !=0){
				set_min_-=1;
			}
			else if (!rotationDirection && set_min_ ==0){
				set_min_= 59;
			}

			sprintf(buffer_min, "%02d:%02d", set_hour, set_min_);
			SSD1306_GotoXY (0, 30);
			SSD1306_Puts ("             ", &Font_11x18, 1);
			//SSD1306_UpdateScreen();
			SSD1306_GotoXY (37, 30);
			SSD1306_Puts (buffer_min, &Font_11x18, 1);
			SSD1306_UpdateScreen();
//			HAL_Delay (500);
//			if(set_min_!= previouscount ){
//				SSD1306_GotoXY (37, 30);
//				SSD1306_Puts ("     ", &Font_11x18, 1);
////				SSD1306_UpdateScreen();
//			}
//			previouscount = set_min_;
		}

	}

	if(!HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin)){
		set_min = set_min_;
		HAL_Delay(1000);
		set_min_ =0;
		return 0;
	}
		return 0;
}

uint8_t user_set_feeding_amount(){
	previouscount =0;

	SSD1306_Fill(0); //clear oled
	SSD1306_GotoXY (0,0);
	SSD1306_Puts ("Meals Per Day", &Font_7x10, 1);

	sprintf(buffer_init, "%2d", feed_amount_);
	SSD1306_GotoXY (0, 30);
	SSD1306_Puts ("             ", &Font_11x18, 1);
	//SSD1306_UpdateScreen();
	SSD1306_GotoXY (37, 30);
	SSD1306_Puts (buffer_init, &Font_11x18, 1);
	SSD1306_UpdateScreen();
	HAL_Delay (1000);

	while (HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin))
	{
		char buffer_feed_amount[10];
		if(TurnsDetected)
		{
		  TurnsDetected = 0;

			if (rotationDirection && feed_amount_ !=4){
				feed_amount_+=1;
			}
			else if (rotationDirection && feed_amount_ ==4){
				feed_amount_ = 0;
			}
			else if (!rotationDirection && feed_amount_ !=0){
				feed_amount_-=1;
			}
			else if (!rotationDirection && feed_amount_ ==0){
				feed_amount_= 4;
			}
			sprintf(buffer_feed_amount, "%2d", feed_amount_);
			SSD1306_GotoXY (0, 30);
			SSD1306_Puts ("             ", &Font_11x18, 1);
		//	SSD1306_UpdateScreen();
			SSD1306_GotoXY (37, 30);
			SSD1306_Puts (buffer_feed_amount, &Font_11x18, 1);
			SSD1306_UpdateScreen();
//			HAL_Delay (500);
//			if(feed_amount_!= previouscount ){
//				SSD1306_GotoXY (37, 30);
//				SSD1306_Puts ("     ", &Font_11x18, 1);
////				SSD1306_UpdateScreen();
//			}
//			previouscount = feed_amount_;
		}

	}

	if(!HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin)){
		feed_amount = feed_amount_;
		HAL_Delay(1000);
		feed_amount_ =0;
		return 0;
	}
		return 0;
}

uint8_t user_run_auger(){
	run=0;
	  SSD1306_Fill(0); //clear oled
	  SSD1306_GotoXY (0,0);
	  SSD1306_Puts ("Run Feeder Until", &Font_7x10, 1);
	  SSD1306_GotoXY (0,15);
	  SSD1306_Puts ("Amount Satisfied", &Font_7x10, 1);
	  SSD1306_UpdateScreen();

	while (1){
		  while(!HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin)){
			  	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);//sets pb4 to high
			  	  		  TIM1->CCR1 = 250;//duty cycle 50 percent, same as	  htim1.Instance->CCR1 = 250;
			  	  		  run++;
			  	  		  HAL_Delay(1000);//Delay in ms, 3 seconds

		  }
		  if( run >= 3){
  	  		  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);//sets pb4 to high

			  break;
		  }
	}
	run_time = run;
	run=0;
	return 0;
}

