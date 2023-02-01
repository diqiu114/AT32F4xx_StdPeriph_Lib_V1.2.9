/**
  ******************************************************************************
  * File   : FreeRTOS/bsp/bsp.c 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Bsp Service Routines.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "bsp.h"

/** @addtogroup AT32F403_StdPeriph_Examples
  * @{
  */

/** @addtogroup FreeRTOS_TaskManagement_CreatDel
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void bsp_Init(void)
{
    /* It is recommended to assign all the priority bits to be preempt 
     * priority bits, leaving no priority bits as subpriority bits
     */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	bsp_InitUart(); 	/* initialize USART1 */
	bsp_InitLed(); 		/* initialize LED */
	bsp_InitKey();		/* initialize KEY */
  bsp_InitHardTimer();/* initialize TMR2 */
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
