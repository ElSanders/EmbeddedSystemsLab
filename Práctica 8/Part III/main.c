#include "sam.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "uart.h"
#include "myprintf.h"

/* Priorities at which the tasks are created. */
#define myTASK_TASK_PRIORITY         	( tskIDLE_PRIORITY + 1 )



xQueueHandle Global_Queue_Handle = 0;




void sender_Task(void *p){
	static int but = 0; // 1 = Up, 2 = Down, 3 = Left, 4 = Right;
	while (1) {
		
		if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA14 ) ) == _U_( 0x00000000 ) ){
			//presionado UP
			but = 1;
			myprintf("Send Button pressed: %d to receiver task\n", but);
		if (!xQueueSend(Global_Queue_Handle, &but, 1000)) {
			myprintf("Failed to send to queue\n");
		}
	
		}
		 else if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA09 ) ) == _U_( 0x00000000 ) ){
			//presionado Down
			but = 2;
			myprintf("Send Button pressed: %d to receiver task\n", but);
		if (!xQueueSend(Global_Queue_Handle, &but, 1000)) {
			myprintf("Failed to send to queue\n");
		}
		
		}
		else if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA08 ) ) == _U_( 0x00000000 ) ){
			//presionado Left
			but = 3;
			myprintf("Send Button pressed: %d to receiver task\n", but);
		if (!xQueueSend(Global_Queue_Handle, &but, 1000)) {
			myprintf("Failed to send to queue\n");
		}
		
		}
		else if( ( PORT->Group[ 0 ].IN.reg & PORT_IN_IN( PORT_PA15 ) ) == _U_( 0x00000000 ) ){
			//presionado Right
			but = 4;
			myprintf("Send Button pressed: %d to receiver task\n", but);
		if (!xQueueSend(Global_Queue_Handle, &but, 1000)) {
			myprintf("Failed to send to queue\n");
		}
			
		}else continue;
		
		
		
		vTaskDelay(2000);
	}
}

void receiver_Task(void *p){
	int rx_int = 0;
	while (1) {
		if (xQueueReceive(Global_Queue_Handle, &rx_int, 1000)){
			switch (rx_int){
				case 1:
				myprintf("Received %d: UP\n", rx_int);
				break;
				case 2:
				myprintf("Received %d: DOWN\n", rx_int);
				break;
				case 3:
				myprintf("Received %d: LEFT\n", rx_int);
				break;
				case 4:
				myprintf("Received %d: RIGHT\n", rx_int);
				break;
			}
		}
		else {
			myprintf("Failed to receive data from queue \n");
		}
	}
}

int main()
{
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	
	SYSCTRL->OSC8M.bit.PRESC = 0;
	initUART();
	//config for arduino
	PORT->Group[0].PINCFG[PIN_PA14].reg = 0x02; 
	PORT->Group[0].PINCFG[PIN_PA09].reg = 0x02; 
	PORT->Group[0].PINCFG[PIN_PA08].reg = 0x02; 
	PORT->Group[0].PINCFG[PIN_PA15].reg = 0x02;
	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA14; //pin 14, UP
	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA09; //pin 09, DOWN
	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA08; //pin 04, LEFT
	PORT->Group[ 0 ].DIRCLR.reg = PORT_PA15; //pin 15, RIGHT
	// Create the queue
	Global_Queue_Handle = xQueueCreate(3, sizeof(int));
	xTaskCreate(sender_Task, (const char * const) "tx", 512, NULL, myTASK_TASK_PRIORITY, NULL);
	xTaskCreate(receiver_Task, (const char * const) "rx", 512, NULL, myTASK_TASK_PRIORITY, NULL);
	
	/* Start the tasks and timer running. */
	vTaskStartScheduler();

	for( ;; ){
		
		
	}
	return(0);
}
