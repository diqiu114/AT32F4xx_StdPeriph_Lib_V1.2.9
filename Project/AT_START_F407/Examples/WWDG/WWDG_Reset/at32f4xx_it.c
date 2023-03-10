/**
  ******************************************************************************
  * File   : WWDG/WWDG_Reset/at32f4xx_it.c 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Interrupt program body
  ******************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"
/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup WWDG_Reset
  * @{
  */

extern uint8_t EXTI_flag;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            AT32F407 Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetIntStatus(EXTI_Line0) != RESET)
  {
    EXTI_flag = 1;
    
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearIntPendingBit(EXTI_Line0);
  }
}

/******************************************************************************/
/*                 at32f4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f403_xx.s).                                            */
/******************************************************************************/

/**
  * @}
  */

/**
  * @}
  */
  
