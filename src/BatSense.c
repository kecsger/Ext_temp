
/* ******************************************************************************************************/
/* Sourcefile:      BatSense.c   										  								*/
/*																		  								*/
/* Function:        Application function declaration of 	      		  								*/
/*					Battery charge estimation using  2 transistors		  								*/
/*                  (NPN and P-channel MOSFET) via GPIO ports		 	  								*/
/*                                                     					  								*/
/* Documentation:  http://www.microbuilder.eu/Tutorials/Fundamentals/MeasuringBatteryVoltage.aspx		*/
/*                			                                              						  		*/
/* Battery Characteristics:  http://tangentsoft.net/audio/pimeta2/bitmaps/nimh-9v-discharge.png			*/
/*                			                                              						  		*/
/*                			                                              						  		*/
/*                			                                              						  		*/
/* Ellenállás méretezés:               			                                              			*/
/*                			                                              						  		*/
/*     * Azt szeretném, hogy max töltöttség mellet az ADC kimenetén a maximális kódszám (12 bit = 4096)	*/
/*       jelenjen meg.          			                                              				*/
/*     * Ehhez visszafelé kell kiszámolnom az R_12 és R_10 méreteit (R_10 = 10k, könnyen számolható)    */
/*                			                                              						  		*/
/*     * ADC_code = Vout/LSB    																		*/
/* 	   * LSB = Vref / (2^12)   			                                              					*/
/*     * Vout = (R_12 /(R_12+R_10)) * Vin        			                                            */
/*                			                                              						  		*/
/*                			                                              						  		*/
/*     * Ez alapján Vout := Vref, így [R_12 /(R_12+R_10)] = [Vref/Vin]            			            */
/*     * Kiszámolva ezt:           			                                              				*/
/*                					R_12 = 4k		                                              		*/
/*                			        R_10 = 10k                                      					*/
/*                			                                              						  		*/
/*		*  CSAK akkor, ha Vin = 4,5V(3 elem max töltöttsége) és Vref = 1,224V							*/
/*                			                                              						  		*/
/* ******************************************************************************************************/






/*================== [Includes] ======================================================================================*/

#include "BatSense.h"
#include "MIDAS.h"

/*================================[Macros]============================================================================*/


void BatSens_Init()
{
	BatSens_GPIO_Init();
	BatSens_ADC_Init();
}

int8_t Measure_BatteryPercent()
{

	double VIN_Measured = (Measure_BatteryVoltage() / NUMBER_OF_BAT_CELL); // For 1 cell now

	if (VIN_Measured >= BATTERY_CAP_75)
		return 100;

	else if (VIN_Measured < BATTERY_CAP_75 && VIN_Measured >= BATTERY_CAP_50)
		return 75;

	else if (VIN_Measured < BATTERY_CAP_50 && VIN_Measured >= BATTERY_CAP_25)
		return 50;

	else if (VIN_Measured < BATTERY_CAP_25 && VIN_Measured > BATTERY_CAP_0)
		return 25;

	else if (VIN_Measured <= BATTERY_CAP_0)
		return 0;

	else
		send_string("BAT_ERR"); return -1;

}

/* Convert the read ADC code to Voltage */
// Reverse engineering from this site:
// http://www.microbuilder.eu/Tutorials/Fundamentals/MeasuringBatteryVoltage.aspx

double Measure_BatteryVoltage()
{
	BatSens_Enable(); 											// Open NPN transistor for measurement (BatSens_EN)

	// TODO: esetleg kicsi delay
	// Delay_ms(100);

	uint16_t ADC_code = Read_ADCValue();

	double VIN_Measured = -1;
	VIN_Measured = ((ADC_code * LSB) / VOLT_DIV_CONST);			// VIN for 3-cells !!!

	BatSens_Disable();											// Close NPN transistor for shutting off the flow of current to the ADCmeasurement (BatSens_EN)

	return VIN_Measured;
}

/* Read ADC value (code) */

uint16_t Read_ADCValue()
{
	ADC_SoftwareStartConv(ADC1);								//Start the conversion

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));				//Processing the conversion

	return ADC_GetConversionValue(ADC1); 						//Return the converted data
}


void BatSens_Enable()
{
	GPIO_SetBits(GPIOA, BatSens_EN_PIN);
}

void BatSens_Disable()
{
	GPIO_ResetBits(GPIOA, BatSens_EN_PIN);
}

void BatSens_GPIO_Init()
{
	GPIO_InitTypeDef GPIO_InitDef;

	// Enable clock for GPOIA and GPIOB

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	// GPIOA Pins 2 BatSense_EN
	GPIO_InitDef.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOA, &GPIO_InitDef);
}

void BatSens_ADC_Init()
{
	ADC_InitTypeDef ADC_InitStructure; 											// Structure for ADC configuration
	ADC_CommonInitTypeDef ADC_CommonInitStructure;								// Structure for ADC Common Init

	GPIO_InitTypeDef GPIO_InitStructre; 										// Structure for Analog input pin

	/* TODO: DMA használat eseleg ??? */


	/* Clock configuration */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 						// The ADC1 is connected the APB2 peripheral bus thus we will use its clock source
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 						// Clock for the ADC port!!

	/* Analog pin configuration */
	GPIO_InitStructre.GPIO_Pin = GPIO_Pin_1; 									// The channel 10 is connected to PC0
	GPIO_InitStructre.GPIO_Mode = GPIO_Mode_AN; 								// The PC0 pin is configured in analog mode
	GPIO_InitStructre.GPIO_PuPd = GPIO_PuPd_NOPULL; 							// We don't need any pull up or pull down
	//GPIO_InitStructre.GPIO_OType = GPIO_OType_PP;								// TODO: ???????
	//GPIO_InitStructre.GPIO_Speed = GPIO_Speed_50MHz;							// TODO: DONT KNOW ABOUT THIS ONE !!

	GPIO_Init(GPIOA, &GPIO_InitStructre); 										// Affecting the port with the initialization structure configuration

	/* ADC Common Init */
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	//ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	//ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;

	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC structure configuration */
	ADC_DeInit(ADC1);

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 						// Input voltage is converted into a 12bit number giving a maximum value of 4096
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 								// The scan is configured in one channel
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 							// Conversion is continuous, the input data is converted more than once
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // No trigger for conversion
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 						// Data converted will be shifted to right
	ADC_InitStructure.ADC_NbrOfConversion = 1; 									// I think this one is clear

	//ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1; 	// Conversion is synchronous with TIM1 and CC1 (actually I'm not sure about this one :/)

	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 								// The scan is configured in one channel

	ADC_Init(ADC1, &ADC_InitStructure); 										// Initialize ADC with the previous configuration

	/* Select the channel to be read from */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_96Cycles);

	/* Enable ADC conversion */
	ADC_Cmd(ADC1, ENABLE);

}













