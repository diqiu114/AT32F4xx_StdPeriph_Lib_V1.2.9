/**
  **************************************************************************
  * File   : at32f4xx_it.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : at32f4xx interrupt service routines.
  **************************************************************************
  */ 


/* Includes ------------------------------------------------------------------*/
#include "at32f4xx_it.h"


/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup I2S_FullDuplex
  * @{
  */

extern const uint16_t I2S3_Buffer_Tx[32];
extern uint16_t I2S2_Buffer_Rx[32];
extern  __IO uint32_t TxIdx, RxIdx;

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
  
