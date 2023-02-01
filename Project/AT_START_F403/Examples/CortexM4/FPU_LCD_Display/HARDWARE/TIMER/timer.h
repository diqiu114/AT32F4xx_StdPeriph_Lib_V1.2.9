/**
  **************************************************************************
  * File   : time.h
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : none
  **************************************************************************
  */

#ifndef _TIMER_H
#define _TIMER_H
#include "at32f4xx.h"

void TMR3_Int_Init(u16 arr,u16 psc);
#endif
