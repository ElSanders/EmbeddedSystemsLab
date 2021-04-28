/*
 * TarjetaSD.cpp
 *
 * Created: 4/27/2021 8:19:55 PM
 * Author : L03068681
 */ 


#include "sam.h"
#include "myprintf.h"
#include "spi.h"

#define RXBUFSIZE 0x400
#define LENGTH_R1 0x03
#define LENGTH_R7 0x07

void initCycles(void);

#define SIZE_SD_CMD 0x06
#define kCMD00 0x40
#define kCMD08 0x48
#define kCMD55 0x77
#define kCMD41 0x69

const uint8_t CMD00[SIZE_SD_CMD]  ={0x40, 0x00, 0x00, 0x00, 0x00, 0x95};
const uint8_t CMD08[SIZE_SD_CMD]  ={0x24, 0x00, 0x00, 0x00, 0xD5, 0x07};
uint8_t CMD17[SIZE_SD_CMD]  ={0x51, 0x00, 0x00, 0x00, 0x00, 0x01};
uint8_t CMD172[SIZE_SD_CMD]  ={0x51, 0x00, 0x00, 0x08, 0x00, 0x01};
const uint8_t CMD18[SIZE_SD_CMD]  ={0x52, 0x00, 0x00, 0x00, 0x00, 0x01};
const uint8_t CMD55[SIZE_SD_CMD]  ={0x77, 0x00, 0x00, 0x00, 0x00, 0x65};
const uint8_t CMD41[SIZE_SD_CMD] = {0x69, 0x40, 0x00, 0x00, 0x00, 0x77};

uint8_t RxBuffer[RXBUFSIZE];

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

void initCycles(void){
	uint32_t i;
	REG_PORT_OUTSET0 = PORT_PA18;
	for(i=0;i<76;i++)
	spiSend(0xFF);
}

//ADD spiXchg FUNCTION TO THE "spi.c" FILE

uint32_t spiXchg(const uint8_t * send_buff, uint32_t bc, uint8_t * receive_buff ) {
	uint8_t temp = 0xFF;
	uint32_t i;
	uint8_t temp_cmd = send_buff[0];
	
	REG_PORT_OUTCLR0 = PORT_PA18;
	for(i=0; i< bc; i++) {
		temp = spiSend(*(send_buff++));
		myprintf(" %x", temp);
	}
	switch(temp_cmd) {
		case kCMD00 :
		for(i=0; i<LENGTH_R1; i++) {
			temp = spiSend(0xFF);
			myprintf(" %x", temp);
		}
		break;
		case kCMD08 :
		for(i=0; i<LENGTH_R7; i++) {
			temp = spiSend(0xFF);
			myprintf(" %x", temp);
		}
		break;
		case kCMD41 :
		for(i=0; i<LENGTH_R1-1; i++) {
			temp = spiSend(0xFF);
			myprintf(" %x", temp);
		}
		spiSend(0xFF);
		break;
		case kCMD55 :
		for(i=0; i<LENGTH_R1; i++) {
			temp = spiSend(0xFF);
			//myprintf(" %x", temp);
		}
		break;
		default :
		myprintf("\n Error in CMD");
	}
	REG_PORT_OUTSET0 = PORT_PA18;
	return(temp);
}

void rcvr_datablock(const uint8_t * send_buff, uint32_t lba, uint8_t * receive_buff, uint32_t bs ) {
	uint8_t temp = 0xFF;
	uint32_t i;
	
	REG_PORT_OUTCLR0 = PORT_PA18;
	myprintf("\n\n");

	temp = send_buff[0];
	temp = spiSend(temp);
	myprintf(" %x", temp);
	temp = ((uint8_t*)&lba)[3];
	temp = spiSend(temp);
	myprintf(" %x", temp);

	// Complete the code that is missing

	temp = send_buff[5];
	temp = spiSend(temp);
	myprintf(" %x", temp);

	// Reading to find the beginning of the sector

	temp = spiSend(0xFF);
	while(temp != 0xFE){
		temp = spiSend(0xFF);
		myprintf(" %x", temp);
	}
	
	// Receiving the memory sector/block
	
	myprintf("\n\n");
	for(i=0; i< bs; i++) {
		while(SERCOM1->SPI.INTFLAG.bit.DRE == 0);
		SERCOM1->SPI.DATA.reg = 0xFF;
		while(SERCOM1->SPI.INTFLAG.bit.TXC == 0);
		while(SERCOM1->SPI.INTFLAG.bit.RXC == 0);
		temp = SERCOM1->SPI.DATA.reg;
		*(receive_buff++) = temp;
		myprintf(" %x", temp);
	}
	REG_PORT_OUTSET0 = PORT_PA18;
	myprintf("\n\n");
}

