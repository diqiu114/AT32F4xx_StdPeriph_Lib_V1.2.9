
/**
  ******************************************************************************
  * File   : QueueManagement_BlockOnSend/readme.txt 
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Blocking when sending to a queue example
  ******************************************************************************
  */

  @brief
  	#define configUSE_MUTEXES  1
  	#define INCLUDE_vTaskSuspend 1 to support portMAX_DELAY
  	
    This example demonstrates a queue being created, data being sent to the queue
    from multiple tasks, and data being received from the queue. The queue is created
    to hold data items of type Data_t.
    
    The priority of the tasks that send to the queue are higher than the priority of the 
    task that receives from the queue. This means the queue will normally be full. This is 
    because, as soon as the receiving task removes an itme form the queue, it is pre-empted 
    by one of the sending tasks which then immediately refills the queue. The sending task
    then re-enters the Blocked state to wait for space to become available on the queue again.

