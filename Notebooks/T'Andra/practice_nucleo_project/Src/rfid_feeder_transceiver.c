#include "rfid_feeder_transceiver.h" // Header file for RFID module functions
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_it.h"
#include "stm32f4xx_hal_gpio.h"
#include "main.h"
#include "u8g2.h"
#include "stepper.h"
#include "fonts.h"
#include "ssd1306.h"

extern uint32_t lastFeederReceiveTime;


void TransmitRange() {
    // Function to open the box
    // Send command to RFID card to trigger opening the box
	//preamble: notify when receiver to start listening
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    HAL_Delay(5/100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_Delay(5/100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    HAL_Delay(5/100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_Delay(5/100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    HAL_Delay(5/100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_Delay(5/100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);
    HAL_Delay(5/100);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);
    HAL_Delay(5/100);

// x52 = "R"
	 HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);//
     HAL_Delay(5/100);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);//
     HAL_Delay(5/100);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);//5
     HAL_Delay(5/100);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);//
     HAL_Delay(5/100);

     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);//
     HAL_Delay(5/100);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);//
     HAL_Delay(5/100);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 1);//2
     HAL_Delay(5/100);
     HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, 0);//
     HAL_Delay(5/100);


}


void CheckReceive() {
	uint32_t start_listening = 0;
	uint32_t message [8];
	HAL_Delay(50/100);
	if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_5)){
		start_listening++;
		HAL_Delay(5/100);
		while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5)){
			start_listening++;
			HAL_Delay(10/100);

	}
		//sprintf(buffer_encoder, "%02d:%02d \r\n", count, 00);
		//HAL_UART_Transmit(&huart2, (uint8_t *)buffer_encoder, strlen(buffer_encoder), HAL_MAX_DELAY);
	if(start_listening == 4){
		for(int i=0 ; i <7; i++ ){
			message[i] = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_5);
			HAL_Delay(6/100);
		}
		if(message[0]==0 && message[1]==1 && message[2]==0 && message[3]==0 && message[4]==	1 && message[5]==1 && message[6]==1 ){
        	 lastFeederReceiveTime = HAL_GetTick();
			 motorOn();
			 stepCV(200,150.0f); // Counter Clockwise 100 steps 150 RPM
			 motorOff();
			 HAL_Delay(110);
		}}
			start_listening =0;

	}

}

