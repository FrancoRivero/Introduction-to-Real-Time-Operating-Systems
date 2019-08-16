/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "board.h"

#include "pin_mux.h"
#include "clock_config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Task priorities. */
#define standard_task_PRIORITY (configMAX_PRIORITIES - 1)

#define MAX_STATES 2
#define QUEUE_STACK 10
#define QUEUE_ITEM_SIZE sizeof(char *)


/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void sync_task(void *pvParameters);
static void async_task(void *pvParameters);
static void print_task(void *pvParameters);


// Global Variables
uint8_t state = 0;
bool change_state=false;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Application entry point.
 */
int main(void){
    /* Init board hardware. */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();

    LED_RED_INIT(LOGIC_LED_OFF);
    LED_GREEN_INIT(LOGIC_LED_OFF);
    LED_BLUE_INIT(LOGIC_LED_OFF);

    QueueHandle_t xQueue;
    xQueue = xQueueCreate( QUEUE_STACK, QUEUE_ITEM_SIZE);

    vTraceEnable(TRC_START);
    xTaskCreate(sync_task, "Synchronic_task", 200, xQueue, standard_task_PRIORITY, NULL);
    xTaskCreate(async_task, "Asynchronic_task", configMINIMAL_STACK_SIZE, xQueue, standard_task_PRIORITY, NULL);
    xTaskCreate(print_task, "Print_task", configMINIMAL_STACK_SIZE, xQueue, standard_task_PRIORITY, NULL);
    vTaskStartScheduler();
    while(1){}
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void async_task(void *pvParameters){
	TickType_t xDelay;
	QueueHandle_t xQueue ;
	xQueue  = (QueueHandle_t) pvParameters;
	xDelay = 200 / portTICK_PERIOD_MS;
	char * char_message;
	const char* phrase;
	while(1){
		if(change_state){
			LED_BLUE_OFF();
			LED_GREEN_OFF();
			LED_RED_OFF();
			while(change_state);

			uint8_t randy = rand()/5;
			if(randy <86){
			    phrase = "franco 1994";
			}else if(randy<=170 && randy >86){
				phrase = "franco fabian rivero 571994";
			}else{
				phrase = "franco fabian rivero 571994 38111351 coltirolesa";
			}
			int size = strlen(phrase);
			char_message = pvPortMalloc( (size + 1) * sizeof(char));
			strcpy(char_message,phrase);
			char_message[size] = '\0';

			xQueueSendToBack(xQueue, &char_message, 100);
		}
		vTaskDelay(xDelay);
	}
}

static void print_task(void *pvParameters){
	QueueHandle_t xQueue ;
	xQueue  = (QueueHandle_t) pvParameters;
	char * received_message;
	while(1){
		if(xQueueReceive(xQueue,&(received_message),portMAX_DELAY) == pdTRUE ){
			PRINTF("%s\r\n",received_message);
			vPortFree(received_message);
		}
	}
}

static void sync_task(void *pvParameters){
	TickType_t xDelay;
	QueueHandle_t xQueue ;
	xQueue  = (QueueHandle_t) pvParameters;
	char * char_message;

	while(1){
		xDelay = 1500 / portTICK_PERIOD_MS;
    	switch(state) {
    		case 0:
    			LED_BLUE_TOGGLE();
    			LED_GREEN_TOGGLE();
			break;
    		case 1:
    			LED_GREEN_TOGGLE();
    			LED_RED_TOGGLE();
    			LED_BLUE_TOGGLE();
    		break;
    		default:
    		break;
    	}
    	int temp = rand() % 50;
    	int len = 15;
		char_message = pvPortMalloc( (len+1) * sizeof(char));
		sprintf(char_message,"Temp es: %d", temp);
		char_message[len] = '\0';
        xQueueSendToBack(xQueue, &char_message,100);
        vTaskDelay(xDelay);
    }
}

void BOARD_SW2_IRQ_HANDLER(){
    uint32_t result = GPIO_ReadPinInput(BOARD_SW2_GPIO,BOARD_SW2_GPIO_PIN);
    if(result == 0){
    	//mantengo la tarea al mantener apretado para notar en tracealizer
    	change_state = true;
    	if (++state >= MAX_STATES) {
    		state = 0;
    	}
    } else {
    	change_state =false;
    }
	PORT_ClearPinsInterruptFlags(BOARD_SW2_PORT, 1<<BOARD_SW2_GPIO_PIN);
}



