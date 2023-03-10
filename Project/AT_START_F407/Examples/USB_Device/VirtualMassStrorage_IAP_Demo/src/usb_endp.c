/**
  ******************************************************************************
  * File   : USB_Device/VirtualMassStorage_IAP_Demo/src/usb_endp.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : Endpoint routines
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_bot.h"
#include "usb_istr.h"
/** @addtogroup AT32F407_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_VirtualMassStrorage_IAP_Demo
  * @{
  */ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EP1 IN Callback Routine.
  * @param  None.
  * @retval None.
  */
void EP1_IN_Callback(void)
{
  Mass_Storage_In();
}

/**
  * @brief  EP2 OUT Callback Routine.
  * @param  None.
  * @retval None.
  */
void EP2_OUT_Callback(void)
{
  Mass_Storage_Out();
}
/**
  * @}
  */ 

/**
  * @}
  */



