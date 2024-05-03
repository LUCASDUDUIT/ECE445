/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include <user_set.h>
#include "u8g2.h"
#include "stepper.h"
#include "rfid_feeder_transceiver.h"
#include "stm32f4xx_it.h"
#include "fonts.h"
#include "ssd1306.h"
#include "RH_ASK.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
extern I2C_HandleTypeDef hi2c1;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
//I2C_HandleTypeDef hi2c1;

RTC_HandleTypeDef hrtc;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
//extern uint8_t u8x8_stm32_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
//extern uint8_t u8x8_byte_stm32_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
//extern uint8_t u8x8_byte_4wire_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


//static u8g2_t u8g2;


int count, stepNumber, feed_amount, dispense_flag, run_time, run_, empty_flag;
uint8_t set_hour, set_min, init_hour, init_min;
uint8_t door_open_time_hour = 60;
uint8_t door_open_time_min = 60;
uint32_t lastFeederReceiveTime;
int feeding_times[];
int reset_state=0;
int close_flag =0;
int do_not_dispense =0;
int open_door_flag =0;
int run_motor=0;
//int rotationDirection;
//uint8_t TurnsDetected;



/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_I2C1_Init(void);
static void MX_RTC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_I2C1_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

 // RH_ASK_Initialization();
  HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
 // HAL_TIM_Base_Start(&htim1);//TIMER INITIALIZATION FOR THE DELAY ON MICRO SECONDS, stepper code
  HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_ALL);//initialization for encoder first example
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);

//stepper init
  HAL_TIM_Base_Start(&htim3);
  stepNumber = 1;




  SSD1306_Init();
  SSD1306_Fill(0); //clear oled
  SSD1306_GotoXY (0,0);
  SSD1306_Puts ("Set Current Time", &Font_7x10, 1);
  SSD1306_UpdateScreen();
  user_set_hour();

  //below is a test to see if return 0; takes us here
  char buffer_test[50];
  sprintf(buffer_test, "%02d:%02d\r\n", set_hour, set_min);
  HAL_UART_Transmit(&huart2, (uint8_t *)buffer_test, strlen(buffer_test), HAL_MAX_DELAY);
  HAL_Delay(1000);

  //take user input and store it in rtc
  RTC_TimeTypeDef time_;
  time_.Hours = set_hour;
  time_.Minutes = set_min;

  //RTC_TimeTypeDef Usertime;
 // Usertime.Hours = set_hour;
//  init_hour= set_hour-1;
 // Usertime.Minutes = set_min;
//  init_min = set_min;
 //HAL_RTC_SetTime(&hrtc, &Usertime, RTC_FORMAT_BIN);




  SSD1306_Fill(0); //clear oled
  SSD1306_GotoXY (0,0);
  SSD1306_Puts ("Meals Per Day", &Font_7x10, 1);
  SSD1306_UpdateScreen();

  user_set_feeding_amount();

  //store hour and mins in a buffer to compare later to RTC(current time) for dispensing
  SSD1306_Fill(0); //clear oled
  SSD1306_GotoXY (0,0);
  SSD1306_Puts ("Set Meal Times", &Font_7x10, 1);
  SSD1306_UpdateScreen();

  int feeding_times[feed_amount*2];
  for(int i=0; i < feed_amount*2; i+=2){
	  user_set_hour();
	  feeding_times[i] = set_hour;
	  feeding_times[i+1] = set_min;

	    sprintf(buffer_test, "%02d :%02d\r\n ", feeding_times[i], feeding_times[i+1]);
	    HAL_UART_Transmit(&huart2, (uint8_t *)buffer_test, strlen(buffer_test), HAL_MAX_DELAY);

  }





  //"run auger until satisfied for amount"
  SSD1306_Fill(0); //clear oled
  SSD1306_GotoXY (0,0);
  SSD1306_Puts ("Run Feeder Until", &Font_7x10, 1);
  SSD1306_GotoXY (0,15);
  SSD1306_Puts ("Amount Satisfied", &Font_7x10, 1);
  SSD1306_UpdateScreen();

  user_run_auger();

  SSD1306_Fill(SSD1306_COLOR_BLACK);
  SSD1306_UpdateScreen();


  sprintf(buffer_test, "%02d\r\n ", run_time);
  HAL_UART_Transmit(&huart2, (uint8_t *)buffer_test, strlen(buffer_test), HAL_MAX_DELAY);
  HAL_Delay(1000);

  HAL_RTC_SetTime(&hrtc, &time_, RTC_FORMAT_BIN);

 //HAL_RTC_SetTime(&hrtc, &Usertime, RTC_FORMAT_BIN);







  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



//	  //real time clock code
	 // 	RTC_TimeTypeDef time_;
	  	HAL_RTC_GetTime(&hrtc, &time_, RTC_FORMAT_BIN);

	  	char buffer_test[50];

	  	char buffer[50];
	  	char buffer1[50];

	  	sprintf(buffer1, "%02d:%02d:%02d\r\n", time_.Hours , time_.Minutes, time_.Seconds);
	  	HAL_UART_Transmit(&huart2, (uint8_t *)buffer1, strlen(buffer1), HAL_MAX_DELAY);

	  	sprintf(buffer, "%02d:%02d", time_.Hours, time_.Minutes);

//	  	SSD1306_GotoXY (0, 30);
//	  	SSD1306_Puts ("             ", &Font_11x18, 1);
//	  	SSD1306_UpdateScreen();
		SSD1306_GotoXY (37, 30);
		SSD1306_Puts (buffer, &Font_11x18, 1);
		SSD1306_UpdateScreen();
	  	HAL_Delay(1000);


	  	// DC MOTOR CODE
	  	for(int i=0; i < feed_amount*2; i+=2)
	  	{
		  if(time_.Hours == feeding_times[i])
		  {			  	//char *test = "wont dispense? 1\r\n";

			  if(time_.Minutes == feeding_times[i+1] && !do_not_dispense) //if feeding time is equal and food is not overflown, dispense
			  {

				  run_ = run_time;
				  while(run_ !=0)
				  {
				 	  char *test = "dc motor on\r\n";
				  	  HAL_UART_Transmit(&huart2, (uint8_t *)test, strlen(test), HAL_MAX_DELAY);

				  	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);//sets pb4 to high
				  	  //TIM1->CCR1 = 250;//duty cycle 50 percent, same as	  htim1.Instance->CCR1 = 250;
					  run_--;
					  HAL_Delay(1000);//Delay in ms, 3 seconds


				  }
				  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 0);//turn motor off

//			  	  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);//send signal to allow dog to come to feeder and eat
			  	  do_not_dispense =1;

				 // HAL_Delay(45000);
				  break;
			  }
		  }
		  //i++;
	   }

	  	 	 	 	 	 	 if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5) && !close_flag && do_not_dispense ){//if we get a signal back open dog feeder.

	  					 	  	 char *test = "stepper motor open\r\n";
	  					  		 HAL_UART_Transmit(&huart2, (uint8_t *)test, strlen(test), HAL_MAX_DELAY);
	  					  		 run_motor=1;
	  					  		 motorOn();
	  							 stepCV(200,150.0f); // Counter Clockwise 100 steps 150 RPM
	  							 motorOff();
	  	//						 HAL_Delay(1000);
	  						  	//RTC_TimeTypeDef time_for_CLOSE;

	  						  	 HAL_RTC_GetTime(&hrtc, &time_, RTC_FORMAT_BIN);

	  							 door_open_time_hour= time_.Hours;
	  							 door_open_time_min= time_.Minutes;
	  						  	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);//stop transmitting
	  							 close_flag=1;
	  							 open_door_flag = 0;

	  							    	char test_ [10];
	  								    sprintf(test_,"%02d:%02d", door_open_time_hour, door_open_time_min );
	  								    HAL_UART_Transmit(&huart2, (uint8_t *)test_, strlen(test_), HAL_MAX_DELAY);
	  					  }

