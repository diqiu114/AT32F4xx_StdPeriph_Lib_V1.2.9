/*
  **************************************************************************
  * File   : random.h
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Header file for AT-START board
 */

#ifndef __RANDOM_H
#define __RANDOM_H	 
#include <at32f4xx.h>

#define EnableRTCAsSeed     (1)  //This define  seed for random number gen,RTC for AT32F403/AT32F413,ERTC for AT32F415

void RandNum_Test ( void);


#endif
