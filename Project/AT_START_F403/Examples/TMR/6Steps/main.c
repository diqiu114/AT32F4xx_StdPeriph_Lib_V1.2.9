 /**
  ******************************************************************************
  * File   : TMR/TMR_6Steps/main.c 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Main program body
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"

/** @addtogroup AT32F403_StdPeriph_Examples
  * @{
  */
  
/** @addtogroup TMR_6Steps
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
TMR_TimerBaseInitType  TMR_TimeBaseStructure = {0};
TMR_OCInitType  TMR_OCInitStructure = {0};
TMR_BRKDTInitType TMR_BDTRInitStructure = {0};
uint16_t CCR1_Val = 32767;
uint16_t CCR2_Val = 24575;
uint16_t CCR3_Val = 16383;
uint16_t CCR4_Val = 8191;

/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void SysTick_Configuration(void);
void NVIC_Configuration(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief   Main program
  * @param  None
  * @retval None
  */
int main(void)
{             
  /* System Clocks Configuration */
  RCC_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* SysTick Configuration */
  SysTick_Configuration();

  /*-----------------------------------------------------------------------------
  The AT32F403 TMR1 peripheral offers the possibility to program in advance the 
  configuration for the next TMR1 outputs behaviour (step) and change the configuration
  of all the channels at the same time. This operation is possible when the COM 
  (commutation) event is used.
  The COM event can be generated by software by setting the COM bit in the TMR1_EGR
  register or by hardware (on TRC rising edge).
  In this example, a software COM event is generated each 100 ms: using the Systick 
  interrupt.
  The TMR1 is configured in Timing Mode, each time a COM event occurs, 
  a new TMR1 configuration will be set in advance.
  The following Table  describes the TMR1 Channels states:
              -----------------------------------------------
             | Step1 | Step2 | Step3 | Step4 | Step5 | Step6 |
   ----------------------------------------------------------
  |Channel1  |   1   |   0   |   0   |   0   |   0   |   1   |
   ----------------------------------------------------------
  |Channel1N |   0   |   0   |   1   |   1   |   0   |   0   |
   ----------------------------------------------------------
  |Channel2  |   0   |   0   |   0   |   1   |   1   |   0   |
   ----------------------------------------------------------
  |Channel2N |   1   |   1   |   0   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3  |   0   |   1   |   1   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3N |   0   |   0   |   0   |   0   |   1   |   1   |
   ----------------------------------------------------------
  -----------------------------------------------------------------------------*/

  /* Time Base configuration */
  TMR_TimeBaseStructInit(&TMR_TimeBaseStructure);
  TMR_TimeBaseStructure.TMR_DIV = 0;
  TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
  TMR_TimeBaseStructure.TMR_Period = 4095;
  TMR_TimeBaseStructure.TMR_ClockDivision = 0;
  TMR_TimeBaseStructure.TMR_RepetitionCounter = 0;

  TMR_TimeBaseInit(TMR1, &TMR_TimeBaseStructure);

  /* Channel 1, 2,3 and 4 Configuration in PWM mode */
  TMR_OCStructInit(&TMR_OCInitStructure);
  TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_Timing;
  TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;
  TMR_OCInitStructure.TMR_OutputNState = TMR_OutputNState_Enable;
  TMR_OCInitStructure.TMR_Pulse = 2047;
  TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;
  TMR_OCInitStructure.TMR_OCNPolarity = TMR_OCNPolarity_High;
  TMR_OCInitStructure.TMR_OCIdleState = TMR_OCIdleState_Set;
  TMR_OCInitStructure.TMR_OCNIdleState = TMR_OCNIdleState_Set;

  TMR_OC1Init(TMR1, &TMR_OCInitStructure);

  TMR_OCInitStructure.TMR_Pulse = 1023;
  TMR_OC2Init(TMR1, &TMR_OCInitStructure);

  TMR_OCInitStructure.TMR_Pulse = 511;
  TMR_OC3Init(TMR1, &TMR_OCInitStructure);

  /* Automatic Output enable, Break, dead time and lock configuration*/
  TMR_BRKDTStructInit(&TMR_BDTRInitStructure);
  TMR_BDTRInitStructure.TMR_OSIMRState = TMR_OSIMRState_Enable;
  TMR_BDTRInitStructure.TMR_OSIMIState = TMR_OSIMIState_Enable;
  TMR_BDTRInitStructure.TMR_LOCKgrade = TMR_LOCKgrade_OFF;
  TMR_BDTRInitStructure.TMR_DeadTime = 1;
  TMR_BDTRInitStructure.TMR_Break = TMR_Break_Enable;
  TMR_BDTRInitStructure.TMR_BreakPolarity = TMR_BreakPolarity_High;
  TMR_BDTRInitStructure.TMR_AutomaticOutput = TMR_AutomaticOutput_Enable;

  TMR_BRKDTConfig(TMR1, &TMR_BDTRInitStructure);

  TMR_CCPreloadControl(TMR1, ENABLE);

  TMR_INTConfig(TMR1, TMR_INT_HALL, ENABLE);

  /* TMR1 counter enable */
  TMR_Cmd(TMR1, ENABLE);

  /* Main Output Enable */
  TMR_CtrlPWMOutputs(TMR1, ENABLE);

  while (1)
  {}
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{  
  /* TMR1, GPIOA, GPIOB, GPIOE and AFIO clocks enable */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_TMR1 | RCC_APB2PERIPH_GPIOA | RCC_APB2PERIPH_GPIOE|
                         RCC_APB2PERIPH_GPIOB |RCC_APB2PERIPH_AFIO, ENABLE);
}

/**
  * @brief  Configure the TMR1 Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitType GPIO_InitStructure = {0};

  /* GPIOA Configuration: Channel 1, 2 and 3 as alternate function push-pull */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8 | GPIO_Pins_9 | GPIO_Pins_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* GPIOB Configuration: Channel 1N, 2N and 3N as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_13 | GPIO_Pins_14 | GPIO_Pins_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* GPIOB Configuration: BKIN pin */
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_12;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOB, &GPIO_InitStructure);  
}

/**
  * @brief  Configures the SysTick.
  * @param  None
  * @retval None
  */
void SysTick_Configuration(void)
{
  /* Setup SysTick Timer for 50 msec interrupts  */
  if (SysTick_Config((SystemCoreClock) / 20))
  { 
    /* Capture error */ 
    while (1);
  }

   NVIC_SetPriority(SysTick_IRQn, 0x0);
}

/**
  * @brief  Configures the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  NVIC_InitType NVIC_InitStructure = {0};

  /* Enable the TMR1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TMR1_TRG_COM_TMR11_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure); 

}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  while (1)
  {}
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 
