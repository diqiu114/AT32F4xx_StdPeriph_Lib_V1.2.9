/*
 * An event group can be used to create a synchronization point:
 * (1)Each task that must participate in the synchronization is
 *    assigned a unique event bit within the event group
 * (2)Each tast sets its own event bit when it reaches the 
 *    synchronization point
 * (3)Having set its own event bit, each task blocks on the event
 *    group to wait for the event bits that represent all the 
 *    other synchronizing tasks to also become set.
 *
 *  However, the xEventGroupSetBits() and xEventGroupWaitBits() API
 *  functions cannot be used in this scenario. If they ware used, 
 *  then the setting of a bit and the testing of bits would be 
 *  performed as two separate operations. To successfully use an 
 *  event group to create a synchronization point, the setting of
 *  an event bit, and the subsequent testing of event bits, must 
 *  be performed as a single uninterruptable operation. The
 *  xEventGroupSync() API function is provided for that purpose.
 */

#include "includes.h"
/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup FreeRTOS_EvtGroup_MultiTasksSyn
  * @{
  */ 
  
#define BIT_0 (1<<0)
#define BIT_1 (1<<1)
#define BIT_2 (1<<2)
#define ALL_SYNC_BITS (BIT_0 | BIT_1 | BIT_2)

static void AppTaskCreate (void);
static void AppObjCreate(void);
void App_Printf(const char *format, ...);

static void vTaskLED(void *pvParameters);
static void vTaskMsgProcess(void *pvParameters);
static void vTaskCtrolProcess(void *pvParameters);
static void vTaskEventGroup0(void *pvParameters);
static void vTaskEventGroup1(void *pvParameters);
static void vTaskEventGroup2(void *pvParameters);

//static void TMR_CallBack1(void);
//static void TMR_CallBack2(void);

/* Declare variables that are used to hold the handle of task */
static TaskHandle_t xHandleTaskLED          = NULL;
static TaskHandle_t xHandleTaskMsgProcess   = NULL;
static TaskHandle_t xHandleTaskCtrolProcess      = NULL;
static SemaphoreHandle_t xHandleMutexPrint = NULL;

static TaskHandle_t xHandleTaskEventGroup0          = NULL;
static TaskHandle_t xHandleTaskEventGroup1          = NULL;
static TaskHandle_t xHandleTaskEventGroup2          = NULL;

static EventGroupHandle_t xHandleEventGroupPrint = NULL;

int main(void)
{
	__set_PRIMASK(1); //disable global interrupts 
	
	/* Perform any hardware setup necessary */
	bsp_Init(); 

    /* setup a timer for gather task statistics */
	vTimerInitForRunTimeState();
	
	/* Application tasks created */
	AppTaskCreate();

    /* Application communication mechanism created */
	AppObjCreate();
	
    /* Start the created tasks running */
    vTaskStartScheduler();

    /* If all is well then main() will never reach here as the scheduler will
     * now be running the tasks. If main() does reach here then it is likely
     * that there was insufficient heap memory available for the idle task to be 
     * created
     */
	while(1);
    //return 0;
}

/**
 * @brief   LED task
 * @param   pvParameters: passed to the task
 * @retval  None
 */

static void vTaskLED(void *pvParameters)
{
    while(1)
    {
		bsp_LedToggle(2);//LED2
		vTaskDelay(pdMS_TO_TICKS(1000));//delay 1s
	}

}

/**
 * @brief   obtain task run-time statistics 
 * @param   pvParameters: passed to the task
 * @retval  None
 */
static void vTaskMsgProcess(void *pvParameters)
{
    uint8_t ucCommandValue=0xFF;
    uint8_t pcWriteBuffer[500];
    
    //App_Printf("\r\n1. Print task run-time statistics\r\n");    
    while(1)
    {
        comGetChar(COM1,&ucCommandValue);
        switch(ucCommandValue)
        {
            case '1':
                App_Printf("==================================================\r\n");
                App_Printf("Name                  State  Priority  Stack   Num\r\n");
                vTaskList((char*)pcWriteBuffer);
                App_Printf("%s\r\n", pcWriteBuffer);

                App_Printf("\r\n");
                App_Printf("Task                  Abs Time         %%Time      \r\n");
                App_Printf("***************************************************\r\n");
                vTaskGetRunTimeStats((char*)pcWriteBuffer);
                App_Printf("%s\r\n", pcWriteBuffer);
                break;
            case '2':
                //bsp_StartHardTimer(1, 50000, (void*)TMR_CallBack1);//after 50ms to execute ISR, and then disable CC1 interrupt
                break;
            case '3':
                //bsp_StartHardTimer(2, 50000, (void*)TMR_CallBack2);//after 50ms to execute ISR, and then disable CC2 interrupt
                break;
            default:
                break;
        }

        if(ucCommandValue =='1' || ucCommandValue =='2' || ucCommandValue =='3')
        {
            //App_Printf("1. Print task run-time statistics\r\n");
            comClearRxFifo(COM1);
            ucCommandValue = 0xFF;
        }
           
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}


static void vTaskCtrolProcess(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
		//App_Printf("This is Ctrol Task\r\n");
    }
}

//create all tasks within the application
static void AppTaskCreate (void)
{
    BaseType_t xReturn;
    
    xReturn = xTaskCreate(  vTaskLED,   	    // function of task
                            "vTaskLED",         // task name. used for debugging aid only*/
                            512,               	// stack size. specify the number of words(32bits) */
                            NULL,              	// task parameter*/
                            2,                 	// task priority. Note: the smaller number, the lower priority*/
                            &xHandleTaskLED );  // task handle. used to reference the task in API calls, for example, change the task priority or delete the task*/
	if(xReturn != pdPASS)
	{
	    while(1);//There is insufficient heap memory
	}
	xReturn = xTaskCreate(  vTaskMsgProcess,     		
                            "vTaskMsgProcess",   		
                            512,             		
                            NULL,           		
                            3,               		
                            &xHandleTaskMsgProcess );  
    if(xReturn != pdPASS)
	{
	    while(1);//There is insufficient heap memory
	}
	
	xReturn = xTaskCreate(  vTaskCtrolProcess,     		
                            "vTaskCtrolProcess",   		
                            512,            		
                            NULL,           		
                            4,              		
                            &xHandleTaskCtrolProcess ); 
    if(xReturn != pdPASS)
	{
	    while(1);//There is insufficient heap memory
	}

	xReturn = xTaskCreate(  vTaskEventGroup0,     		
                            "vTaskEventGroup0",   		
                            512,            		
                            NULL,           		
                            4,              		
                            &xHandleTaskEventGroup0 ); 
    if(xReturn != pdPASS)
	{
	    while(1);//There is insufficient heap memory
	}

	xReturn = xTaskCreate(  vTaskEventGroup1,     		
                            "vTaskEventGroup1",   		
                            512,            		
                            NULL,           		
                            3,              		
                            &xHandleTaskEventGroup1 ); 
    if(xReturn != pdPASS)
	{
	    while(1);//There is insufficient heap memory
	}

	xReturn = xTaskCreate(  vTaskEventGroup2,     		
                            "vTaskEventGroup2",   		
                            512,            		
                            NULL,           		
                            2,              		
                            &xHandleTaskEventGroup2 ); 
    if(xReturn != pdPASS)
	{
	    while(1);//There is insufficient heap memory
	}
}

static void AppObjCreate(void)
{
    xHandleMutexPrint = xSemaphoreCreateMutex();

    if(xHandleMutexPrint == NULL)
    {
        printf("There is insufficient heap memory\r\n");
    }

    xHandleEventGroupPrint = xEventGroupCreate();

    if(xHandleEventGroupPrint == NULL)
    {
        printf("There was insufficient FreeRTOS heap available.\r\n");
    }
}

void App_Printf(const char *format, ...)
{
    char buf_str[200+1];//max number contain character is 200
    va_list v_args;

    va_start(v_args, format);
    
    (void)vsnprintf((char*)&buf_str[0],
                    (size_t)sizeof(buf_str),
                    (char const*)format,
                                v_args);
    va_end(v_args);

    xSemaphoreTake(xHandleMutexPrint, portMAX_DELAY);//block indefinitely until mutex be obtained
    printf("%s", buf_str);
    xSemaphoreGive(xHandleMutexPrint);//the mutex must be given back
}

static void vTaskEventGroup0(void * pvParameters)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        /* Set bit 0 in the event group to note this task has reached the
         * sync point. The other two tasks will set the other two bits defined
         * by ALL_SYNC_BITS. All three tasks have reached the synchronization
         * point when all the ALL_SYNC_BITS bits are set.
         */
        xEventGroupSync( xHandleEventGroupPrint,
                                    BIT_0,//set the bit 0
                                    ALL_SYNC_BITS,//wait for all bits
                                    portMAX_DELAY);

        App_Printf("Task0 makes the synchronization is completed\r\n");

    }
}

static void vTaskEventGroup1(void * pvParameters)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));

        xEventGroupSync(xHandleEventGroupPrint,BIT_1,ALL_SYNC_BITS,portMAX_DELAY);

        /* xEventGroupSync() was called with an indefinite block time,
         * so this task will only reach here if the synchronization was
         * made by all three task, so there is no need to test the 
         * return value 
         */
        App_Printf("Task1 makes the synchronization is completed\r\n");
    }
}

static void vTaskEventGroup2(void * pvParameters)
{
    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));

        xEventGroupSync(xHandleEventGroupPrint,BIT_2,ALL_SYNC_BITS,portMAX_DELAY);

        App_Printf("Task2 makes the synchronization is completed\r\n");
    }
}

#if 0
static void TMR_CallBack1(void)
{
    bsp_LedToggle(3);
}

static void TMR_CallBack2(void)
{
    bsp_LedToggle(4);
}
#endif

#if ( configUSE_IDLE_HOOK == 1 )
/* Call the user defined function from within the idle task.  This
 * allows the application designer to add background functionality
 * without the overhead of a separate task.
 *  
 * NOTE: vApplicationIdleHook() MUST NOT, UNDER ANY CIRCUMSTANCES,
 * CALL A FUNCTION THAT MIGHT BLOCK. THE FUNCTION NAME IS FIXED*/

