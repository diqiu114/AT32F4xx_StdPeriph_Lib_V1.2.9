/**
  ******************************************************************************
  * File   : FreeRTOS/QM_Mailbox/at32f4xx_assert.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Assert Service Routines.
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"	
#include <stdio.h>

/** @addtogroup AT32F403_StdPeriph_Examples
  * @{
  */

/** @addtogroup FreeRTOS_QM_Mailbox
  * @{
  */ 
#ifdef USE_FULL_ASSERT

/**
 * @brief   assert failed 
 * @param   file:file name
 * @param   line:line number 
 * @retval  None
 */
void assert_failed(uint8_t* file, uint32_t line)
{ 

	while (1)
	{
	}
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
