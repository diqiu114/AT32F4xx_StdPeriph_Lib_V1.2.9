
/**
  @page FreeRTOS_DeferToDaemonTask example
  
  @verbatim
  * File   : FreeRTOS/DeferToDaemonTask/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Description of the FreeRTOS_DeferToDaemonTask example.
  ******************************************************************************
   @endverbatim

  @brief
  	#define configUSE_MUTEXES  			1
  	#define INCLUDE_vTaskSuspend 		1 
  	#define INCLUDE_xTimerPendFunctionCall 1
  	#define configUSE_TIMERS			1
  	
    This example demonstrates deferring interrupt processing to the RTOS daemon
    task, removing the need to create a separate task for each interrupt.

 */