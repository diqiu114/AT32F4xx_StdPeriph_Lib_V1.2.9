/**
  @page I2C example
  
  @verbatim
  * File   : I2C/INT(Low_PRI)_MA_RX&SLA_TX(data_only)/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Description of the I2C communication example based on interrupt mode.
  *          This example only uses interrupts that are not the highest priority 
  *          in the data section.
  ******************************************************************************
   @endverbatim

@par Example Description 

  This demo is based on the AT-START-F403 board,in this demo,use hardware I2C1 
  with two board communication and USART1 printf the result. change the macro 
  definition MASTER_BOARD can switch among master-receive/slave-transmit operation.

  ATTENTION:
    1. I2C bus must pull-up
	2. Press the master button to start communication.

 */
