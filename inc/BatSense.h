/*
 * BatSense.h
 *
 *  Created on: 2015 szept. 13
 *      Author: kecsger12
 */

#ifndef BATSENSE_H_
#define BATSENSE_H_

#include "stm32l1xx.h"

// BatSense circuit hardware relevant definitions
//=================================================
/* The internal VREF value is individually measured in production and stored in dedicated EEPROM bytes */

#define 	VREF_MIN				1.202 						// [V]
#define 	VREF_MAX				1.242 						// [V]
#define 	VREF_TYP				1.224 						// [V]

#define 	VIN						3.26						// TODO: 3.27
#define 	R_12					1							// TODO: ??
#define 	R_10					1							// TODO: ??
#define 	VOLT_DIV_CONST			(R_12/ (R_12 + R_10))		//


#define		BATTERY_CAP_100			1.50						// 1.50V;  theoretical MAX voltage charge value of 1 cell
#define		BATTERY_CAP_75			1.35						// 1.35V; theoretical 75% voltage charge value of 1 cell
#define		BATTERY_CAP_50			1.20						// 1.20V; theoretical 50% voltage charge value of 1 cell
#define		BATTERY_CAP_25			1.05						// 1.05V; theoretical 25% voltage charge value of 1 cell
#define		BATTERY_CAP_0			0.9							// 0.9V; theoretical MIN voltage charge value of 1 cell

#define 	NUMBER_OF_BAT_CELL		3							// Right now I'm using 3 battery cells (3 x AAA NiMH 2800mAh)


#define 	ADC_RES					( 1 << 12 ) 				// 12-bit ADC => 4096
#define		LSB						( VREF_TYP/ADC_RES )		// LSB-> least value for the ADC to distinguish from
//=================================================

// MISC
//=================================================
#define 	BatSens_ADC_PIN 		GPIO_Pin_1
#define 	BatSens_EN_PIN 			GPIO_Pin_2

//=================================================


/*================================[Type definitions]==================================================================*/

/*================================[Prototype function declarations]===================================================*/
int8_t 		Measure_BatteryPercent();
double 		Measure_BatteryVoltage();
uint16_t 	Read_ADCValue();

void 		BatSens_Enable();
void 		BatSens_Disable();

void 		BatSens_Init();
void 		BatSens_GPIO_Init();
void 		BatSens_ADC_Init();
/*================================[External constants]================================================================*/


/*================================[Internal constants]================================================================*/


/*================================[Function definitions]==============================================================*/



#endif /* BATSENSE_H_ */
