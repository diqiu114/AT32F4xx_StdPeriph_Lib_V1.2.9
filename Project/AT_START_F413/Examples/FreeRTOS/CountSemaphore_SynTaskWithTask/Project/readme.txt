
/**
  ******************************************************************************
  * File   : CountingSemaphore_SynTaskWithTask/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Using counting semaphore to synchronize the task with another task example
  ******************************************************************************
  */

  @brief
  	#define configUSE_MUTEXES  				1
  	#define INCLUDE_vTaskSuspend 			1 
  	#define configUSE_COUNTING_SEMAPHORES 	1
  	
    This example demonstrates how to synchronize the task with another task using 
    counting semaphore. 
    
