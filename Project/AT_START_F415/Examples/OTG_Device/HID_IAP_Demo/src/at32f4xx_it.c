/**
  ******************************************************************************
  * File   : at32f4xx_it.c 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"
#include "usb_dcd_int.h"
#include "usbd_customhid_core.h"

extern USB_OTG_CORE_HANDLE    USB_OTG_dev;
extern __IO uint8_t Send_Buffer[];
/** @addtogroup AT32F415_StdPeriph_Examples
  * @{
  */

/** @addtogroup GPIO_LEDToggle
  * @{
  */

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}


void USBWakeUp_IRQHandler(void)
{  
    EXTI_ClearIntPendingBit(EXTI_Line18);
}


void USBOTG_IRQHandler(void)
{
  USB_DEVICE_OTG_INTSTS_Handler(&USB_OTG_dev);
}

/**
  * @brief  This function handles External lines 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  
  if(EXTI_GetIntStatus(EXTI_Line0) != RESET)
  { 


    Send_Buffer[0] = 0x05;
    
    if (GPIO_ReadInputDataBit(GPIOA,GPIO_Pins_0) == Bit_SET)
    {
      Send_Buffer[1] = (~Send_Buffer[1])&0x1;
    }
    usb_device_hid_sendreport(&USB_OTG_dev, (uint8_t *)Send_Buffer, 64);
      /* Write the descriptor through the endpoint */    
    /* Clear the EXTI line 13 pending bit */
    EXTI_ClearIntPendingBit(EXTI_Line0);
  }
}
/******************************************************************************/
/*                 AT32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f413_xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

