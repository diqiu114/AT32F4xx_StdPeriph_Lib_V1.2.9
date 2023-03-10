/**
  ******************************************************************************
  * File   : USB_Device/Custom_HID/inc/hw_config.h
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : USB Hardware Configuration & setup header
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <at32f4xx.h>
#include "usb_type.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define DOWN            1
#define LEFT            2
#define RIGHT           3
#define UP              4
#define CURSOR_STEP     40

#define USB_FIFO_MAX     1024
typedef struct _usb_usart_fifo
{
  uint8_t  fifo[USB_FIFO_MAX];
  uint16_t wrpointer;      /*write pointer*/
  uint16_t curpointer;      /*cur pointer*/
  uint16_t remaindpointer; /*remaind pointer*/
  uint16_t total;
}usb_usart_fifo;

extern usb_usart_fifo usb_txfifo;
extern usb_usart_fifo usb_rxfifo;

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(void);
void GPIO_AINConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
uint16_t CDC_Send_DATA (uint8_t *u8SendBuffer, uint16_t u16Sendlen);
uint16_t CDC_Receive_DATA(uint8_t *u8RcvBuffer, uint16_t u16RecvLen);
void Get_SerialNum(void);
#endif  /*__HW_CONFIG_H*/