//  	 	 	 	 	 		 uint8_t diff_mins= time_.Minutes - door_open_time_min;
//
//	  	 	 	 	 	 	 while(diff_mins < 1){
//	  	 	 	 	 	 		 diff_mins= time_.Minutes - door_open_time_min;
//	  	 	 	 	 	 		 if(run_motor){
//	  					  		 motorOn();
//	  							 stepCV(200,150.0f); // Counter Clockwise 100 steps 150
//	  							 motorOff();
//
//	  	 	 	 	 	 	 }
//	  	 	 	 	 	 	 }
	  	 	 	 	 	 	// run_motor=0;

	  	if(close_flag){
		  	uint8_t diff_mins =0;
		  //	HAL_RTC_GetTime(&hrtc, &time_, RTC_FORMAT_BIN);

	  	if(door_open_time_hour == time_.Hours){
	  		char *test = "same hour\r\n";
	  		HAL_UART_Transmit(&huart2, (uint8_t *)test, strlen(test), HAL_MAX_DELAY);

	  	    diff_mins= time_.Minutes - door_open_time_min;
			if(diff_mins >= 1){


			  	char *test = "greater than 1\r\n";
			    HAL_UART_Transmit(&huart2, (uint8_t *)test, strlen(test), HAL_MAX_DELAY);
			    char test_ [10];
			    sprintf(test_,"%02d\r\n", time_.Minutes);
			    HAL_UART_Transmit(&huart2, (uint8_t *)test_, strlen(test_), HAL_MAX_DELAY);

				 motorOn();
				 stepCCV(200,150.0f); // Counter Clockwise 100 steps 150 RPM
				 motorOff();
				  	close_flag=0;
				do_not_dispense = 0;
				run_motor=0;

			}}
	  	else if(door_open_time_hour != time_.Hours){
	  		char *test = "diff hour\r\n";
	  		HAL_UART_Transmit(&huart2, (uint8_t *)test, strlen(test), HAL_MAX_DELAY);

		    	char test_ [10];
			    sprintf(test_,"%02d\r\n", time_.Hours);
			    HAL_UART_Transmit(&huart2, (uint8_t *)test_, strlen(test_), HAL_MAX_DELAY);
	  		diff_mins = (time_.Minutes + (60 - door_open_time_min));
	  		if(diff_mins >= 1){


				    char test_ [10];
					    sprintf(test_,"%02d\r\n", time_.Minutes);
					    HAL_UART_Transmit(&huart2, (uint8_t *)test_, strlen(test_), HAL_MAX_DELAY);

			  		char *test = "greater than 1\r\n";
			  		HAL_UART_Transmit(&huart2, (uint8_t *)test, strlen(test), HAL_MAX_DELAY);

				 motorOn();
				 stepCCV(200,150.0f); // Counter Clockwise 100 steps 150 RPM
				 motorOff();
				  	close_flag=0;
				  	do_not_dispense = 0;
					run_motor=0;




	  		}
	  	}
	  	}


//	  	while(readInfraredDetector_EMPTY()){ //after dispensing, if nothing is detected, flash oled
////	  		flash OLED
//	  		SSD1306_Fill(SSD1306_COLOR_WHITE);
//	  		SSD1306_Fill(SSD1306_COLOR_BLACK);//0
//	  	}


//this is the dc motor, which runs the auger, should only run in one direction
//	  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);//sets pb4 to high
//	  TIM1->CCR1 = 250;//duty cycle 50 percent, same as	  htim1.Instance->CCR1 = 250;
//	  HAL_Delay(3000);//Delay in ms, 3 seconds


//ALTERNATIVE STEPPER MOTOR CODE
//	  	 motorOn();
//	     stepCV(200,150.0f); // Clockwise 200 steps 150 RPM
//	     motorOff();
//	     HAL_Delay(2000);
//
//	     motorOn();
//	     stepCCV(200,150.0f); // Counter Clockwise 100 steps 150 RPM
//	     motorOff();
//	     HAL_Delay(2000);




//OLED CODE
//	u8g2_FirstPage(&u8g2);
//	do
//	{
//		 u8g2_SetFont(&u8g2, u8g2_font_ncenB14_tr);
//		 u8g2_DrawStr(&u8g2, 0, 15, "Set Time");
//		 u8g2_DrawCircle(&u8g2, 64, 40, 10, U8G2_DRAW_ALL);
//	} while (u8g2_NextPage(&u8g2));

