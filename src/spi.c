/* **************************************************************************/
/* Sourcefile:      spi.c       									  	  	*/
/*																		  	*/
/* Function:        Application function declaration of the SPI 	  		*/
/*					Communication Stack                            			*/
/*                                                                       	*/
/* Documentation: 	       													*/
/*                			                                              	*/
/* **************************************************************************/

#include "spi.h"


/*================================ [Function definitions] ==============================================================*/

void SPI_init()
{

	GPIO_InitTypeDef GPIO_InitStruct;
	SPI_InitTypeDef SPI_InitStruct;

	// enable clock for used IO pins
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);


	/* configure pins used by SPI2
	 * P13 = SCK
	 * P14 = MISO
	 * P15 = MOSI
	 */

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/* PB12 = #SS (Slave Select -> Negated!) */

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_40MHz;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;

	/* Initialize GPIOA pins */
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	// connect SPI2 pins to SPI alternate function
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);

	// TODO
	/* #SS pin to HIGH ?? */
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	/*
	* configure SPI2 in Mode 0
	 * CPOL = 0 --> clock is low when idle
	 * CPHA = 0 --> data is sampled at the first edge
	 */

	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 			// set to full duplex mode, separate MOSI and MISO lines
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;     								// transmit in master mode, NSS pin has to be always high
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b; 								// one packet of data is 8 bits wide
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;        								// clock is low when idle
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;      								// data sampled at first edge
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft | SPI_NSSInternalSoft_Set; 			// set the NSS management to internal and pull internal NSS high
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; 			// SPI frequency is APB2 frequency / 4
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;								// data is transmitted MSB first

	SPI_Init(SPI2, &SPI_InitStruct);

	SPI_Cmd(SPI2, ENABLE); 														// enable SPI2

}

uint16_t SPI_send_data( uint16_t data)
{

	uint16_t recievedData;
	// #SS High
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);											// Set #SS to low

	/*
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, adress);
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI2);
	*/

	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI2, data);
	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE));
	recievedData = SPI_I2S_ReceiveData(SPI2);


	// #SS Low
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	return recievedData;

}


void send_data(uint16_t data)
{

    GPIO_ResetBits(GPIOA, GPIO_Pin_12); 							// CS low
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //wait buffer empty
    SPI_SendData(SPI2, data);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET); 	//wait finish sending
    GPIO_SetBits(GPIOA, GPIO_Pin_12); 								// CS high
}

uint16_t recieve_data()
{
	uint16_t recievedData;

	// wait for transfer to finish
	while ( SPI_I2S_GetFlagStatus( SPI2, SPI_I2S_FLAG_RXNE ) == RESET );
	recievedData = SPI_I2S_ReceiveData(SPI2);

	return recievedData;
}


/* The following code demonstrates master sends 1 byte command to slave and reads 1 byte back. */
uint16_t SPI_send_recieve_data(uint16_t cmd)
{
    uint16_t result;

    GPIO_ResetBits(GPIOA, GPIO_Pin_12); 							// CS low

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET); //wait buffer empty
    SPI_SendData8(SPI2, cmd);
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_BSY) == SET); 	//wait finish sending



    // Read receiving FIFO until it is empty
    /*TODO: uncomment 2 lines below
     *
    while (SPI_GetReceptionFIFOStatus(SPI2) != SPI_ReceptionFIFOStatus_Empty)
        SPI_ReceiveData8(SPI2);
	*/

    SPI_BiDirectionalLineConfig(SPI2, SPI_Direction_Rx);
    while (!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) ; 	// wait data received

    GPIO_SetBits(GPIOA, GPIO_Pin_4); 									// CS high
    SPI2->CR1 |= SPI_Direction_Tx;  									// Set Tx mode to stop Rx clock

    result = SPI_ReceiveData(SPI2);
    return result;
}
