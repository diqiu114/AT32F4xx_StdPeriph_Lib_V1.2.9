/**
  ******************************************************************************
  * File   : TMR/OCToggle/at32f4xx_it.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"

/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup TMR_TMR9_OCToggle
  * @{
  */

uint16_t capture = 0;
extern __IO uint16_t CCR1Val;
extern __IO uint16_t CCR2Val;

void TMRingDelay_Decrement(void);
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
//  TMRingDelay_Decrement();
}


/**
  * @brief  This function handles TMR1 Break and TMR9 interrupts request.
  * @param  None
  * @retval None
  */
void TMR1_BRK_TMR9_IRQHandler(void)
{
  /* TMR9_CH1 toggling with frequency = 366.2 Hz */
  if (TMR_GetINTStatus(TMR9, TMR_INT_CC1) != RESET)
  {
    TMR_ClearITPendingBit(TMR9, TMR_INT_CC1 );
    capture = TMR_GetCapture1(TMR9);
    TMR_SetCompare1(TMR9, capture + CCR1Val );
  }

  /* TMR9_CH2 toggling with frequency = 732.4 Hz */
  if (TMR_GetINTStatus(TMR9, TMR_INT_CC2) != RESET)
  {
    TMR_ClearITPendingBit(TMR9, TMR_INT_CC2);
    capture = TMR_GetCapture2(TMR9);
    TMR_SetCompare2(TMR9, capture + CCR2Val);
  }
}
/**
  * @}
  */ 

/**
  * @}
  */