//ALTERNATIVE OLED CODE
//	  for (int num=1;num<=1000;num++)
//	  		{
//	  			itoa(num,string, 10);
//	  			SSD1306_GotoXY (0, 30);
//	  			SSD1306_Puts ("             ", &Font_16x26, 1);
//	  			SSD1306_UpdateScreen();
//	  			if(num<10) {
//	  				SSD1306_GotoXY (53, 30);  // 1 DIGIT
//	  			}
//	  			else if (num<100) {
//	  				SSD1306_GotoXY (45, 30);  // 2 DIGITS
//	  			}
//	  			else  {
//	  				SSD1306_GotoXY (37, 30);  // 3 DIGITS
//	  			}
//	  			SSD1306_Puts (string, &Font_16x26, 1);
//	  			SSD1306_UpdateScreen();
//	  			HAL_Delay (500);
//
//	  		}


//IR SENSOR CODE
//  // Turn on SEN-18772 (IR emitter)
//	controlInfraredEmitter(1);
//
//	//Read SEN-19018 (IR detector)
//	uint8_t detection = readInfraredDetector();
//
//	// Your code logic here based on the detection result
// 		if(detection){
// 			//stop dispensing until ir detects nothing
//				 motorOff();
//				 HAL_Delay(2000);
// 		}
// //Delay for a short period to avoid reading too frequently
//	HAL_Delay(1000); // Adjust delay as needed


//PUSH BUTTON CODE
//HAL_GPIO_ReadPin(SELECT_BUTTON_GPIO_Port, SELECT_BUTTON_Pin)



// ROTARY ENCODER CODE
//
//char buffer_encoder[50];
//if(TurnsDetected){
//  TurnsDetected = 0;
//  if(rotationDirection){
//	  count+=1;
//  }
//
//  else if(!rotationDirection){
//	  count-=1;
//
//  }
//}
//sprintf(buffer_encoder, "%02d:%02d \r\n", count, 00);
//HAL_UART_Transmit(&huart2, (uint8_t *)buffer_encoder, strlen(buffer_encoder), HAL_MAX_DELAY);
//HAL_Delay(1000);


	 // HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);

	  //TransmitRange();

	  //HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);

//	  if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)){
		 // CheckReceive();
//	  }