void vApplicationIdleHook( void )
{
    while(1);
}

#endif /* configUSE_IDLE_HOOK */

#if ( configUSE_TICK_HOOK == 1 )
/* The tick hook gets called at regular intervals, even if the
 * scheduler is locked. 
 * NOTE: THE FUNCTION NAME IS FIXED */

void vApplicationTickHook( void )
{
    while(1);
}

#endif /* configUSE_TICK_HOOK */

#if ( configUSE_MALLOC_FAILED_HOOK == 1 )
/* NOTE: THE FUNCTION NAME IS FIXED */

void vApplicationMallocFailedHook(void)
{
    while(1);
}

#endif /* configUSE_MALLOC_FAILED_HOOK */

#if (  configCHECK_FOR_STACK_OVERFLOW > 0 )
/* The stack overflow hook function.
 * xTask: handle of the task that has exceeded its stack space
 * pcTaskName: name of the task that has exceeded its stack sapce
 * NOTE: stack overflow checking can only be used on architectures
 *       that have a linear memory map
 */
void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
    App_Printf("task: %s stack is overFlow\r\n",pcTaskName);
}
#endif

#if( configUSE_DAEMON_TASK_STARTUP_HOOK == 1 )
/* Allow the application writer to execute some code in the context of
 * this task at the point the task starts executing.  This is useful if the
 * application includes initialisation code that would benefit from
 * executing after the scheduler has been started. */

void vApplicationDaemonTaskStartupHook( void )
{
}

#endif /* configUSE_DAEMON_TASK_STARTUP_HOOK */
/**
  * @}
  */ 

/**
  * @}
  */ 
  

