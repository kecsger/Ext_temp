/*
 * MCP9804.h
 *
 *  Created on: 2015 szept. 13
 *      Author: kecsger12
 */

#ifndef MCP9804_H_
#define MCP9804_H_

/*================== [Includes] ======================================================================================*/

#include "stm32l1xx.h"

/*================================[Macros]============================================================================*/

#define 	SLAVE_ADDRESS 			0x18
#define 	I2C_READ 				0x01
#define 	I2C_WRITE 				0x00

/*================================[Type definitions]==================================================================*/

typedef enum {
	TempRes_05 = 0,
	TempRes_025,
	TempRes_0125,
	TempRes_00625
} MCP9804_TempRes;

typedef enum {
	Continue = 0,
	Shutdown
} Shdwn_mode;

/*================================[Prototype function declarations]===================================================*/

double	MCP904_ReadTemp();
void 	MCP9804_SetTempResolution(MCP9804_TempRes tempRes);
void 	MCP9804_ShutDown();
void 	MCP9804_WakeUp();

/*================================[External constants]================================================================*/

/*================================[Internal constants]================================================================*/




#endif /* MCP9804_H_ */
