
/**
  ******************************************************************************
  * File   : ResourceManagement_CriticalSections/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Critical sections example
  ******************************************************************************
  */

  @brief
  	#define configUSE_MUTEXES  1
  	#define INCLUDE_vTaskSuspend 1 to support portMAX_DELAY
  	
    This example demonstrates how to protect critical sections

    Basic critical sections are regions of code that are surrounded by calls to 
    the macros taskENTER_CRITICAL() and taskEXIT_CRITICAL(), respectively. Critical
    sections are also known as critical regions.

