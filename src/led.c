/*
 * led.c
 *
 *  Created on: 2015 szept. 14
 *      Author: kecsger12
 */


#include "led.h"

/*================================[Function definitions]==============================================================*/


void LEDs_init()
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitTypeDef GPIO_InitDef;

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_40MHz;

	GPIO_Init(GPIOB, &GPIO_InitDef);
}


void LED1_On()
{
	LEDs_init();

	GPIO_SetBits(GPIOB, GPIO_Pin_9);
}


void LED1_Off()
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_9);
}


void LED2_On()
{
	LEDs_init();

	GPIO_SetBits(GPIOB, GPIO_Pin_8);
}


void LED2_Off()
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_8);

}

