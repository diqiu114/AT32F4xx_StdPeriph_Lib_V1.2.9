/**
  ******************************************************************************
  * File   : ETH/PMT/main.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : ethernet function test.
  ******************************************************************************
  */

/* Includes -----------------------------------------------------------------*/
#include "at32_board.h"
#include "main.h"
#include "eth_config.h"

/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup ETH_Telnet
  * @{
  */ 
  
/* Gloable variables ---------------------------------------------------------*/
__IO uint32_t LocalTime = 0; /* this variable is used to create a time reference incremented by 10ms */
uint32_t timingdelay;
ErrorStatus HSEStartUpStatus;

/* Gloable functions ---------------------------------------------------------*/
/**
  * @brief  PMT configuration
  * @param  None
  * @retval None
  */
void ETH_PMTConfiguration(void)
{
    EXTI_InitType EXTI_InitStructure;    
    
    //Disable transmitter
    ETH_DMATransmissionCmd(DISABLE);
    ETH_MACTransmissionCmd(DISABLE);
  
    //Disable receiver
    ETH_MACReceptionCmd(DISABLE);
    ETH_DMAReceptionCmd(DISABLE);
  
    //Configure external interrupt
    EXTI_InitStructure.EXTI_Line = EXTI_Line19;
    EXTI_InitStructure.EXTI_LineEnable = ENABLE;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitStructure);
    
    //Enable Magic Packet Detect
    ETH_MagicPacketDetectionCmd(ENABLE);
  
    //EMAC power down
    ETH_PowerDownCmd(ENABLE);
    
    //Enable receiver
    ETH_MACReceptionCmd(ENABLE);
    
    //Enter stop mode
    SysTick->CTRL = 0x00;
    SysTick->VAL = 0x00;
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR, ENABLE);
    EXTI_ClearFlag(EXTI_Line19);
    RTC_ClearINTPendingBit(RTC_INT_OV | RTC_INT_ALA | RTC_INT_PACE);
    PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/**
  * @brief  Main Function.
  * @param  None
  * @retval None
  */
int main(void)
{
  uint32_t Status;
	/* --------BSP Init ------------------------------------------------*/
  AT32_Board_Init();
  UART_Print_Init(115200);
  /* Setup AT32 system (clocks, Ethernet, GPIO, NVIC)*/
  Status = System_Setup();
  while(Status == ETH_DRIVER_FAIL);
  /* Initilaize the LwIP satck ip???????????????mac?????????*/
  LwIP_Init();
  /* Configure PMT */
  printf("Enter Stop Mode\n");
  ETH_PMTConfiguration();
	while(1)
	{         
      System_Periodic_Handle();//??????????????????????????????????????????????????????TCP timers
      printf("Free Run\n");
      Delay(1000);
    
	}
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: number of 10ms periods to wait for.
  * @retval None
  */
void Delay(uint32_t nCount)
{
  /* Capture the current local time */
  timingdelay = LocalTime + nCount;  

  /* wait until the desired delay finish */  
  while(timingdelay > LocalTime)
  {     
  }
}

/**
  * @brief  Updates the system local time
  * @param  None
  * @retval None
  */
void Time_Update(void)
{
  LocalTime += SYSTEMTICK_PERIOD_MS;
}

/**
  * @brief  Handles the periodic tasks of the system
  * @param  None
  * @retval None
  */
void System_Periodic_Handle(void)
{
  /* LwIP periodic services are done here */
  LwIP_Periodic_Handle(LocalTime);
}

/**
  * @brief  Configures system clock after wake-up from STOP: enable HSE, PLL and select PLL as system clock source.
  * @param  None
  * @retval None
  */
void SYSCLKConfig_STOP(void)
{
    uint32_t i;
    RCC_HSEConfig(RCC_HSE_ENABLE);   ///<Enable HSE
    for(i=0;i<20000;i++);
    HSEStartUpStatus = RCC_WaitForHSEStable();
    if(HSEStartUpStatus == SUCCESS)
    {
        RCC_PLLCmd(ENABLE);   ///<Enable PLL
        while(RCC_GetFlagStatus(RCC_FLAG_PLLSTBL) == RESET)  ///<Wait till PLL is ready
        {
        }
        RCC_SYSCLKConfig(RCC_SYSCLKSelction_PLL);            ///<Select PLL as system clock source
        while(RCC_GetSYSCLKSelction() != 0x08)               ///<Wait till PLL is used as system clock source
        {
        }
    }
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif


