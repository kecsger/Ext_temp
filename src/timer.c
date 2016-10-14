/*
 * timer.c
 *
 *  Created on: 2015 szept. 13
 *      Author: kecsger12
 */

#include "timer.h"

__IO uint32_t TimingDelay = 0;

void Delay_ms(__IO uint32_t nTime)
{
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