void spiInit( void ) {
	/* Switch to 8MHz clock (disable prescaler) */
	SYSCTRL->OSC8M.bit.PRESC = 0;

	PM->APBCMASK.bit.SERCOM1_ = 1; //enable the clock for SERCOM1 peripheral
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_SERCOM1_CORE;
	while( GCLK->STATUS.bit.SYNCBUSY ) { }
	
	//structures to configure the SERCOM1 peripheral
	const SERCOM_SPI_CTRLA_Type ctrla = {
		.bit.DORD = 0, // MSB first
		.bit.CPHA = 0, // Mode 0
		.bit.CPOL = 0,
		.bit.FORM = 0x0, // SPI frame
		.bit.DIPO = 0x3, // MISO on PAD[3]
		.bit.DOPO = 0x0, // MOSI on PAD[0], SCK on PAD[1], SS_ on PAD[2]
		.bit.MODE = 0x3  // Master Mode
	};
	SERCOM1->SPI.CTRLA.reg = ctrla.reg;
	const SERCOM_SPI_CTRLB_Type ctrlb = {
		.bit.RXEN = 1,   // RX enabled
		.bit.MSSEN = 1,  // Manual SC
		.bit.CHSIZE = 0x0  // 8-bit
	};
	SERCOM1->SPI.CTRLB.reg = ctrlb.reg;

	//Formula to configure the desired baudrate
	uint32_t br = ( double )( 8000000 / ( 2 * BAUDRATE ) ) - 1 ;
	SERCOM1->SPI.BAUD.reg = SERCOM_SPI_BAUD_BAUD( ( uint8_t )br );

	//structure to configure multiple PINs
	const PORT_WRCONFIG_Type wrconfig = {
		.bit.WRPINCFG = 1,
		.bit.WRPMUX = 1,
		.bit.PMUX = MUX_PA16C_SERCOM1_PAD0 | MUX_PA17C_SERCOM1_PAD1 | MUX_PA19C_SERCOM1_PAD3,
		.bit.PMUXEN = 1,
		.bit.HWSEL = 1,
		.bit.PINMASK = ( uint16_t )( ( PORT_PA16 | PORT_PA17 | PORT_PA19 ) >> 16 )
	};
	PORT->Group[0].WRCONFIG.reg = wrconfig.reg;

	//SS/CS (Slave Select/Chip Select) PIN 18 configuration
	REG_PORT_DIRSET0 = PORT_PA18;
	REG_PORT_OUTSET0 = PORT_PA18;
	
	//enable the SPI
	SERCOM1->SPI.CTRLA.bit.ENABLE = 1;
	while( SERCOM1->SPI.SYNCBUSY.bit.ENABLE ) { }
}

uint8_t spiSend( uint8_t data ) {
	uint8_t temp;
	while( SERCOM1->SPI.STATUS.bit.BUFOVF) { } //wait until buffer is empty
	SERCOM1->SPI.DATA.bit.DATA = SERCOM_SPI_DATA_DATA( data ); //transmit data
	while( !SERCOM1->SPI.INTFLAG.bit.RXC ) { } //wait until a data is received
	temp = SERCOM1->SPI.DATA.bit.DATA; //read data
	while( !SERCOM1->SPI.INTFLAG.bit.TXC ) { } //wait until there is no data to transmit
	myprintf( " %x", temp ); //printf the value in putty
	return temp;
}

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();

	UARTInit();
	spiInit();

	initCycles();
	//myprintf("\n");
	
	spiXchg( CMD00, SIZE_SD_CMD, RxBuffer );  /* reset instruction */
	spiXchg( CMD08, SIZE_SD_CMD, RxBuffer );
	spiXchg( CMD55, SIZE_SD_CMD, RxBuffer );
	spiXchg( CMD41, SIZE_SD_CMD, RxBuffer );
	myprintf("\n");

	//Write your code here
	volatile uint8_t rData;
	volatile uint8_t sData = 85;
	while (1) {
	REG_PORT_OUTCLR0 = PORT_PA18; //initiate transaction by SS_low
	rData = spiSend( sData );
	REG_PORT_OUTSET0 = PORT_PA18; //finish transaction by SS_high
	myprintf("%d",rData);
	}
	myprintf("\nDone");
}


