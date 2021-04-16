#include "sam.h"
#include "my_samd21g18a.h"

#define PINCFG_CONFIG_VALUE 0b00000000
#define GCLK_BASE_ADDRESS 
int main (void) {
	
//Initialize the SAM system
	SystemInit();

//Port configuration for general purpose PIN
	PORT->Group[0].PINCFG[14].reg = 0b00000000;
	PORT->Group[0].DIRSET.reg = PORT_PA14;


//TIMER configuration   	
//Configure the POWER MANAGER to enable the TC3 module
   	PM->APBCMASK.reg |= 0x00000800; 
//Configure the GENERIC CLOCK CONTROL used by the TC3 modul
GCLK->GENDIV.reg =  0x00000000;	
GCLK->GENCTRL.reg = 0x00010600;	
GCLK->CLKCTRL.reg = 0x401B;

//Determine the initial count
//Time = Toscillator * (2^TIMERMODE - COUNT) * Prescaler
	
TC3->COUNT16.COUNT.reg = 65307;
//Configure the TC3 peripheral
TC3->COUNT16.CTRLA.reg = 0;
TC3->COUNT16.CTRLBSET.reg = 0;
TC3->COUNT16.INTFLAG.reg = 0x01;
//Enabling the TC3 peripheral
TC3->COUNT16.CTRLA.reg |=  0x02;
//Loop to sync the TC clock with the processor/main clock
	
while(!(TC3->COUNT16.STATUS.reg && 0x80)) {
}	

//Loop to generate the square-wave signal
//EXTRA cycles to ADD to the initial count---it will be still a 
//polling-error of +/- 7 us in the period of the signal
//LDRB(4) + ANDS(1) + BEQ(2) + LDRB(4) + ANDS(1) + BEQ(1) + STR(4) + 
//STRB(4) + STRH(4) + B(2) = 27 cycles

while(true){
if (!(TC3->COUNT16.STATUS.reg && 0x01)){
PORT->Group[0].OUTTGL.reg = 0x00004000; //toggle the output pin
//TC3->COUNT16.STATUS.reg = 0x01; // clear the OVF flag
TC3->COUNT16.COUNT.reg = 65307; //recharge the count
}

}