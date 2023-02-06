/**
  ******************************************************************************
  * File   : USART/Printf/main.c 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Main program body
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include "at32_board.h"
#include <stdio.h>
#include "st7789.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demos.h"
#include "./lv_sim_visual_studio/LVGL.Simulator/ui/ui.h"
#include "spi_dma.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

void TMR4_GLOBAL_IRQHandler(void)
{
    if(SET==TMR_GetINTStatus(TMR4, TMR_INT_Overflow))
    {
        lv_tick_inc(10);
        TMR_ClearITPendingBit(TMR4, TMR_INT_Overflow);
    }
}
static void Timer4Init(uint16_t Period)
{
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_TMR4, ENABLE);
    TMR_TimerBaseInitType TMR_TimeBaseInitStruct = 
    {
        .TMR_Period =Period * 10 - 1,
        .TMR_RepetitionCounter =0,
        .TMR_CounterMode = TMR_CounterDIR_Up,
        .TMR_ClockDivision = TMR_CKD_DIV1,
        .TMR_DIV = 24000 - 1,
    };
    TMR_TimeBaseInit(TMR4, &TMR_TimeBaseInitStruct);

   NVIC_InitType NVIC_InitStruct = 
   {
       .NVIC_IRQChannel = TMR4_GLOBAL_IRQn,
       .NVIC_IRQChannelCmd = ENABLE,
       .NVIC_IRQChannelPreemptionPriority = 0,
       .NVIC_IRQChannelSubPriority = 0,
   };
   NVIC_Init(&NVIC_InitStruct);
    TMR_INTConfig(TMR4, TMR_INT_Overflow, ENABLE);
    TMR_Cmd(TMR4, ENABLE);
}
/** @addtogroup USART_Printf
  * @{
  */ 
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{     
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  Spi1DmaInit();
  ST7789_Init();
  UART_Print_Init(115200);
  Timer4Init(10);
  AT32_BUTTON_Init(BUTTON_USER_KEY); /*PC13*/
  Delay_init();
  lv_init();
  lv_port_disp_init();
  lv_port_indev_init();

//  BACK_COLOR=WHITE;
//  LCD_ShowString(22,60, (const unsigned char *)"1.54IPS TFT SPI",BLACK);
//  LCD_ShowString(22,80, (const unsigned char *)"LCD_W:",BLACK);	LCD_ShowNum(82,80,LCD_W,3,BLACK);
//  LCD_ShowString(22,100, (const unsigned char *)"LCD_H:",BLACK);LCD_ShowNum(82,100,LCD_H,3,BLACK);
  /* Output a message on Hyperterminal using printf function */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
    // lv_demo_stress();
    ui_init();
  while (1)
  {
    Delay_ms(10);
    lv_task_handler();
//    printf("按钮状态：%d\r\n", AT32_BUTTON_State(BUTTON_WAKEUP) == Bit_SET);
  }
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

  /* Infinite loop */
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

