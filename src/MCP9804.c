/* ************************************************************************/
/* Sourcefile:      MCP9804.c   										  */
/*																		  */
/* Function:        Application function declaration of MicroChip's 	  */
/*					MCP9804 Temperature sensor                            */
/*                                                                        */
/* Documentation: 	http://www.farnell.com/datasheets/1669501.pdf         */
/*                			                                              */
/* ************************************************************************/


#include "MCP9804.h"
#include "i2c.h"

/*================================[Function definitions]==============================================================*/


double MCP904_ReadTemp()
{
	uint8_t UpperByte;
	uint8_t LowerByte;
	double TempAmbient;

	I2C_Start(I2C1, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter); // Start a transmission in Master transmitter mode
	I2C_Write(I2C1, 0x05);										  // Write CONFIG register

	I2C_ReStart(I2C1, SLAVE_ADDRESS<<1, I2C_Direction_Receiver);   // ReStart a transmission in Master receiver mode

	UpperByte = I2C_ReadAck(I2C1);
	LowerByte = I2C_ReadNack(I2C1);

	I2C_Stop(I2C1);

	//First Check flag bits
	/*if ((UpperByte & 0x80) == 0x80)//TA >= TCRIT
	{
	}
	if ((UpperByte & 0x40) == 0x40) //TA > TUPPER
	{
	}
	if ((UpperByte & 0x20) == 0x20) //TA < TLOWER
	{
	}
	*/

	UpperByte = UpperByte & 0x1F; //Clear flag bits -> Upper 3 bits

	if ((UpperByte & 0x10) == 0x10) //IF 4. Bit(SIGN) is 1 => TA < 0°C
	{
		UpperByte = UpperByte & 0x0F; //Clear SIGN
		TempAmbient = 256 - (UpperByte * 16 + LowerByte / 16);
	}
	else //4. Bit is 0 => TA >= 0°C
	{
		TempAmbient = (UpperByte * 16 + LowerByte / 16); //Temperature = Ambient Temperature (°C)
	}

	return TempAmbient;
}

void MCP9804_SetTempResolution(MCP9804_TempRes tempRes)
{
	I2C_Start(I2C1, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
	I2C_Write(I2C1, 0x08);

	//TempRes register
	/*Resolution Bits
	* 00 = +0.5 °C
	* 01 = +0.25 °C
	* 10 = +0.125 °C
	* 11 = +0.0625 °C
	*/
	I2C_Write(I2C1, (0b000000) | tempRes);

	I2C_Stop(I2C1);
}

void MCP9804_ShutDown()
{
	I2C_Start(I2C1, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
	I2C_Write(I2C1, 0x01);								//CONFIG register

	I2C_Write(I2C1, (0b00000000 | Shutdown) );			//write data MSB (first 8 bit
	I2C_Write(I2C1, 0x00);								//write data LSB (last 8 bit)

	I2C_Stop(I2C1);
}

void MCP9804_WakeUp()
{
	I2C_Start(I2C1, SLAVE_ADDRESS<<1, I2C_Direction_Transmitter);
	I2C_Write(I2C1, 0x01);								//CONFIG register

	uint8_t data = (0b00000000 | Continue);

	I2C_Write(I2C1, data);								//write data MSB (first 8 bit)
	I2C_Write(I2C1, 0x00);								//write data LSB (last 8 bit)

	I2C_Stop(I2C1);
}





