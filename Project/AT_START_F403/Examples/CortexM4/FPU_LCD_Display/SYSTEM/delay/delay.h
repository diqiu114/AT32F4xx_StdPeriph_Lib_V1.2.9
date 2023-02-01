/**
  **************************************************************************
  * File   : delay.h
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : none
  **************************************************************************
  */
 
#ifndef __DELAY_H
#define __DELAY_H 			   
#include "sys.h"  

	 
void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif





























