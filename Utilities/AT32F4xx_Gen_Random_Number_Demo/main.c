/**
  **************************************************************************
  * File   : main.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Main program body.
  **************************************************************************
  */
 
/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include <stdio.h>
#include "at32_board.h"

#include "random.h"

/** @addtogroup Gen Random number example
  * @{
  */
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */


int main(void)
{     


  AT32_Board_Init();
  UART_Print_Init(115200);
  printf("----------------- Random Number Gen Test  -----------------------\r\n");
  RandNum_Test();
  while (1)
  {

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

  printf("Wrong parameters value: file %s on line %d\r\n", file, line);

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

