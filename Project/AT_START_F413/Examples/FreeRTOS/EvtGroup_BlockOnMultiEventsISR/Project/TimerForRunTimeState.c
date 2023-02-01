#include "bsp.h"


/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup FreeRTOS_EvtGroup_BlockOnMultiEventsISR
  * @{
  */ 
#define  timerINTERRUPT_FREQUENCY	20000 //50us
#define  timerHIGHEST_PRIORITY		1

//the time base for gathering task statistics  
volatile uint32_t ulHighFrequencyTimerTicks = 0UL;

//configure timer4 for gather task statistcs
void vTimerInitForRunTimeState(void)
{
	bsp_SetTMRforInt(TMR4, timerINTERRUPT_FREQUENCY, timerHIGHEST_PRIORITY, 0);
}

//interrupt handler for timer4
void TMR4_GLOBAL_IRQHandler( void )
{
	if(TMR_GetINTStatus(TMR4, TMR_INT_Overflow) != RESET)
	{
		ulHighFrequencyTimerTicks++;
		TMR_ClearITPendingBit(TMR4, TMR_INT_Overflow);
	}
}
/**
  * @}
  */ 

