
/**
  ******************************************************************************
  * File   : TaskManagement_TaskCreatAndDel/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Tasks create, delete, suspend and resume  example
  ******************************************************************************
  */

  @brief
  	#define configUSE_MUTEXES  									1
  	#define INCLUDE_vTaskSuspend 								1 to support portMAX_DELAY
  	#define INCLUDE_vTaskDelete				    			1
  	#define INCLUDE_xTaskResumeFromISR          1
  	
    This example describes how to creat tasks, delete tasks, suspend tasks 
    and resume tasks. command read from USART1

