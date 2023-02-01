
/**
  ******************************************************************************
  * File   : BinarySemaphore_SynTaskWithISR/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Using binary semaphore to synchronize the task with the interrupt example
  ******************************************************************************
  */

  @brief
  	#define configUSE_MUTEXES  1
  	#define INCLUDE_vTaskSuspend 1 to support portMAX_DELAY
  	
    This example demonstrates how to synchronize the task with the interrupt using 
    binary semaphore. The binary semaphore is used to defer interrupt processing 
    to a task.

    The structure of the task is adequate only if interrupts occur at a relatively
    low frequency, or the interrupt will be missed.

