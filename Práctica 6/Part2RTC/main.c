/*
 * RTC_i2C.c
 *
 * Created: 26/09/2019 10:05:21 p. m.
 * Author : Asus X556U
 */ 

#include "sam.h"
#include "stdbool.h"
#include "RTCControl.h"
#include "myprintf.h"

#define SLAVE_ADDR 0x68u
#define BUF_SIZE 4

// Control variables for data transfer and reception
volatile uint32_t i;

void UARTInit(void) {
	/* Initialize the SAM system */
	SystemInit();
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;
	
	/* port mux configuration*/
	PORT->Group[0].DIR.reg |= (1 << 10);                  /* Pin 10 configured as output */
	PORT->Group[0].PINCFG[PIN_PA11].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	PORT->Group[0].PINCFG[PIN_PA10].bit.PMUXEN = 1;       /* Enabling peripheral functions */
	
	/*PMUX: even = n/2, odd: (n-1)/2 */
	PORT->Group[0].PMUX[5].reg |= 0x02;                   /* Selecting peripheral function C */
	PORT->Group[0].PMUX[5].reg |= 0x20;                   /* Selecting peripheral function C */
	
	/* APBCMASK */
	//PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;			  /* SERCOM 0 enable*/
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM0;

	/*GCLK configuration for sercom0 module: using generic clock generator 0, ID for sercom0, enable GCLK*/

	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID(SERCOM0_GCLK_ID_CORE) |
	GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN(0);

	
	/* configure SERCOM0 module for UART as Standard Frame, 8 Bit size, No parity, BAUDRATE:9600*/

	SERCOM0->USART.CTRLA.reg =
	SERCOM_USART_CTRLA_DORD | SERCOM_USART_CTRLA_MODE_USART_INT_CLK |
	SERCOM_USART_CTRLA_RXPO(3/*PAD3*/) | SERCOM_USART_CTRLA_TXPO(1/*PAD2*/);
	
	uint64_t br = (uint64_t)65536 * (8000000 - 16 * 9600) / 8000000;
	
	SERCOM0->USART.CTRLB.reg = SERCOM_USART_CTRLB_RXEN | SERCOM_USART_CTRLB_TXEN | SERCOM_USART_CTRLB_CHSIZE(0/*8 bits*/);

	SERCOM0->USART.BAUD.reg = (uint16_t)br;

	SERCOM0->USART.CTRLA.reg |= SERCOM_USART_CTRLA_ENABLE;
}

int main(void)
{
	RTCtrl send,receive;
	RTCtrl* ptr;
	ptr = &receive;
    SystemInit();
	UARTInit();
	SYSCTRL->OSC8M.bit.PRESC = 0;
	i2cInit();
	ptr = &send;
	ptr->seconds.reg = 21;
	ptr->minutes.reg = 42;
	ptr->hours.reg   = 34;
	ptr->day.reg     = 5;
	ptr->date.reg    = 22;
	ptr->month.reg   = 8;
	ptr->year.reg    = 33;
	
	sendI2CDataArray(SLAVE_ADDR,ptr,BUF_SIZE);
	ptr = &receive;
	while(1){
	receiveI2CDataArray(SLAVE_ADDR,SECONDS_ADDR,ptr,BUF_SIZE);
	myprintf("%d%d:%d%d:%d%d ",ptr->hours.bits.tenHour,ptr->hours.bits.hours,ptr->minutes.bits.tenMinutes,ptr->minutes.bits.minutes,ptr->seconds.bits.tenSeconds,ptr->seconds.bits.seconds);
	switch(ptr->day.bits.day){
		case 1:
		myprintf("Monday");
		break;
		case 2:
		myprintf("Tuesday");
		break;
		case 3:
		myprintf("Wednesday");
		break;
		case 4:
		myprintf("Thursday");
		break;
		case 5:
		myprintf("Friday");
		break;
		case 6:
		myprintf("Saturday");
		break;
		case 7:
		myprintf("Sunday");
		break;
	}
	myprintf(" %d%d/%d%d/%d%d\n",ptr->date.bits.tenDate,ptr->date.bits.date,ptr->month.bits.tenMonth,ptr->month.bits.month,ptr->year.bits.tenYear,ptr->year.bits.year);

	}
	// Turn off the I2C
	SERCOM3->I2CM.CTRLA.bit.ENABLE = 0x0u;
}