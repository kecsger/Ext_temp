/* ************************************************************************/
/* Sourcefile:      MIDAS.c       									      */
/*																		  */
/* Function:        Application function declaration of MIDAS's 	      */
/*					MC20805BH6W LCD display                  			  */
/*                                                                        */
/* Documentation:   http://www.farnell.com/datasheets/1663604.pdf      	  */
/*                			                                              */
/* ************************************************************************/




#include "MIDAS.h"
#include "timer.h"

/*================================[Function definitions]==============================================================*/

//Initializes the 8-bit parallel interface to the MC20805BH6W
void LCD_init()
{
	MIDAS_GPIO_Init();

	RS(0);
    RW(0);

	//Tell the LCD we are using 8bit data communication
    Delay_ms(100);
	send_cmd(BIT_MODE_8);

	Delay_ms(100);
	send_cmd(BIT_MODE_8);

	Delay_ms(100);
	send_cmd(BIT_MODE_8);

	//Tell the LCD we are using 8bit data communication, 2 lines and 5x8 dot per pixels
	send_cmd(LCD_INIT_MODE);
	send_cmd(DISP_OFF);
	send_cmd(BLINKCUR_ON);

	// Entry Mode Set
	send_cmd(ENTRY_MODE_SET);

	//write special character to CGRAM for further use (battery status + 'é' + 'û')
	// TODO: uncomment the lne below
	// Write_CheckSpecialChar_to_CGRAM();

	send_cmd(CLR_DISP);
	send_cmd(CUR_HOME);
	send_cmd(BLINKCUR_ON);
}

/* Function for displaying LCD status */
void LCD_Write_Status(const char *status)
{
	// Status shall always be displayed at the beginning of the 1st line
	send_cmd(CUR_HOME);

	send_string(status);
}


/* Function for displaying the actual temperature */

void LCD_Write_Temp(double temp_act)
{
	// Set cursor to 2nd line (8 shifts) and Shift cursor 2nd position
	//	pl:
	//			--------
	//			--21,5°C
	//
	send_cmd(CUR_HOME);

	uint8_t i;

	for(i=0; i < LCD_SEG_NUM+2; i++)
		send_cmd(CUR_RIGHT);

	// TODO
	// double to string conversion !!!
	//send_string(temp_act);

	// Write '°C' at the end
	send_string("°C");

	send_cmd(CUR_HOME);
}


void LCD_WriteBat_percent(uint8_t percent)
{
    //a) Shift cursor to initial position
	send_cmd(CUR_HOME);

	//(b) ...and then start shifting it 8x
	int i;
	for (i = 0; i < LCD_SEG_NUM; i++)
		send_cmd(CUR_RIGHT);

    switch (percent)
    {
      case 100:
    	  Display_CGRAM(0);
        break;
      case 75:
    	  Display_CGRAM(1);
        break;
      case 50:
    	  Display_CGRAM(2);
        break;
      case 25:
    	  Display_CGRAM(3);
        break;
      case 0:
    	  Display_CGRAM(4);
        break;
      default:
    	  send_string("?");
        break;
	}

	send_cmd(CUR_HOME);
}

// Only for sending command (not DDRAM/CGRAM actions!!)
void send_cmd(unsigned char c)
{
	LCD_BusyCheck(); //wait if LCD is busy

	RW(0);
	RS(0);

	// 0x01 is for masking out the upper bits, because only the last 1 bit is important
	DB7(( c >> 7 ) && 0x01);
	DB6(( c >> 6 ) && 0x01);
	DB5(( c >> 5 ) && 0x01);
	DB4(( c >> 4 ) && 0x01);
	DB3(( c >> 3 ) && 0x01);
	DB2(( c >> 2 ) && 0x01);
	DB1(( c >> 1 ) && 0x01);
	DB0(c && 0x01);

	//Toggle the Enable Pin
	E(1);
	E(0);

}

void send_char(unsigned char c)
{
	LCD_BusyCheck(); //wait if LCD is busy

	RW(0);
	RS(1);

	DB7(( c >> 7 ) && 0x01);
	DB6(( c >> 6 ) && 0x01);
	DB5(( c >> 5 ) && 0x01);
	DB4(( c >> 4 ) && 0x01);
	DB3(( c >> 3 ) && 0x01);
	DB2(( c >> 2 ) && 0x01);
	DB1(( c >> 1 ) && 0x01);
	DB0(c && 0x01);

	//Toggle the Enable Pin
	E(1);
	E(0);

}