//	  if((HAL_GetTick() - lastFeederReceiveTime) >= 30000) {
//		  //add stepper motor code that closes feeder
//		 motorOn();
//		 stepCCV(200,150.0f); // Counter Clockwise 100 steps 150 RPM
//		 motorOff();
//		 HAL_Delay(1000);
//	  }




    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 400000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef sDate = {0};
  RTC_AlarmTypeDef sAlarm = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 127;
  hrtc.Init.SynchPrediv = 255;
  hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
  hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */

  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x00;
  sTime.Minutes = 0x00;
  sTime.Seconds = 0x00;
  sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sTime.StoreOperation = RTC_STOREOPERATION_RESET;
  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  sDate.WeekDay = RTC_WEEKDAY_MONDAY;
  sDate.Month = RTC_MONTH_JANUARY;
  sDate.Date = 0x1;
  sDate.Year = 0x0;

  if (HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable the Alarm A
  */
  sAlarm.AlarmTime.Hours = 0x0;
  sAlarm.AlarmTime.Minutes = 0x0;
  sAlarm.AlarmTime.Seconds = 0x1;
  sAlarm.AlarmTime.SubSeconds = 0x0;
  sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  sAlarm.AlarmMask = RTC_ALARMMASK_NONE;
  sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
  sAlarm.AlarmDateWeekDay = 0x1;
  sAlarm.Alarm = RTC_ALARM_A;
  if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 83;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 499;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 65535;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 10;
  sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 10;
  if (HAL_TIM_Encoder_Init(&htim2, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 83;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 65535;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, TRANSMIT_Pin|IR_EMITTER_EMPTY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, IN0_Pin|IN1_Pin|IN2_Pin|IN3_Pin
                          |IR_EMITTER_Pin|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, OLED_DC_Pin|OLED_RST_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ENCODER_DT_Pin ENCODER_CLK_Pin */
  GPIO_InitStruct.Pin = ENCODER_DT_Pin|ENCODER_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : RECEIVE_Pin IR_DETECTOR_EMPTY_Pin */
  GPIO_InitStruct.Pin = RECEIVE_Pin|IR_DETECTOR_EMPTY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : TRANSMIT_Pin IR_EMITTER_EMPTY_Pin */
  GPIO_InitStruct.Pin = TRANSMIT_Pin|IR_EMITTER_EMPTY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : IN0_Pin IN1_Pin IN2_Pin IN3_Pin */
  GPIO_InitStruct.Pin = IN0_Pin|IN1_Pin|IN2_Pin|IN3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : IR_EMITTER_Pin PB4 */
  GPIO_InitStruct.Pin = IR_EMITTER_Pin|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : IR_DETECTOR_Pin */
  GPIO_InitStruct.Pin = IR_DETECTOR_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(IR_DETECTOR_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : OLED_DC_Pin OLED_RST_Pin */
  GPIO_InitStruct.Pin = OLED_DC_Pin|OLED_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : SELECT_BUTTON_Pin */
  GPIO_InitStruct.Pin = SELECT_BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(SELECT_BUTTON_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI1_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */

/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */



/**
  * @brief  Period elapsed callback in non-blocking mode
  * @param  htim TIM handle
  * @retval None
  */
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//	RH_HandleTimerInterrupt_16KHz();
////	g_timerCount++;
//}

// Function to control SEN-18772 (IR emitter)
//void controlInfraredEmitter(int state) {
//    HAL_GPIO_WritePin(IR_EMITTER_GPIO_Port, IR_EMITTER_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//}
//
//// Function to read SEN-19018 (IR detector)
//int readInfraredDetector() {
//    return HAL_GPIO_ReadPin(IR_DETECTOR_GPIO_Port, IR_DETECTOR_Pin);
//}
//
//void controlInfraredEmitter_EMPTY(int state) {
//    HAL_GPIO_WritePin(IR_EMITTER_EMPTY_GPIO_Port, IR_EMITTER_EMPTY_Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
//}
//
//// Function to read SEN-19018 (IR detector)
//int readInfraredDetector_EMPTY() {
//    return HAL_GPIO_ReadPin(IR_DETECTOR_EMPTY_GPIO_Port, IR_DETECTOR_EMPTY_Pin);
//}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	//HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}
//int user_reset(){
//		  SSD1306_Fill(0); //clear oled
//	  	  SSD1306_GotoXY (0,0);
//	  	  SSD1306_Puts ("Set Current Time", &Font_7x10, 1);
//	  	  user_set_hour();
//
//	  	  char buffer_test[50];
//	  	  sprintf(buffer_test, "%02d:%02d\r\n", set_hour, set_min);
//	  	  HAL_UART_Transmit(&huart2, (uint8_t *)buffer_test, strlen(buffer_test), HAL_MAX_DELAY);
//	  	  HAL_Delay(1000);
//
//	  	  //take user input and store it in rtc
//	  	  RTC_TimeTypeDef userTime;
//	  	  userTime.Hours = set_hour+1;
//	  	  userTime.Minutes = set_min;
//	  	  HAL_RTC_SetTime(&hrtc, &userTime, RTC_FORMAT_BIN);
//
//	  	  user_set_feeding_amount();
//
//	  	  //store hour and mins in a buffer to compare later to RTC(current time) for dispensing
//	  	  SSD1306_Fill(0); //clear oled
//	  	  SSD1306_GotoXY (0,0);
//	  	  SSD1306_Puts ("Set Meal Times", &Font_7x10, 1);
//	  	 // SSD1306_UpdateScreen();
//
//	  	  int feeding_times[feed_amount*2];
//		  for(int i=0; i < feed_amount; i++){
//		  user_set_hour();
//		  feeding_times[i] = set_hour;
//		  feeding_times[i+1] = set_min;
//
//			sprintf(buffer_test, "%02d %02d\r\n ", feeding_times[i], feeding_times[i+1]);
//			HAL_UART_Transmit(&huart2, (uint8_t *)buffer_test, strlen(buffer_test), HAL_MAX_DELAY);
//
//			HAL_Delay(1000);
//
//
//	  	  }
//	  	  user_run_auger();
//
//	  	  SSD1306_Fill(SSD1306_COLOR_BLACK);
//	  	  SSD1306_UpdateScreen();
//
//
//	  	  sprintf(buffer_test, "%02d\r\n ", run_time);
//	  	  HAL_UART_Transmit(&huart2, (uint8_t *)buffer_test, strlen(buffer_test), HAL_MAX_DELAY);
//	  	  HAL_Delay(1000);
//
//	  	  reset_state=0;
//	  	  return 0;
//}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
