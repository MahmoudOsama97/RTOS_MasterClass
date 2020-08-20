
/* Standard includes. */
#include <stdlib.h>
#include <stdio.h>

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "lpc21xx.h"
#include "semphr.h"
/* Peripheral includes. */
#include "serial.h"
#include "GPIO.h"

#define HIGH PIN_IS_HIGH
#define LOW PIN_IS_LOW
#define pressed PIN_IS_HIGH /* on real hardware it was  low */

const signed char s100 [] = "task100\r\n";
const signed char s500 [] = "task500\r\n";
volatile int dummy =0;
SemaphoreHandle_t xSemaphore;
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



void Task100(void *pvParameters)
{
	int i;
	
	
	for (;;) {
		if ( xSemaphoreTake( xSemaphore,portMAX_DELAY) == pdTRUE ) {
							for (i =0; i<10; i++)
							{							
							vSerialPutString(s100,9); 
							}
					
					
							 xSemaphoreGive( xSemaphore );
							vTaskDelay(100);
						}
		
					}

		
	}



	

void Task500(void *pvParameters)
{
	int k;
	int j;
 
	 
	for (;;) {	
					if ( xSemaphoreTake( xSemaphore, portMAX_DELAY) == pdTRUE ) {
						for ( j=0; j<10; j++)
							{
							
						for ( k =0; k<100000; k++) dummy=0;//heavyload
							
							vSerialPutString(s500,9); 
							}
					
					 xSemaphoreGive( xSemaphore );
							vTaskDelay(500);
		
						}
					
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
	
	
			 xSemaphore = xSemaphoreCreateMutex();
	xTaskCreate(Task100, "Task100", 100, NULL,2, NULL);

	xTaskCreate(Task500, "Task500", 100, NULL, 1, NULL);
		
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

