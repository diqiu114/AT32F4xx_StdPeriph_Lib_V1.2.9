/**
  ******************************************************************************
  * File   : Project/bsp/bsp_led.h
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Bsp Service Routines.
  ******************************************************************************
  */ 

#ifndef __BSP_LED_H
#define __BSP_LED_H

void bsp_InitLed(void);
void bsp_LedOn(uint8_t _no);
void bsp_LedOff(uint8_t _no);
void bsp_LedToggle(uint8_t _no);

#endif

