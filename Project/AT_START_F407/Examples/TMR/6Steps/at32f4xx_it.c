/**
   ******************************************************************************
   * File   : TMR/TMR_6Steps/at32f4xx_it.c
  * Version: V1.2.9
  * Date   : 2021-01-15
   * Brief  : Main Interrupt Service Routines.
   *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  */ 
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

/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */
  
/** @addtogroup TMR_6Steps
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t step = 1;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

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
  {}
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
  {}
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
  {}
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
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  /* Generate TMR1 COM event by software */
  TMR_GenerateEvent(TMR1, TMR_EventSource_HALL);
}

/******************************************************************************/
/*            AT32F407 Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles TMR1 Trigger and commutation interrupts
  *   requests.
  * @param  None
  * @retval None
  */
void TMR1_TRG_COM_TMR11_IRQHandler(void)
{
   /* Clear TMR1 COM pending bit */
   TMR_ClearITPendingBit(TMR1, TMR_INT_HALL);

  if (step == 1)
  {
    /* Next step: Step 2 Configuration ---------------------------- */
    /*  Channel3 configuration */
    TMR_CCxCmd(TMR1, TMR_Channel_3, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_3, TMR_CCxN_Disable);

    /*  Channel1 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_1, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_1, TMR_CCx_Enable);
    TMR_CCxNCmd(TMR1, TMR_Channel_1, TMR_CCxN_Disable);

    /*  Channel2 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_2, TMR_OCMode_PWM1 );
    TMR_CCxCmd(TMR1, TMR_Channel_2, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_2, TMR_CCxN_Enable);
    step++;
  }
  else if (step == 2)
  {
    /* Next step: Step 3 Configuration ---------------------------- */
    /*  Channel2 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_2, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_2, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_2, TMR_CCxN_Enable);
    
    /*  Channel3 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_3, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_3, TMR_CCx_Enable);
     TMR_CCxNCmd(TMR1, TMR_Channel_3, TMR_CCxN_Disable);

    /*  Channel1 configuration */
    TMR_CCxCmd(TMR1, TMR_Channel_1, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_1, TMR_CCxN_Disable);
    step++;
  }
  else if (step == 3)
  {
    /* Next step: Step 4 Configuration ---------------------------- */
    /*  Channel3 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_3, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_3, TMR_CCx_Enable);
    TMR_CCxNCmd(TMR1, TMR_Channel_3, TMR_CCxN_Disable);

    /*  Channel2 configuration */
    TMR_CCxCmd(TMR1, TMR_Channel_2, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_2, TMR_CCxN_Disable);

    /*  Channel1 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_1, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_1, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_1, TMR_CCxN_Enable);
    step++;
  }
  else if (step == 4)
  {
    /* Next step: Step 5 Configuration ---------------------------- */
    /*  Channel3 configuration */
    TMR_CCxCmd(TMR1, TMR_Channel_3, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_3, TMR_CCxN_Disable);

    /*  Channel1 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_1, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_1, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_1, TMR_CCxN_Enable);

    /*  Channel2 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_2, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_2, TMR_CCx_Enable);
    TMR_CCxNCmd(TMR1, TMR_Channel_2, TMR_CCxN_Disable);
    step++;
  }
  else if (step == 5)
  {
    /* Next step: Step 6 Configuration ---------------------------- */
    /*  Channel3 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_3, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_3, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_3, TMR_CCxN_Enable);

    /*  Channel1 configuration */
    TMR_CCxCmd(TMR1, TMR_Channel_1, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_1, TMR_CCxN_Disable);

    /*  Channel2 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_2, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_2, TMR_CCx_Enable);
    TMR_CCxNCmd(TMR1, TMR_Channel_2, TMR_CCxN_Disable);
    step++;
  }
  else
  {
    /* Next step: Step 1 Configuration ---------------------------- */
    /*  Channel1 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_1, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_1, TMR_CCx_Enable);
    TMR_CCxNCmd(TMR1, TMR_Channel_2, TMR_CCxN_Disable);

    /*  Channel3 configuration */
    TMR_SelectOCxM(TMR1, TMR_Channel_3, TMR_OCMode_PWM1);
    TMR_CCxCmd(TMR1, TMR_Channel_3, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_3, TMR_CCxN_Enable);

    /*  Channel2 configuration */
    TMR_CCxCmd(TMR1, TMR_Channel_2, TMR_CCx_Disable);
    TMR_CCxNCmd(TMR1, TMR_Channel_2, TMR_CCxN_Disable);
    step = 1;
  }
}


/**
  * @}
  */ 

/**
  * @}
  */ 


