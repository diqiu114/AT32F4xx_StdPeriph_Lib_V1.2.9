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
#include "./spi/spi_dma.h"
#include "./lcd/st7789.h"
#include "stdint.h"
/** @addtogroup AT32F403A_StdPeriph_Examples
 * @{
 */

/** @addtogroup USART_Printf
 * @{
 */

/**
 * @brief  Main program
 * @param  None
 * @retval None
 */
static void colorFill(uint16_t color, uint8_t* pBuffer, int size)
{
  int cnt = 0;
  while(size > cnt)  {
    pBuffer[cnt] = color >> 8;
    cnt++;
    pBuffer[cnt] = color & 0xFF;
    cnt++;
  }
}


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
  Delay_init();
  UART_Print_Init(115200);
  /* Output a message on Hyperterminal using printf function */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");

  ST7789_Init();
  char FillDate[128] = {0, 0};
  char FillDate2[128] = {0, 0}; 
   colorFill(YELLOW, FillDate, sizeof(FillDate));
   colorFill(BLUE, FillDate2, sizeof(FillDate2));
  while (1)
  {
      // Display_SPI_DMA_Send(buffer, sizeof(buffer));
      // for(int cnt = 0; cnt < 0xff; cnt++);
//      ST7789_Test();
//    ST7789_SetAddressWindow(1, 1, 1, 1);

   ST7789_SetAddressWindow(0, 0, ST7789_WIDTH - 1, ST7789_HEIGHT - 1);
   ST7789_Select();
   ST7789_DC_Set();
    for (int i = 0; i < ST7789_WIDTH / 8; i++)
    for (int j = 0; j < ST7789_HEIGHT / 8; j++) {
      Display_SPI_DMA_Send(FillDate, sizeof(FillDate));
      while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY)==SET);
    }
  Delay_ms(1000);
   ST7789_Select();
   ST7789_DC_Set();
    for (int i = 0; i < ST7789_WIDTH / 8; i++)
    for (int j = 0; j < ST7789_HEIGHT / 8; j++) {
      Display_SPI_DMA_Send(FillDate2, sizeof(FillDate2));
      while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY)==SET);
    }
  Delay_ms(1000);
    // ST7789_Fill_Color(MAGENTA);
    // ST7789_Fill_Color(WHITE);
  }
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
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
