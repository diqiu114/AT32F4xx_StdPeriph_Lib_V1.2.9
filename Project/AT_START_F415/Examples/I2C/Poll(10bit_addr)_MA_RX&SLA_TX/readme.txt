/**
  @page I2C example
  
  @verbatim
  * File   : I2C/Poll(10bit_addr)_MA_RX&SLA_TX/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Description of the I2C communication example with 10 bit address 
  *          based on polling mode.
  ******************************************************************************
   @endverbatim

@par Example Description 

  This demo is based on the AT-START-F415 board,in this demo,use hardware I2C1 
  with two board communication and USART1 printf the result. change the macro 
  definition MASTER_BOARD can switch among master-receive/slave-transmit operation.

  ATTENTION:
    1. I2C bus must pull-up
	2. Press the master button to start communication.

 */
