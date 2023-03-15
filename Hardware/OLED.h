#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
/****
The driver IC SSD1306Z;
Segment	:	128
Com		:	64
MCU	Bus	Interface	Pin	Selection
SSD1306Pinname	I2C	6800-parallel	8080-parallel 4-wire seiral	3-wire serial	
BS0				0		0				0				0				1
BS1				1		0				1				0				0
BS2				0		1				1				0				0

I2C hardware interface : 
D0: SCl;
D1:	SDA(in);
D2:	SDA(out);
Note: Both the SDA and SCL must be connected the pull-up resistor;

Slave Address bit (SA0):
Pin D/C acts as SA0;
b7	b6	b5	b4	b3	b2	b1		b0
0	1	1	1	1	0	SA0		R(1)/W#(0)

OLED display clock :DCLK = Fosc / D ;D: define D5h from1~16;
the Frame Freq of display Ffrm = Fosc/(D*K*No. of Mux)
K:
No. of Mux : A8h; the power on reset value is 63;
Fosc :



*****/
// OLED parameter

#define OLEDI2CAddrRead		0x79    //SA0  == 0;
#define OLEDI2CAddrWrite	0x78    //SA0  == 0;
//#define OLEDI2CAddrRead	0x7B	//SA0 == 1;
//#define OLEDI2CAddrWrite	0x7A	//SA0 == 1;

#define OLEDI2CModeCommand		0x00	//CommandMode
#define OLEDI2CModeData			0x40	//DataMode

#define MaxRepeatRWTimes			3
#define TransferFault				1






//Fundamental Commamd

#define OLEDCommandContrastSet		0x81
#define OLEDParaContrastSet			0x7F	//	Adjustable Para,0x00~0xFF;

#define OLEDCommandDisplayWithRAM	0xA4
#define	OLEDCommandDisplayAll		0xA5

#define OLEDCommandDisplayNormal	0xA6
#define OLEDCommandDisplayInverse	0xA7

#define OLEDCommandDisplayOn		0xAF
#define OLEDCommmanDisplayOff		0xAE

//Scrolling Command
#define OLEDCommandHorizontalScrollRight		0x26
#define OLEDCommandHorizontalScrollLeft			0x27

#define OLEDParaHorizontalScroll1Dummy			0x00
#define OLEDParaHorizontalScroll2StartPage		0x00	//Adjustable para,0x00~0x07
#define OLEDParaHorizontalScroll3IntervalTime	0x00	//Adjustable para,0x00~0x07;
	/****
	0x00: 5 frame 
	0x01: 64 frame
	0x02: 128 frame
	0x03: 256 frame
	0x04: 3 frame
	0x05: 4 frame
	0x06: 25 frame
	0x07: 2 frame
	*****/
#define OLEDParaHorizontalScroll4Endpage		0x07	//Adjustable para, 0x00~0x07;


#define OLEDCommandVerticalHorizontalScrollRight		0x29
#define OLEDCommandVerticalHorizontalScrollLeft			0x2A
#define OLEDParaVerticalHorizontalScroll1Dummy			0x00
#define OLEDParaVerticalHorizontalScroll2StartPage		0x00	//Adjustable para,0x00~0x07
#define OLEDParaVerticalHorizontalScroll3IntervalTime	0x00	//Adjustable para,0x00~0x07;
	/****
	0x00: 5 frame
	0x01: 64 frame
	0x02: 128 frame
	0x03: 256 frame
	0x04: 3 frame
	0x05: 4 frame
	0x06: 25 frame
	0x07: 2 frame
	*****/
#define OLEDParaVerticalHorizontalScroll4Endpage		0x07	//Adjustable para, 0x00~0x07;
#define OLEDParaVerticalHorizontalScroll5offset			0x01	//Adjustable para,	0x01~0x3F;
	

#define OLEDCommandScrollDeactive		0x2E
#define OLEDCommandScrollActive			0x2F

#define OLEDCommandVerticalScrollArea	0xA3
#define OLEDParaVerticalScrollArea1TopFixrows	0x00		//Adjustable
#define OLEDparaVerticalScrollArea2				0x3F		//Adjustable


// Addressing Setting Command

#define OLEDCommandPageAddrStartAddrLow4bit			0x00		//Adjustable	0x00~0x0f
#define OLEDCommandPageAddrStartAddrHigherh4bit		0x10		//Adjustable	0x10~0x1f
//This command is only for page addressing mode


