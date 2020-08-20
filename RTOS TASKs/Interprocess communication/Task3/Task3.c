
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "semphr.h"
#include "queue.h"
/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

#define HIGH PIN_IS_HIGH
#define LOW PIN_IS_LOW
#define pressed PIN_IS_HIGH /* on real hardware it was  low */

TaskHandle_t xHandle1 = NULL;
TaskHandle_t xHandle2 = NULL;
TaskHandle_t xHandle3 = NULL;
TaskHandle_t xHandle4 = NULL;
QueueHandle_t xQueue1;
/*-----------------------------------------------------------*/

/* Constants to setup I/O and processor. */
#define mainBUS_CLK_FULL	( ( unsigned char ) 0x01 )

/* Constants for the ComTest demo application tasks. */
#define mainCOM_TEST_BAUD_RATE	( ( unsigned long ) 115200 )


/*
 * Configure the processor for use with the Keil demo board.  This is very
 * minimal as most of the setup is managed by the settings in the project
 * file.
 */
static void prvSetupHardware( void );
/*-----------------------------------------------------------*/



void Taskbutton1(void *pvParameters)
{
	char currentButtonState;
	char PrevButtonState;
	const signed char s1Rise[] = "t1R\r\n";
	const signed char s1fall[] = "t1f\r\n";
	for (;;) {
		
		currentButtonState=GPIO_read(PORT_0,PIN0);
		if((currentButtonState==0)&&(PrevButtonState==1)){
			 xQueueSend( xQueue1, ( void * ) &s1Rise, ( TickType_t ) 0 );
		}
		if((currentButtonState==1)&&(PrevButtonState==0)){
			 xQueueSend( xQueue1, ( void * ) &s1fall, ( TickType_t ) 0 );
		}
		PrevButtonState=currentButtonState;
		vTaskDelay(50);
	}
}



	

void Taskbutton2(void *pvParameters)
{
	 char currentButtonState;
	char PrevButtonState;
	const signed char s2Rise[] = "t2R\r\n";
	const signed char s2fall[] = "t2f\r\n";
	for (;;) {
		
		currentButtonState=GPIO_read(PORT_0,PIN1);
		if((currentButtonState==0)&&(PrevButtonState==1)){
			 xQueueSend( xQueue1, ( void * ) &s2Rise, ( TickType_t ) 0 );
		}
		if((currentButtonState==1)&&(PrevButtonState==0)){
			 xQueueSend( xQueue1, ( void * ) &s2fall, ( TickType_t ) 0 );
		}
		PrevButtonState=currentButtonState;
		vTaskDelay(50);
	}
}


void TaskPeriodic(void *pvParameters)
{

	const signed char s3[] = "t3p\r\n";
	 
	for (;;) {	
		xQueueSend( xQueue1, ( void * ) &s3, ( TickType_t ) 0 );
		vTaskDelay(100);
		 			}	
}

void TaskUART(void *pvParameters)
{
	 const signed char s4[6];
	for (;;) {	
				 xQueueReceive(xQueue1,( void * ) &s4, ( TickType_t ) 0  );
		 vSerialPutString(s4,6 );
		vTaskDelay(20);
		 			}	
}











/*
 * Application entry point:
 * Starts all the other tasks, then starts the scheduler. 
 */
int main( void )
{
	/* Setup the hardware for use with the Keil demo board. */
	prvSetupHardware();

	
    /* Create Tasks here */
	
	
			 
	xTaskCreate(Taskbutton1, "Task1", 100, ( void * ) 1,4, &xHandle1);

	xTaskCreate(Taskbutton2, "Task2", 100, ( void * ) 1, 3, &xHandle2);
	
	xTaskCreate(TaskPeriodic, "Task3", 100, ( void * ) 1, 2, &xHandle3);
	
	xTaskCreate(TaskUART, "Task4", 100, ( void * ) 1, 1, &xHandle4);
	
	 xQueue1= xQueueCreate(  100,6 );
		
	vTaskStartScheduler();

	/* Should never reach here!  If you do then there was not enough heap
	available for the idle task to be created. */
	for( ;; );
}
/*-----------------------------------------------------------*/

static void prvSetupHardware( void )
{
	/* Perform the hardware setup required.  This is minimal as most of the
	setup is managed by the settings in the project file. */

	/* Configure UART */
	xSerialPortInitMinimal(mainCOM_TEST_BAUD_RATE);
	
	/* Configure GPIO */
	GPIO_init();

	/* Setup the peripheral bus to be the same as the PLL output. */
	VPBDIV = mainBUS_CLK_FULL;
}
/*-----------------------------------------------------------*/

