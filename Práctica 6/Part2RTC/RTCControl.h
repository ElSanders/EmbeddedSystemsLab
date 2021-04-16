#ifndef RTCCONTROL_H_               /*this goes at the top of the document*/
#define RTCCONTROL_H_
#define SECONDS_ADDR 0x00
#define MINUTES_ADDR 0x01
#define HOURS_ADDR 0x02
#define DAY_ADDR 0x03
#define DATE_ADDR 0x04
#define MONTH_ADDR 0x05
#define YEAR_ADDR 0x06

typedef union {
	struct {
		uint8_t seconds:4,
				tenSeconds:3,
				unused:1;
	}bits;
	uint8_t reg;
}seconds_type;

typedef union {
	struct {
		uint8_t minutes:4,
				tenMinutes:3,
				unused:1;
	}bits;
	uint8_t reg;
}minute_type;

typedef union {
	struct {
		uint8_t hours:4,
				tenHour:1,
				tenPM:1,
				form:1,
				unused:1;
	}bits;
	uint8_t reg;
}hour_type;

typedef union {
	struct {
		uint8_t day:3,
				unused:5;
	}bits;
	uint8_t reg;
}day_type;

typedef union {
	struct {
		uint8_t date:4,
				tenDate:2,
				unused:2;
	}bits;
	uint8_t reg;
}date_type;

typedef union {
    struct {
        uint8_t month  	:4,
         	    tenMonth:1,
                unused:3;
    }bits;
    uint8_t reg;
} month_type;

typedef union {
	struct {
		uint8_t year:4,
				tenYear:4;
	}bits;
	uint8_t reg;
}year_type;

typedef union {
	struct {
		uint8_t RS0:1,
				RS1:1,
				unused1:2,
				SQWE: 1,
				unused2:2,
				OUT:1;
	};
	uint8_t reg;
}ctrl_type;

typedef union {
	seconds_type seconds;
	minute_type minutes;
	hour_type hours;
	day_type day;
	date_type date;
	month_type month;
	year_type year;
	ctrl_type control;
}RTCtrl;

void i2cInit(){
	/* port mux configuration */
	PORT->Group[0].PINCFG[PIN_PA22].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN | PORT_PINCFG_PULLEN; /* SDA */
	PORT->Group[0].PINCFG[PIN_PA23].reg = PORT_PINCFG_PMUXEN | PORT_PINCFG_INEN | PORT_PINCFG_PULLEN; /* SCL */
	
	/* PMUX: even = n/2, odd: (n-1)/2 */
	PORT->Group[0].PMUX[11].reg |= 0x02u;
	PORT->Group[0].PMUX[11].reg |= 0x20u;
	
	/* APBCMASK */
	PM->APBCMASK.reg |= PM_APBCMASK_SERCOM3;

	/*GCLK configuration for sercom3 module*/
	GCLK->CLKCTRL.reg = GCLK_CLKCTRL_ID (SERCOM3_GCLK_ID_CORE) |
	GCLK_CLKCTRL_ID (SERCOM3_GCLK_ID_SLOW) |
	GCLK_CLKCTRL_GEN(4) |
	GCLK_CLKCTRL_CLKEN;
	GCLK->GENCTRL.reg |= GCLK_GENCTRL_SRC_OSC8M|GCLK_GENCTRL_GENEN|GCLK_GENCTRL_ID(4);

	/* set configuration for SERCOM3 I2C module */
	SERCOM3->I2CM.CTRLB.reg = SERCOM_I2CM_CTRLB_SMEN; /* smart mode enable */
	while (SERCOM3->I2CM.SYNCBUSY.reg); // waiting loading
	
	/* calculate BAUDRATE */
	uint64_t tmp_baud =((8000000/100000)-10-(8000000*250 /1000000000))/2;
	SERCOM3->I2CM.BAUD.bit.BAUD = SERCOM_I2CM_BAUD_BAUD((uint32_t)tmp_baud);
	while (SERCOM3->I2CM.SYNCBUSY.reg); // waiting loading
	// value equals 0x22 or decimal 34
	
	SERCOM3->I2CM.CTRLA.reg = SERCOM_I2CM_CTRLA_ENABLE   |/* enable module */
	SERCOM_I2CM_CTRLA_MODE_I2C_MASTER |		/* i2c master mode */
	SERCOM_I2CM_CTRLA_SDAHOLD(3);		 /* SDA hold time to 600ns */
	while (SERCOM3->I2CM.SYNCBUSY.reg);  /* waiting loading */

	SERCOM3->I2CM.STATUS.reg |= SERCOM_I2CM_STATUS_BUSSTATE(1); /* set to idle state */
	while (SERCOM3->I2CM.SYNCBUSY.reg);  /* waiting loading */
}

void StopCond(){
  SERCOM3->I2CM.CTRLB.bit.CMD = 0x3;
}

void sendI2CDataArray(uint8_t SlAddr, RTCtrl *ptrData, int Size){ 
	SERCOM3->I2CM.ADDR.reg = (SlAddr << 1) | 0;    
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};		
	SERCOM3->I2CM.DATA.reg=ptrData->seconds.reg;
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};
	SERCOM3->I2CM.DATA.reg=ptrData->minutes.reg;
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};
	SERCOM3->I2CM.DATA.reg=ptrData->hours.reg;
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};		   
	SERCOM3->I2CM.DATA.reg=ptrData->day.reg;
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};
	SERCOM3->I2CM.DATA.reg=ptrData->date.reg;
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};
	SERCOM3->I2CM.DATA.reg=ptrData->month.reg;
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};		   
	SERCOM3->I2CM.DATA.reg=ptrData->year.reg;
	while(!SERCOM3->I2CM.INTFLAG.bit.MB){};
	StopCond();
}

void receiveI2CDataArray(uint8_t SlAddr, uint8_t ptr, RTCtrl *data, int Size){
  	SERCOM3->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
  	while(SERCOM3->I2CM.SYNCBUSY.reg);
	SERCOM3->I2CM.ADDR.reg = (SlAddr << 1) | 0; 
    while(SERCOM3->I2CM.INTFLAG.bit.MB ==0);
	SERCOM3->I2CM.DATA.reg = 0;
	while(SERCOM3->I2CM.INTFLAG.bit.MB ==0){};
	StopCond();
	SERCOM3->I2CM.ADDR.reg = (SlAddr<<1)|1;
	
	SERCOM3->I2CM.CTRLB.reg &= ~SERCOM_I2CM_CTRLB_ACKACT;
	
	data->seconds.reg = SERCOM3 ->I2CM.DATA.reg;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	data->minutes.reg = SERCOM3 ->I2CM.DATA.reg;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	data->hours.reg = SERCOM3 ->I2CM.DATA.reg;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	data->day.reg = SERCOM3 ->I2CM.DATA.reg;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	data->date.reg = SERCOM3 ->I2CM.DATA.reg;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	data->month.reg = SERCOM3 ->I2CM.DATA.reg;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	data->year.reg = SERCOM3 ->I2CM.DATA.reg;
	while(SERCOM3->I2CM.INTFLAG.bit.SB==0){};
	SERCOM3->I2CM.CTRLB.reg |= SERCOM_I2CM_CTRLB_ACKACT;
  	StopCond();
}
#endif /*RTCCONTROL_H_*/          /*this goes at the bottom of the document*/