#define OLEDCommandMemoryAddrModeSet		0x20
#define OLEDParaMemoryAddr1ModeHorizontal	0x00
#define OLEDParaMemoryAddr1Vertical			0x01
#define OLEDParaMemoryAddr1PageMode			0x02


#define	OLEDCommandColumnAddrSet		0x21
#define	OLEDParaColumnAddr1Start		0x00		//Adjustable 0x00~0x7F, < EndAddr;
#define OLEDParaColumnAddr2End			0x7F		//Adjustable 0x00~0x7F, > StartAddr;
// This command is only for horizontal or vertical addressing mode.

#define	OLEDCommandPageAddrSet		0x22
#define	OLEDParaPageAddr1Start		0x00		//Adjustable 0x00~0x07, < EndPage;
#define OLEDParaPageAddr2End		0x07		//Adjustable 0x00~0x07, > StartPage;
// This command is only for horizontal or vertical addressing mode.

#define OLEDCommandPageAddrStartPage	0xB0	//Adjustable 0xB0~0XB7
// This command is only for page addressing mode



// Hardware	Configura
#define OLEDCommandHardwareStartLine		0x40	//Adjustable 0x40~0x7F, total:64 line;


#define	OLEDCommandHardwareReMapNormal		0xA0	// 
#define	OLEDCommandHardwareReMapInverse		0xA1		

#define OLEDCommandHardwareMultiplexRatioSet			0xA8
#define	OLEDParaHardwareMulitplexRatio				0x3F	//Adjustable (0x0F~0x3F)+1, for16 to 64,0to14 invalid entry;

#define OLEDCommandHardwareComPortScanDirectionAdd		0xC0	//from 0 to N-1,
#define OLEDCommandHardwareComPortScanDirectionDec		0xC8	//From N-1 to 0,  N: Multiplex Ratio

#define OLEDCommandHardwareDisplayOffset	0xD3	
#define OLEDParaHardwareDisplayOffset		0x00			//Adjustable 0x00~0x3F

#define OLEDCommandHardwareComPortConf		0xDA			
/*****
00A[4]A[5],0010
A[4]=0b, Sequential COM pin configuration
A[4]=1b(RESET), Alternative COM pin configuration
A[5]=0b(RESET), Disable COM Left/Right remap
A[5]=1b, Enable COM Left/Right remap
*****/


//Time and Driving Command

#define OLEDCommandClockDivideSet	0xD5
#define OLEDParaClockDivideset		0x80
/****
A[3:0] : Define the divide ratio (D) of the
display clocks (DCLK):
Divide ratio= A[3:0] + 1, RESET is
0000b (divide ratio = 1)
A[7:4] : Set the Oscillator Frequency, FOSC.
Oscillator Frequency increases with
the value of A[7:4] and vice versa.
RESET is 1000b
Range:0000b~1111b
Frequency increases as setting value
increases.
****/	
#define OLEDCommandPreChargePeriodSet	0xD9
#define OLEDParaPreChargePeriodSet		0x22
/****
A[3:0] : Phase 1 period of up to 15 DCLK clocks 0 is invalid entry
(RESET=2h)
A[7:4] : Phase 2 period of up to 15 DCLK clocks 0 is invalid entry
(RESET=2h )
***/

#define OLEDCommandComDeselectVoltageLevelSet		0xDB
#define OLEDParaComDeselectVoltageLevelSetL		0x00	//0.65Vcc
#define OLEDParaComDeselectVoltageLevelSetM		0x20	//0.77Vcc(default)
#define OLEDParaComDeselectVoltageLevelSetH		0x30	//0.83Vcc
/*****
****/

#define OLEDCommandChargePumpSet		0x8D
#define OLEDParaChargePumpEnable		0x14
#define OLEDParaChargePumpDisable		0x10



#define OLEDCommadNop		0xE3

void OLED_Clear(void);
void OLED_ShowChar(uint8_t Row, uint8_t Col, char Char);
void OLED_ShowString(uint8_t Row, uint8_t	Col, char *String);

void OLED_ShowUnsignNum(uint8_t Row, uint8_t Col, uint32_t Num, uint8_t Length);


void OLED_Init(void);






#endif

