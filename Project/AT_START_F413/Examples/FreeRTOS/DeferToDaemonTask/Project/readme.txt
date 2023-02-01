
/**
  ******************************************************************************
  * File   : InterruptManagement_DeferToDaemonTask/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Defer interrupt processing to the RTOS daemon task example
  ******************************************************************************
  */

  @brief
  	#define configUSE_MUTEXES  			1
  	#define INCLUDE_vTaskSuspend 		1 
  	#define INCLUDE_xTimerPendFunctionCall 1
  	#define configUSE_TIMERS			1
  	
    This example demonstrates deferring interrupt processing to the RTOS daemon
    task, removing the need to create a separate task for each interrupt.

