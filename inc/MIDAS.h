/*
 * MIDAS.h
 *
 *  Created on: 2015 szept. 13
 *      Author: kecsger12
 */

#ifndef MIDAS_H_
#define MIDAS_H_

/*================== [Includes] ======================================================================================*/

#include "stm32l1xx.h"

/*================================[Macros]============================================================================*/
#define 	LCD_SEG_NUM    	8
#define 	CGRAM_BANK_DIST	8 				// Distance from each CGRAM symbols

// LCD Hardware port definitions
//=================================================
#define 	DB0(val)		val==1 ? GPIO_SetBits(GPIOB, GPIO_Pin_1):GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define 	DB1(val)		val==1 ? GPIO_SetBits(GPIOB, GPIO_Pin_2):GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define 	DB2(val)		val==1 ? GPIO_SetBits(GPIOA, GPIO_Pin_7):GPIO_ResetBits(GPIOA, GPIO_Pin_7)
#define 	DB3(val)		val==1 ? GPIO_SetBits(GPIOB, GPIO_Pin_0):GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define 	DB4(val)		val==1 ? GPIO_SetBits(GPIOA, GPIO_Pin_5):GPIO_ResetBits(GPIOA, GPIO_Pin_5)
#define 	DB5(val)		val==1 ? GPIO_SetBits(GPIOA, GPIO_Pin_6):GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define 	DB6(val)		val==1 ? GPIO_SetBits(GPIOA, GPIO_Pin_3):GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define 	DB7(val)		val==1 ? GPIO_SetBits(GPIOA, GPIO_Pin_4):GPIO_ResetBits(GPIOA, GPIO_Pin_4)

#define 	RS(val)			val==1 ? GPIO_SetBits(GPIOA, GPIO_Pin_10):GPIO_ResetBits(GPIOA, GPIO_Pin_10)
#define 	RW(val)			val==1 ? GPIO_SetBits(GPIOB, GPIO_Pin_10):GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define 	E(val)			val==1 ? GPIO_SetBits(GPIOB, GPIO_Pin_11):GPIO_ResetBits(GPIOB, GPIO_Pin_11)
//=================================================

// LCD Commands definitions
//=================================================
#define     FALSE       	0
#define     TRUE        	1

#define     CLR_DISP        0b00000001 		//Clear display

#define     CUR_HOME        0b00000010    	//Move cursor home and clear screen memory
#define     CUR_RIGHT       0b00010100    	//Move cursor one to right
#define     CUR_LEFT        0b00010000    	//Move cursor one to left

#define     SCROLL_RIGHT    0b00011100    	//Scroll entire screen right one space
#define     SCROLL_LEFT     0b00011000    	//Scroll entire screen left one space

#define     DISP_ON         0b00001100    	//Turn visible LCD on
#define     DISP_OFF        0b00001000    	//Turn visible LCD off

#define     UNDERLINE_ON    0b00001110    	//Turn on underlined cursor
#define     UNDERLINE_OFF   0b00001100    	//Turn off underlined cursor

#define     BLINKCUR_ON     0b00001101    	//Turn on blinking box cursor
#define     BLINKCUR_OFF    0b00001100    	//Turn off blinking box cursor

#define     DUALCUR_ON      0b00001111    	//Turn on blinking box and underline cursor
#define     DUALCUR_OFF     0b00001100    	//Turn off blinking box and underline cursor

#define     SET_CURSOR      0b10000000    	//SET_CURSOR + X : Sets cursor position to X

#define     DD_RAM_ADDR     0b10000000 		//
#define     DD_RAM_ADDR2    0b11000000 		//

#define     CGRAM_ADDR_0   	0b01000000 		//
#define     CGRAM_ADDR_8   	0b01001000
#define     CGRAM_ADDR_16  	0b01010000
#define     CGRAM_ADDR_24  	0b01011000
#define     CGRAM_ADDR_32  	0b01100000
#define     CGRAM_ADDR_40  	0b01101000
#define     CGRAM_ADDR_48  	0b01110000
#define     CGRAM_ADDR_54  	0b01111000

#define     ENTRY_MODE_SET  0b00000110 		//
#define 	BIT_MODE_8		0b00110000		// 8-bit interface mode
#define 	LCD_INIT_MODE	0b00111000		/* bit4: 0-> 4-bit interface mode
											*		 1-> 8-bit interface mode
											*  bit3: 0-> 1-line mode
											*        1-> 2-line mode
											*  bit2: 0-> 5x8 dots
											*	 	 1-> 5x11 dots
											*  bit1 and bit0: DONT CARE
											*/

//=================================================


/*================================[Type definitions]==================================================================*/

/*================================[Prototype function declarations]===================================================*/

void 	LCD_init();												// Initializing the MIDAS display
void 	send_cmd(unsigned char c);								// MIDAS display configuration with internal commands (see documentation)
void 	send_char(unsigned char c);								// Writing characters to MIDAY display
void 	send_string(const char* incoming_string);				// Writing string to MIDAY display
void 	LCD_BusyCheck();										// Wait function until display is not busy anymore

void 	LCD_Test(); 											// Write "TEST" string to the display
void 	MIDAS_GPIO_Init();										// Initializing MIDAS GPIO ports

void 	LCD_Write_Status(const char *status);				// Function for displaying LCD status
void 	LCD_Write_Temp(double temp_act);						// Function for displaying the actual temperature
void 	LCD_WriteBat_percent(uint8_t percent); 					// 100% battery status

void 	Write_to_CGRAM(unsigned char *mychar);					// Writing special character to CGRAM
void 	Display_CGRAM(uint8_t CGRAM_index);						// Displaying the character from CGRAM

void 	Write_CheckSpecialChar_to_CGRAM();						// Check, if the sent character is special character (é,û,...)

/*================================[External constants]================================================================*/


/*================================[Internal constants]================================================================*/



#endif /* MIDAS_H_ */
