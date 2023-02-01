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
/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup FreeRTOS_ResManagement_MutexesAndTask
  * @{
  */
 
#define ERR_INFO "\r\nEnter HardFault_Handler, System Halt.\r\n"

/*
 * @brief  This function handles NMI exception.
 * @param  None
 * @retval None
 */
void NMI_Handler(void)
{
}

/*
 * @brief  This function handles Hard Fault exception.
 * @param  None
 * @retval None
 */
void HardFault_Handler(void)
{
#if 1
    const char *pError = ERR_INFO;
    uint8_t i;
    
    for (i = 0; i < sizeof(ERR_INFO); i++)
    {
        USART1->DT = pError[i];
         
        while ((USART1->STS & USART_FLAG_TRAC) == (uint16_t)RESET);
    }
#endif	

    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/*
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

/*
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

/*
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

#if 0
/*
 * @brief  This function handles SVCall exception.
 * @param  None
 * @retval None
 */
void SVC_Handler(void)
{
}
#endif

/*
 * @brief  This function handles Debug Monitor exception.
 * @param  None
 * @retval None
 */
void DebugMon_Handler(void)
{
}

#if 0
/*
 * @brief  This function handles PendSV_Handler exception.
 * @param  None
 * @retval None
 */
void PendSV_Handler(void)
{
}
#endif

#if 0
/*
 * @brief  This function handles SysTick Handler.
 * @param  None
 * @retval None
 */
void SysTick_Handler(void)
{
}
#endif

/******************************************************************************/
/*                 AT32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f413_xx.s).                                            */
/******************************************************************************/
/**
  * @}
  */ 

/**
  * @}
  */
 