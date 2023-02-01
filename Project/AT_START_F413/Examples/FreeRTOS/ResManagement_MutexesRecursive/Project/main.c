/*
 * A mutex is a special type of binary semaphore that is used to control access to a resource that
 * is shared between two or more tasks.
 * For a task to access the resource legitimately, it must first successfully 'take' the token. When
 * the token holder has finished with the resource, it must 'give' the token back. A task is not 
 * permitted to access the shared resource unless it holds the token.
 */
/* Note: don't use App_Printf() to print information */
#include "includes.h"
/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup FreeRTOS_ResManagement_MutexesRecursive
  * @{
  */
  
static void AppTaskCreate (void);
static void AppObjCreate(void);
void App_Printf(const char *format, ...);

static void vTaskLED(void *pvParameters);
static void vTaskMsgProcess(void *pvParameters);
static void vTaskCtrolProcess(void *pvParameters);
static void TMR_CallBack1(void);
static void TMR_CallBack2(void);

/* Declare variables that are used to hold the handle of task */
static TaskHandle_t xHandleTaskLED          = NULL;
static TaskHandle_t xHandleTaskMsgProcess   = NULL;
static TaskHandle_t xHandleTaskCtrolProcess = NULL;
static SemaphoreHandle_t xHandleMutexPrint  = NULL;
static SemaphoreHandle_t xHandleMutexRecursive = NULL;

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

		xSemaphoreTakeRecursive(xHandleMutexRecursive, portMAX_DELAY);
		printf("The LED task obtain the Recursive Mutex\r\n");
		printf("\r\n");
        xSemaphoreGiveRecursive(xHandleMutexRecursive);
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
    
    xSemaphoreTakeRecursive(xHandleMutexRecursive, portMAX_DELAY);
    printf("\r\n1. Print task run-time statistics\r\n");    
    printf("2. Enable Timer CC1 interrupt\r\n");  
    printf("3. Enable Timer CC2 interrupt\r\n");  
    xSemaphoreGiveRecursive(xHandleMutexRecursive);
    while(1)
    {
        comGetChar(COM1,&ucCommandValue);
        switch(ucCommandValue)
        {
            case '1':
                xSemaphoreTakeRecursive(xHandleMutexRecursive, portMAX_DELAY);
                printf("==================================================\r\n");
                printf("Name                  State  Priority  Stack   Num\r\n");
                vTaskList((char*)pcWriteBuffer);
                printf("%s\r\n", pcWriteBuffer);

                printf("\r\n");
                printf("Task                  Abs Time         %%Time      \r\n");
                printf("***************************************************\r\n");
                vTaskGetRunTimeStats((char*)pcWriteBuffer);
                printf("%s\r\n", pcWriteBuffer);
                xSemaphoreGiveRecursive(xHandleMutexRecursive);
                break;
            case '2':
                bsp_StartHardTimer(1, 50000, (void*)TMR_CallBack1);//after 50ms to execute ISR, and then disable CC1 interrupt
                break;
            case '3':
                bsp_StartHardTimer(2, 50000, (void*)TMR_CallBack2);//after 50ms to execute ISR, and then disable CC2 interrupt
                break;
            default:
                break;
        }

        if(ucCommandValue =='1' || ucCommandValue =='2' || ucCommandValue =='3')
        {
            xSemaphoreTakeRecursive(xHandleMutexRecursive, portMAX_DELAY);
            printf("1. Print task run-time statistics\r\n");
            printf("2. Enable Timer CC1 interrupt\r\n");  
            printf("3. Enable Timer CC2 interrupt\r\n"); 
            xSemaphoreGiveRecursive(xHandleMutexRecursive);
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
        vTaskDelay(pdMS_TO_TICKS(1000));

		if(xSemaphoreTakeRecursive(xHandleMutexRecursive, pdMS_TO_TICKS(500))==pdPASS)
        {
		    printf("The first layer for Recursive Mutex\r\n");
		    xSemaphoreTakeRecursive(xHandleMutexRecursive, pdMS_TO_TICKS(500));
		    printf("The second layer for Recursive Mutex\r\n");

		    xSemaphoreTakeRecursive(xHandleMutexRecursive, pdMS_TO_TICKS(500));
		    printf("The third layer for Recursive Mutex\r\n");

            printf("\r\n");
		    xSemaphoreGiveRecursive(xHandleMutexRecursive);
		    xSemaphoreGiveRecursive(xHandleMutexRecursive);
		    xSemaphoreGiveRecursive(xHandleMutexRecursive);
		}
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
	
}

static void AppObjCreate(void)
{
    xHandleMutexPrint = xSemaphoreCreateMutex();

    if(xHandleMutexPrint == NULL)
    {
        printf("There is insufficient heap memory for standard mutex\r\n");
    }

    xHandleMutexRecursive = xSemaphoreCreateRecursiveMutex();

    if(xHandleMutexRecursive == NULL)
    {
        App_Printf("There is insufficient heap memory for recursive mutex\r\n");
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

static void TMR_CallBack1(void)
{
    bsp_LedToggle(3);
}

static void TMR_CallBack2(void)
{
    bsp_LedToggle(4);
}

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
    xSemaphoreTakeRecursive(xHandleMutexRecursive, portMAX_DELAY);
    printf("task: %s stack is overFlow\r\n",pcTaskName);
    xSemaphoreGiveRecursive(xHandleMutexRecursive);
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
 

