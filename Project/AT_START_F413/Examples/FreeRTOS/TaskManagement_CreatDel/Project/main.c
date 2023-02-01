#include "includes.h"
/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup FreeRTOS_TaskManagement_CreatDel
  * @{
  */
 
static void AppTaskCreate (void);
static void AppObjCreate(void);
void App_Printf(const char *format, ...);

static void vTaskLED(void *pvParameters);
static void vTaskMsgProcess(void *pvParameters);
static void vTaskCtrolProcess(void *pvParameters);
static void TMR_CallBack1(void);
//static void TMR_CallBack2(void);

/* Declare variables that are used to hold the handle of task */
static TaskHandle_t xHandleTaskLED          = NULL;
static TaskHandle_t xHandleTaskMsgProcess   = NULL;
static TaskHandle_t xHandleTaskCtrolProcess      = NULL;
static SemaphoreHandle_t xHandleMutexPrint = NULL;

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
    
    App_Printf("\r\n1. Print task run-time statistics\r\n");    
    App_Printf("2. Creat LED task\r\n");
    App_Printf("3. Delete LED task\r\n");
    App_Printf("4. Suspend LED task\r\n");
    App_Printf("5. Resume LED task\r\n");
    App_Printf("6. Resume LED task from ISR\r\n");
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
                App_Printf("***************************************************\r\n");
                App_Printf("Task                  Abs Time         %%Time      \r\n");
                vTaskGetRunTimeStats((char*)pcWriteBuffer);
                App_Printf("%s\r\n", pcWriteBuffer);
                break;
            case '2':
                if(xHandleTaskLED == NULL)
                {
                    xTaskCreate( vTaskLED,   	    // function of task
                                 "vTaskLED",        // task name. used for debugging aid only*/
                                 512,               // stack size. specify the number of words(32bits) */
                                 NULL,              // task parameter*/
                                 2,                 // task priority. Note: the smaller number, the lower priority*/
                                 &xHandleTaskLED ); // task handle. used to reference the task in API calls, for example, change the task priority or delete the task*/
                    App_Printf("LED task has created successfully\r\n");
                }
                else
                {
                    App_Printf("LED task has created already\r\n");
                }
                App_Printf("\r\n");
                break;
            case '3':
                if(xHandleTaskLED != NULL)
                {
                    vTaskDelete(xHandleTaskLED);
                    xHandleTaskLED = NULL;
                    App_Printf("LED task deleted\r\n");
                }
                else
                {
                    App_Printf("LED task has not created already\r\n");
                }
                App_Printf("\r\n");
                break;
            case '4':
                if(xHandleTaskLED != NULL)
                {
                    vTaskSuspend(xHandleTaskLED);
                    App_Printf("LED task suspended\r\n");
                }
                else
                {
                    App_Printf("LED task has not created already\r\n");
                }
                App_Printf("\r\n");
                break;
            case '5':
                if(xHandleTaskLED != NULL)
                {
                    vTaskResume(xHandleTaskLED);
                    App_Printf("LED task resumed\r\n");
                }
                else
                {
                    App_Printf("LED task has not created already\r\n");
                }
                App_Printf("\r\n");
                break;
            case '6':
                if(xHandleTaskLED != NULL)
                {
                    App_Printf("The LED task will be resuem from ISR after 500ms\r\n");
                    bsp_StartHardTimer(1, 50000, (void*)TMR_CallBack1);//after 500ms to execute ISR, and then disable CC1 interrupt
                    App_Printf("LED task resumed\r\n");
                }
                else
                {
                    App_Printf("LED task has not created already\r\n");
                }
                App_Printf("\r\n");
                break;
            default:
                break;
        }

        if(ucCommandValue =='1' || ucCommandValue =='2' || ucCommandValue =='3' || ucCommandValue =='4' || ucCommandValue =='5' || ucCommandValue =='6')
        {
            App_Printf("1. Print task run-time statistics\r\n");    
            App_Printf("2. Creat LED task\r\n");
            App_Printf("3. Delete LED task\r\n");
            App_Printf("4. Suspend LED task\r\n");
            App_Printf("5. Resume LED task\r\n");
            App_Printf("6. Resume LED task from ISR\r\n");
            App_Printf("\r\n");
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
        printf("There is insufficient heap memory\r\n");
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
    BaseType_t xYieldRequired;

    /* resume the suspended task */
    xYieldRequired = xTaskResumeFromISR(xHandleTaskLED);

    if(xYieldRequired == pdTRUE)
    {
        /* A context switch should now be performed so the ISR returns directly to 
         * the resumed task. This is because the resumed task had a priority that
         * was equal to or higher than the task that is currently in the Running sate.
         * NOTE: The syntax required to perform a context switch from an ISR varies from 
         * port to port, and from compiler to compiler. Check the documentation and 
         * examples for the port being used to find the syntax required by your application
         * It is likely that this if() statement can be replaced by a single call to 
         * protYIELD_FROM_ISR() [or portEND_SWITCHING_ISR()] using xYieldRequired as the macro
         * parameter: portYIELD_FROM_ISR(xYieldRequired)*/
         portYIELD_FROM_ISR(xYieldRequired);
    }
}

#if 0
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
 

