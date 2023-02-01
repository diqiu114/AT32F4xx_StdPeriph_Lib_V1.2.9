
#include "bsp.h"

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

