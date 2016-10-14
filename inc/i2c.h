/*
 * i2c.h
 *
 *  Created on: 2015 szept. 13
 *      Author: kecsger12
 */

#ifndef I2C_H_
#define I2C_H_


/*================== [Includes] ======================================================================================*/

#include "stm32l1xx.h"

/*================================[Macros]============================================================================*/

#define 	SLAVE ADDRESS 0x18

/*================================[Type definitions]==================================================================*/

void 		I2C_init();
void 		I2C_Start(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void 		I2C_ReStart(I2C_TypeDef* I2Cx, uint8_t address, uint8_t direction);
void 		I2C_Write(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t 	I2C_ReadAck(I2C_TypeDef* I2Cx);
uint8_t 	I2C_ReadNack(I2C_TypeDef* I2Cx);
void 		I2C_Stop(I2C_TypeDef* I2Cx);

/*================================[Prototype function declarations]===================================================*/



/*================================[External constants]================================================================*/

/*================================[Internal constants]================================================================*/

#endif /* I2C_H_ */