//Sends a given string to the LCD. Will start printing from
//current cursor position.
void send_string(const char *incoming_string)
{
    uint8_t i;

    for(i = 0 ; incoming_string[i] != '\0' ; i++)
	{
		if(incoming_string[i] == 'é')
		{
			Display_CGRAM(5);				// "é" is in CGRAM at bank position #5
		}

		else if(incoming_string[i] == 'û')
		{
			Display_CGRAM(6);				// "Û" is in CGRAM at bank position #6
		}

		else
			send_char(incoming_string[i]);
	}

}

void LCD_BusyCheck()
{
	RW(1);
	RS(0);

	uint8_t i = 1;

	// Check if D= bit is busy -> 1: busy

	while(i == 1)
	{
		E(1);
		i = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4); // Reading D7 value
		//i = ( (D7 >> 7) & 0x01 );
		/*if ((D7 & 0x80) == 0x00)
			break;
		*/
		/*if (i)
			break;
		*/
		E(0);

		E(1);E(0);

	}
}


/* Function for writing special character to CGRAM
 * CGRAM address is configured above       			*/
void Write_to_CGRAM(unsigned char *mychar)
{
	unsigned int i;

	for(i = 0 ; mychar[i] != '\0' ; i++)
        send_char(mychar[i]);
}

/* Display the given character from CGRAM */
void Display_CGRAM(uint8_t CGRAM_index)
{
	// Display symbol at the given CGRAM index
	send_char(0x00 | CGRAM_index);
}

void LCD_Test()
{
	LCD_init();

	const char *test_string = "TEST";
	send_string(test_string);

	send_cmd(CUR_HOME);
}

void MIDAS_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitDef;

	// Enable clock for GPOIA and GPIOB
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	// GPIOA Pins 3,4,5,6,7 and 10
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_10;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA, &GPIO_InitDef);

	// GPIOB Pins 0,1,2,10 and 11
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOB, &GPIO_InitDef);

}

/* Display contains 5x8 dots, I'm using only 4x7
*
*   For example:
*
*         xx    	---0 0110 => 0x06
*        x  x		---0 1001 => 0x09
*        x  x		---0 1001 => 0x09
*        x  x		---0 1001 => 0x09
*        xxxx		---0 1111 => 0x0F
*        xxxx		---0 1001 => 0x0F
*        xxxx		---0 1111 => 0x0F
*        ----
*/

void Write_CheckSpecialChar_to_CGRAM()
{


	unsigned char percent_100[7] = {0x06,0x0F,0x0F,0x0F,0x0F,0x0F,0x0F};
    unsigned char percent_75[7] =  {0x06,0x0F,0x09,0x0F,0x0F,0x0F,0x0F};
    unsigned char percent_50[7] =  {0x06,0x0F,0x09,0x09,0x0F,0x0F,0x0F};
    unsigned char percent_25[7] =  {0x06,0x0F,0x09,0x09,0x09,0x0F,0x0F};
    unsigned char percent_0[7] =   {0x06,0x0F,0x09,0x09,0x09,0x09,0x0F};


	unsigned char spec_char_e[7] = {0x02,0x04,0x0E,0x11,0x1F,0x10,0x0E}; 	// é
	unsigned char spec_char_u[7] = {0x0A,0x0A,0x11,0x11,0x11,0x13,0x0D};	// û

	/*int i;


	for(i=0; i < (LCD_SEG_NUM * CGRAM_BANK_DIST); i += CGRAM_BANK_DIST)
	{
		send_cmd(CGRAM_ADDR_0 + i);
		Write_to_CGRAM(spec_char_e);
	}
	*/

	send_cmd(CGRAM_ADDR_0);           	//Set CGRAM address
	Write_to_CGRAM(percent_100);

	send_cmd(CGRAM_ADDR_8);
	Write_to_CGRAM(percent_75);

	send_cmd(CGRAM_ADDR_16);
	Write_to_CGRAM(percent_50);

	send_cmd(CGRAM_ADDR_24);
	Write_to_CGRAM(percent_25);

	send_cmd(CGRAM_ADDR_32);
	Write_to_CGRAM(percent_0);

	send_cmd(CGRAM_ADDR_40);
	Write_to_CGRAM(spec_char_e);

	send_cmd(CGRAM_ADDR_48);
	Write_to_CGRAM(spec_char_u);

}





