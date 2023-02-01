/**
  ******************************************************************************
  * File   : ETH/http_server/main.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : This file contains all the functions prototypes for the 
             ethernet function
  ******************************************************************************
  */

#ifndef __MAIN_H
#define __MAIN_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "at32f4xx_eth.h"
#include "at32f4xx_rtc.h"
#include "at32f4xx_pwr.h"
#include "netconf.h"
#include "lwip/tcp.h"

#define SYSTEMTICK_PERIOD_MS  10

//external function
extern void httpd_init(void);

void ADC_Configuration(void);
void Time_Update(void);
void Delay(uint32_t nCount);
void System_Periodic_Handle(void);
#endif
