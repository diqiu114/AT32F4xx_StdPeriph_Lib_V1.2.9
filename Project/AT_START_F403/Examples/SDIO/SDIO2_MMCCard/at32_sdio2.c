/**
  ******************************************************************************
  * File   : SDIO/SDIO2_MMCCard/at32_sdio.c
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : This file provides a set of functions needed to manage the
                SDIO/MMC Card memory.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at32_sdio2.h"
#include "string.h"

/** @addtogroup AT32F403_StdPeriph_Examples
  * @{
  */

/** @addtogroup SDIO2_MMCCard
  * @{
  */ 

  
/** @addtogroup SDIO_SD_Private_Variables
  * @{
  */
SDIO_InitType SDIO_InitStructure;
SDIO_CmdInitType SDIO_CmdInitStructure;
SDIO_DataInitType SDIO_DataInitStructure;

static u8 CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1; ///< SD card type
static u32 CSD_Tab[4], CID_Tab[4], RCA = 0; ///< CSD, SID, RCA
static u8 DeviceMode = SD_POLLING_MODE; ///< working mode
static u8 StopCondition = 0; ///< transmit stop flag
volatile SD_Error TransferError = SD_OK; ///< transmit error flag
volatile u8 TransferEnd = 0; ///< transmit end flag
SD_CardInfo SDCardInfo; ///< SD card information

/**
  * @}
  */

/** @addtogroup SDIO_SD_Private_Function_Prototypes
  * @{
  */
SD_Error CmdError(void);
SD_Error CmdResp7Error(void);
SD_Error CmdResp1Error(u8 cmd);
SD_Error CmdResp3Error(void);
SD_Error CmdResp2Error(void);
SD_Error CmdResp4Error(u8 cmd);
SD_Error CmdResp5Error(u8 cmd);
SD_Error CmdResp6Error(u8 cmd, u16*prca);
SD_Error SDEnWideBus(u8 enx);
SD_Error MMC_Switch(u8 set, u8 index, u8 value);
SD_Error SD_Switch(u32 mode, u32 group, u8 value, u8 *resp);
SD_Error IsCardProgramming(u8 *pstatus);
SD_Error ChangeSpeed(u8 speed);
SD_Error FindSCR(void);
u8 convert_from_bytes_to_power_of_two(u16 NumberOfBytes);

/**
  * @}
  */

/**
  * @brief  Initializes the SD Card and put it into StandBy State (Ready for data
  *         transfer).
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_Init(void)
{
  GPIO_InitType  GPIO_InitStructure = {0};  

  u16 clkdiv = 0;
  SD_Error errorstatus = SD_OK;
  GPIO_StructInit(&GPIO_InitStructure);

  RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_SDIO2, ENABLE); //????SDIO2????
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOC | RCC_APB2PERIPH_GPIOA, ENABLE); //????GPIOA, GPIOC????

  /* ---Remap_01--- */
#ifdef SDIO2_REMAP01
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
  GPIO_PinsRemapConfig(GPIO_Remap01_SDIO2, ENABLE);

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4 | GPIO_Pins_5 | GPIO_Pins_6 | GPIO_Pins_7; //PA4~PA7-->D0~D3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //????????????
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;        //IO????????50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //??????????????????

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4 | GPIO_Pins_5; //PC4-->CK, PC5-->CMD
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //????????????
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;        //IO????????50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);                     //??????????????????

  /* ---Remap_10--- */
#elif defined SDIO2_REMAP10
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
  GPIO_PinsRemapConfig(GPIO_Remap10_SDIO2, ENABLE);

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2 | GPIO_Pins_3 | GPIO_Pins_4 | GPIO_Pins_5 | GPIO_Pins_6 | GPIO_Pins_7; //PA2-->CK, PA3-->CMD, PA4~PA7-->D4~D7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //????????????
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;        //IO????????50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //??????????????????

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_0 | GPIO_Pins_1 | GPIO_Pins_2 | GPIO_Pins_3; //PC0~PC3-->D0~D3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //????????????
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;        //IO????????50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);                     //??????????????????

  /* ---Remap_11--- */
#elif defined SDIO2_REMAP11
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
  GPIO_PinsRemapConfig(GPIO_Remap11_SDIO2, ENABLE);

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_2 | GPIO_Pins_3 | GPIO_Pins_4 | GPIO_Pins_5 | GPIO_Pins_6 | GPIO_Pins_7; //PA2-->CK, PA3-->CMD, PA4~PA7-->D0~D3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //????????????
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;        //IO????????50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //??????????????????

  /* ---No Remap--- */
#else
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_4 | GPIO_Pins_5 | GPIO_Pins_6 | GPIO_Pins_7; //PA4~PA7-->D4~D7
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //????????????
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;        //IO????????50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);                     //??????????????????

  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_0 | GPIO_Pins_1 | GPIO_Pins_2 | GPIO_Pins_3 | GPIO_Pins_4 | GPIO_Pins_5; //PC0~PC3-->D0~D3, PC4-->CK, PC5-->CMD
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;        //????????????
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;        //IO????????50MHz
  GPIO_Init(GPIOC, &GPIO_InitStructure);                     //??????????????????
#endif

  /* Reset SDIO2 */
  SDIO_Reset(SDIO2);

  errorstatus = SD_PowerON();   //????

  if(errorstatus == SD_OK)
  {
    errorstatus = SD_InitializeCards();  //??????SD??
  }

  if(errorstatus == SD_OK)
  {
    errorstatus = SD_GetCardInfo(&SDCardInfo);  //????SD??????
  }

  if(SDIO_MULTIMEDIA_CARD == CardType && SDCardInfo.SD_csd.SysSpecVersion >= 4)
  {
    CardType = SDIO_HIGH_SPEED_MULTIMEDIA_CARD;
  }

  if(errorstatus == SD_OK)
  {
    errorstatus = SD_SelectDeselect((u32)(SDCardInfo.RCA << 16));  //????SD??
  }

  if(errorstatus == SD_OK && ((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)))
  {
    errorstatus = FindSCR();
  }

  if(errorstatus == SD_OK)
  {
    errorstatus = ChangeSpeed(0);  //Set to Normal Speed
  }

  if((errorstatus == SD_OK) || (SDIO_MULTIMEDIA_CARD == CardType))
  {
    if(SDCardInfo.CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1 || SDCardInfo.CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0)
    {
      clkdiv = SystemCoreClock / 6000000; //????SD????????6MHz

      if (clkdiv > 2)
      {
        clkdiv -= 2;
      }
    }
    else
    {
      clkdiv = SystemCoreClock / 6000000; //????SD????????6MHz

      if (clkdiv > 2)
      {
        clkdiv -= 2;
      }
    }

    SDIO_Clock_Set(clkdiv); //????SDIO Clock????????
    errorstatus = SD_SetDeviceMode(SD_POLLING_MODE); //??????????????
  }

  return errorstatus;
}

/**
  * @brief  Set SDIO Clock Devision Factor.
  * @param  clkdiv: SDIO_CK = AHBCLK / (clkdiv+2)
  * @retval None
  */
void SDIO_Clock_Set(u32 clkdiv)
{
  u32 tmpreg = SDIO2->CLKCTRL;
  tmpreg &= 0XFFFE7F00;
  tmpreg |= (clkdiv & 0X000000FF);
  tmpreg |= ((clkdiv & 0X00000300) << 7);
  SDIO2->CLKCTRL = tmpreg;
}

/**
  * @brief  Enquires cards about their operating voltage and configures
  *         clock controls.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_PowerON(void)
{
  u8 i = 0;
  SD_Error errorstatus = SD_OK;
  u32 response = 0, count = 0, validvoltage = 0, clk_div;
  u32 SDType = SD_STD_CAPACITY;

  /*??????????????????????400KHz*/
  clk_div = SystemCoreClock / 200000 - 2; //??????????200KHz

  if (clk_div > 0x3FF) //9 bits
  {
    clk_div = 0X3FF;
  }

  SDIO_InitStructure.SDIO_ClkPsc = clk_div;
  SDIO_InitStructure.SDIO_ClkEdge = SDIO_ClkEdge_Falling; //??????
  SDIO_InitStructure.SDIO_ClkBypass = SDIO_ClkBypass_Disable;  //????????????
  SDIO_InitStructure.SDIO_ClkPowerSave = SDIO_ClkPowerSave_Disable; //??????????????
  SDIO_InitStructure.SDIO_BusWidth = SDIO_BusWidth_1b; //1-bit????????
  SDIO_InitStructure.SDIO_FlowCtrl = SDIO_FlowCtrl_Disable; //????????????
  SDIO_Init(SDIO2, &SDIO_InitStructure);

  SDIO_SetPowerSaveState(SDIO2, SDIO_PowerSave_ON); //????????,??????????

  SDIO_ClockCmd(SDIO2, ENABLE); //SDIOCK????

  for(i = 0; i < 10; i++)
  {
    SDIO_CmdInitStructure.SDIO_Argu = 0x0; //????CMD0????IDLE STAGE????????.
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_GO_IDLE_STATE; //CMD0
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_No; //??????
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable; //??CPSM????????????????????????????????????
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);    //??????????????????

    errorstatus = CmdError();

    if(errorstatus == SD_OK)
    {
      break;
    }
  }

  if(errorstatus)
  {
    return errorstatus;  //????????????
  }

  SDIO_CmdInitStructure.SDIO_Argu = SD_CHECK_PATTERN;   //????CMD8,??????,????SD??????????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SDIO_SEND_IF_COND;    //CMD8
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;  //R7
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;   //????????????
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

  errorstatus = CmdResp7Error(); //????R7????

  if(errorstatus == SD_OK) //R7????????
  {
    CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; //SD 2.0??
    SDType = SD_HIGH_CAPACITY; //????????
  }

  SDIO_CmdInitStructure.SDIO_Argu = 0x00; //????CMD55,??????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_APP_CMD;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);  //????CMD55,??????

  errorstatus = CmdResp1Error(SD_CMD_APP_CMD); //????R1????

  if(errorstatus == SD_OK) //SD2.0/SD 1.1,??????MMC??
  {
    /* SD??,????ACMD41 SD_APP_OP_COND,??????:0x80100000 */
    while((!validvoltage) && (count < SD_MAX_VOLT_TRIAL))
    {
      SDIO_CmdInitStructure.SDIO_Argu = 0x00; //????CMD55,??????
      SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_APP_CMD;   //CMD55
      SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);  //????CMD55,??????

      errorstatus = CmdResp1Error(SD_CMD_APP_CMD); //????R1????

      if(errorstatus != SD_OK) //????????
      {
        return errorstatus;
      }

      /* acmd41????????????????????????????HCS????????HCS????????????????SDSc????sdhc */
      SDIO_CmdInitStructure.SDIO_Argu = SD_VOLTAGE_WINDOW_SD | SDType; //????ACMD41,??????
      SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SD_APP_OP_COND;
      SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;  //r3
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

      errorstatus = CmdResp3Error(); //????R3????

      if(errorstatus != SD_OK) //????????
      {
        return errorstatus;
      }

      response = SDIO2->RSP1; //????????
      validvoltage = (((response >> 31) == 1) ? 1 : 0); //????SD??????????????
      count++;
    }

    if(count >= SD_MAX_VOLT_TRIAL)
    {
      errorstatus = SD_INVALID_VOLTRANGE;
      return errorstatus;
    }

    if(response &= SD_HIGH_CAPACITY)
    {
      CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    }
  }
  else //MMC??
  {
    /* MMC??,????CMD1 SDIO_SEND_OP_COND,??????:0x80FF8000 */
    while((!validvoltage) && (count < SD_MAX_VOLT_TRIAL))
    {
      SDIO_CmdInitStructure.SDIO_Argu = SD_VOLTAGE_WINDOW_MMC; //????CMD1,??????
      SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SEND_OP_COND;
      SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short; //r3
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

      errorstatus = CmdResp3Error(); //????R3????

      if(errorstatus != SD_OK) //????????
      {
        return errorstatus;
      }

      response = SDIO2->RSP1; //????????
      validvoltage = (((response >> 31) == 1) ? 1 : 0);
      count++;
    }

    if(count >= SD_MAX_VOLT_TRIAL)
    {
      errorstatus = SD_INVALID_VOLTRANGE;
      return errorstatus;
    }

    CardType = SDIO_MULTIMEDIA_CARD;
  }

  return(errorstatus);
}

/**
  * @brief  Turns the SDIO output signals off.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_PowerOFF(void)
{
  SDIO_SetPowerSaveState(SDIO2, SDIO_PowerSave_OFF); //SDIO????????,????????
  return SD_OK;
}

/**
  * @brief  Intialises all cards or single card as the case may be Card(s) come
  *         into standby state.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_InitializeCards(void)
{
  SD_Error errorstatus = SD_OK;
  u16 rca = 0x01;

  if(SDIO_GetPowerSaveState(SDIO2) == 0) //????????????,??????????????
  {
    return SD_REQ_NOT_APPLICABLE;
  }

  if(SDIO_SECURE_DIGITAL_IO_CARD != CardType)   //??SECURE_DIGITAL_IO_CARD
  {
    SDIO_CmdInitStructure.SDIO_Argu = 0x0; //????CMD2,????CID,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_ALL_SEND_CID;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Long;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure); //????CMD2,????CID,??????

    errorstatus = CmdResp2Error(); //????R2????
    CID_Tab[0] = SDIO2->RSP1;
    CID_Tab[1] = SDIO2->RSP2;
    CID_Tab[2] = SDIO2->RSP3;
    CID_Tab[3] = SDIO2->RSP4;
  }

  if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || \
      (SDIO_SECURE_DIGITAL_IO_COMBO_CARD == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))          //??????????
  {
    SDIO_CmdInitStructure.SDIO_Argu = 0x00; //????CMD3,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_REL_ADDR; //CMD3
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short; //r6
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);    //????CMD3,??????

    errorstatus = CmdResp6Error(SD_CMD_SET_REL_ADDR, &rca); //????R6????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }
  }

  if (SDIO_MULTIMEDIA_CARD == CardType)
  {
    SDIO_CmdInitStructure.SDIO_Argu = (u32)(rca << 16); //????CMD3,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_REL_ADDR;//CMD3
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short; //r6
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);    //????CMD3,??????

    errorstatus = CmdResp2Error(); //????R2????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }
  }

  if (SDIO_SECURE_DIGITAL_IO_CARD != CardType) //??SECURE_DIGITAL_IO_CARD
  {
    RCA = rca;
    SDIO_CmdInitStructure.SDIO_Argu = (uint32_t)(rca << 16); //????CMD9+??RCA,????CSD,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SEND_CSD;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Long;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp2Error(); //????R2????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }

    CSD_Tab[0] = SDIO2->RSP1;
    CSD_Tab[1] = SDIO2->RSP2;
    CSD_Tab[2] = SDIO2->RSP3;
    CSD_Tab[3] = SDIO2->RSP4;
  }

  return SD_OK;//????????????
}

/**
  * @brief  Returns information about specific card.
  * @param  cardinfo: pointer to a SD_CardInfo structure that contains all SD card
  *         information.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
  uint8_t tmp = 0;

  cardinfo->CardType = (uint8_t)CardType;
  cardinfo->RCA = (uint16_t)RCA;

  /*!< Byte 0 */
  tmp = (uint8_t)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  /*!< Byte 1 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /*!< Byte 2 */
  tmp = (uint8_t)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /*!< Byte 4 */
  tmp = (uint8_t)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /*!< Byte 5 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /*!< Byte 6 */
  tmp = (uint8_t)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /*!< Reserved */

  if ((CardType == SDIO_STD_CAPACITY_SD_CARD_V1_1) || (CardType == SDIO_STD_CAPACITY_SD_CARD_V2_0))
  {
    cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);
    cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;

    cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
    cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);
    cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
    cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
    cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;
    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);
    cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;

    cardinfo->CardCapacity = (cardinfo->SD_csd.DeviceSize + 1) ;
    cardinfo->CardCapacity *= (1 << (cardinfo->SD_csd.DeviceSizeMul + 2));
    cardinfo->CardBlockSize = 1 << (cardinfo->SD_csd.RdBlockLen);
    cardinfo->CardCapacity *= cardinfo->CardBlockSize;
  }
  else if (CardType == SDIO_HIGH_CAPACITY_SD_CARD)
  {
    /*!< Byte 7 */
    tmp = (uint8_t)(CSD_Tab[1] & 0x000000FF);
    cardinfo->SD_csd.DeviceSize = (tmp & 0x3F) << 16;

    /*!< Byte 8 */
    tmp = (uint8_t)((CSD_Tab[2] & 0xFF000000) >> 24);

    cardinfo->SD_csd.DeviceSize |= (tmp << 8);

    /*!< Byte 9 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x00FF0000) >> 16);

    cardinfo->SD_csd.DeviceSize |= (tmp);

    /*!< Byte 10 */
    tmp = (uint8_t)((CSD_Tab[2] & 0x0000FF00) >> 8);

    cardinfo->CardCapacity = (uint64_t)(cardinfo->SD_csd.DeviceSize + 1) * 512 * 1024;
    cardinfo->CardBlockSize = 512;
  }


  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /*!< Byte 11 */
  tmp = (uint8_t)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /*!< Byte 12 */
  tmp = (uint8_t)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /*!< Byte 13 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /*!< Byte 14 */
  tmp = (uint8_t)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /*!< Byte 15 */
  tmp = (uint8_t)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;


  /*!< Byte 0 */
  tmp = (uint8_t)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /*!< Byte 1 */
  tmp = (uint8_t)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /*!< Byte 2 */
  tmp = (uint8_t)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /*!< Byte 3 */
  tmp = (uint8_t)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /*!< Byte 4 */
  tmp = (uint8_t)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /*!< Byte 5 */
  tmp = (uint8_t)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /*!< Byte 6 */
  tmp = (uint8_t)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /*!< Byte 7 */
  tmp = (uint8_t)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /*!< Byte 8 */
  tmp = (uint8_t)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /*!< Byte 9 */
  tmp = (uint8_t)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /*!< Byte 10 */
  tmp = (uint8_t)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /*!< Byte 11 */
  tmp = (uint8_t)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /*!< Byte 12 */
  tmp = (uint8_t)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /*!< Byte 13 */
  tmp = (uint8_t)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /*!< Byte 14 */
  tmp = (uint8_t)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /*!< Byte 15 */
  tmp = (uint8_t)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;

  return(errorstatus);
}

/**
  * @brief  Enables wide bus opeartion for the requeseted card if supported by
  *         card.
  * @param  wmode: Specifies the SD card wide bus mode.
  *   This parameter can be one of the following values:
  *     @arg SDIO_BusWide_8b: 8-bit data transfer (Only for MMC)
  *     @arg SDIO_BusWide_4b: 4-bit data transfer
  *     @arg SDIO_BusWide_1b: 1-bit data transfer
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_EnableWideBusOperation(u32 wmode)
{
  SD_Error errorstatus = SD_OK;

  if((SDIO_MULTIMEDIA_CARD == CardType) || (SDIO_HIGH_SPEED_MULTIMEDIA_CARD == CardType))
  {
    errorstatus = MMC_Switch(EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, (u8)wmode);
  }
  else if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    if(wmode >= 2)
    {
      return SD_UNSUPPORTED_FEATURE;//??????8??????
    }

    errorstatus = SDEnWideBus(wmode);
  }

  if(SD_OK == errorstatus)
  {
    SDIO2->CLKCTRL &= ~(3 << 11);   //??????????????????
    SDIO2->CLKCTRL |= (u16)wmode << 11; //????????????
  }

  return errorstatus;
}

/**
  * @brief  Set SDIO Work Mode.
  * @param  Mode
            SD_DMA_MODE    :  DMA Mode.
            SD_POLLING_MODE: Polling Mode.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_SetDeviceMode(u32 Mode)
{
  SD_Error errorstatus = SD_OK;

  if((Mode == SD_DMA_MODE) || (Mode == SD_POLLING_MODE))
  {
    DeviceMode = Mode;
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;
  }

  return errorstatus;
}

/**
  * @brief  Selects od Deselects the corresponding card.
  * @param  addr: Address of the Card to be selected.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_SelectDeselect(u32 addr)
{
  SDIO_CmdInitStructure.SDIO_Argu =  addr; //????CMD7,??????,??????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SEL_DESEL_CARD;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_INT;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure); //????CMD7,??????,??????

  return CmdResp1Error(SD_CMD_SEL_DESEL_CARD);
}

/**
  * @brief  Read Data From or Write Data to SD Card.
    * @param  SDIO_CmdInitStruct: Pointer to SDIO_CmdInitType Structure.
    * @param  SDIO_DataInitStruct: Pointer to SDIO_DataInitType Structure.
    * @param  buf: Pointer to Data Buffer Read or Write.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SDIO_SendDataCommand(SDIO_CmdInitType *SDIO_CmdInitStruct, SDIO_DataInitType* SDIO_DataInitStruct, u32 *buf)
{
  SD_Error errorstatus = SD_OK;
  u32 count;
  u32 timeout = SDIO_DATATIMEOUT;
  int tlen;

  if(NULL == buf)
  {
    return SD_INVALID_PARAMETER;
  }

  SDIO2->DTCTRL = 0x0; //??????????????????(??DMA)
  SDIO_DataConfig(SDIO2, SDIO_DataInitStruct);
  SDIO_SendCommand(SDIO2, SDIO_CmdInitStruct); //????CMD17+??addr??????????????,??????
  errorstatus = CmdResp1Error(SDIO_CmdInitStruct->SDIO_CmdIdx); //????R1????

  if(errorstatus != SD_OK) //????????
  {
    return errorstatus;
  }

  tlen = SDIO_DataInitStruct->SDIO_DataLen;

  if(DeviceMode == SD_POLLING_MODE) /* ???????? */
  {
    if (SDIO_DataInitStruct->SDIO_TransferDir == SDIO_TransferDir_ToSDIO)
    {
      while(!(SDIO2->STS & (SDIO_INTR_STS_READ_MASK)))
      {
        if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_RXBUF_H) != RESET)
        {
          for(count = 0; count < 8; count++, buf++)
          {
            *buf = SDIO2->BUF;
          }

          timeout = 0X7FFFFF;
        }
        else
        {
          if(timeout == 0) //????
          {
            SD_Init();
            return SD_DATA_TIMEOUT;
          }

          timeout--;
        }
      }

      while(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_RXBUF) != RESET)
      {
        *buf = SDIO2->BUF;
        buf++;
      }
    }
    else
    {
      while(!(SDIO2->STS & SDIO_INTR_STS_WRITE_MASK))
      {
        if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_TXBUF_H) != RESET)
        {
          for(count = 0; count < 8 && tlen > 0; count++, buf++, tlen -= 4)
          {
            SDIO2->BUF = *buf;
          }

          timeout = 0X3FFFFFFF;
        }
        else
        {
          if(timeout == 0) //????
          {
            SD_Init();
            return SD_DATA_TIMEOUT;
          }

          timeout--;
        }
      }
    }

    if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_DTTIMEOUT) != RESET)  //????????
    {
      SDIO_ClearFlag(SDIO2, SDIO_FLG_DTTIMEOUT);
      return SD_DATA_TIMEOUT;
    }
    else if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_DTFAIL) != RESET) //CRC????
    {
      SDIO_ClearFlag(SDIO2, SDIO_FLG_DTFAIL);
      return SD_DATA_FAIL;
    }
    else if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_RXERRO) != RESET) //????????
    {
      SDIO_ClearFlag(SDIO2, SDIO_FLG_RXERRO);
      return SD_RX_OVERRUN;
    }
    else if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_TXERRU) != RESET) //????????
    {
      SDIO_ClearFlag(SDIO2, SDIO_FLG_TXERRU);
      return SD_TX_UNDERRUN;
    }
    else if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_SBITERR) != RESET) //??????????
    {
      SDIO_ClearFlag(SDIO2, SDIO_FLG_SBITERR);
      return SD_START_BIT_ERR;
    }

    if(StopCondition == 1 && SDIO_GetFlagStatus(SDIO2, SDIO_FLG_DTCMPL) != RESET) //????????
    {
      SDIO_CmdInitStruct->SDIO_Argu =  0; //????CMD12+????????
      SDIO_CmdInitStruct->SDIO_CmdIdx = SD_CMD_STOP_TRANSMISSION;
      SDIO_CmdInitStruct->SDIO_Resp = SDIO_Rsp_Short;
      SDIO_CmdInitStruct->SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStruct->SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(SDIO2, SDIO_CmdInitStruct);

      errorstatus = CmdResp1Error(SD_CMD_STOP_TRANSMISSION); //????R1????

      if(errorstatus != SD_OK)
      {
        return errorstatus;
      }
    }

    SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????
  }
  else if(DeviceMode == SD_DMA_MODE) /* DMA???? */
  {
    if (SDIO_DataInitStruct->SDIO_TransferDir == SDIO_TransferDir_ToSDIO)
    {
      SD_DMA_Config(buf, tlen, DMA_DIR_PERIPHERALSRC);
      SDIO2->INTEN |= SDIO_INTR_STS_READ_MASK;

    }
    else
    {
      SD_DMA_Config(buf, tlen, DMA_DIR_PERIPHERALDST);
      SDIO2->INTEN |= SDIO_INTR_STS_WRITE_MASK;
    }

    TransferError = SD_OK;
    TransferEnd = 0; //????????????????????????????1
    SDIO2->DTCTRL |= 1 << 3;

    while(!(SDIO2->STS & SDIO2->INTEN) && timeout)
    {
      timeout--;

      if (TransferEnd)
      {
        break;
      }
    }

    if(timeout == 0)
    {
      SD_Init();
      return SD_DATA_TIMEOUT;
    }

    if(TransferError != SD_OK)
    {
      errorstatus = TransferError;
    }
  }

  return errorstatus;
}

/**
  * @brief  Erase continuous data block
  * @param  addr: starting address
  * @param  nblks: amount of erasing data block
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_EraseBlocks(long long addr, u32 nblks)
{
  SD_Error errorstatus = SD_OK;
  u32 start_addr, end_addr;
  u8 cardstate;

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) //????????
  {
    start_addr = addr >> 9;
    end_addr = start_addr + nblks - 1;
  }
  else
  {
    start_addr = addr;
    end_addr = start_addr + (nblks - 1) * 512;
  }

  SDIO_DataInitStructure.SDIO_DataBlkSize = SDIO_DataBlkSize_1b; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = 0 ;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  if(SDIO2->RSP1 & SD_CARD_LOCKED) //??????
  {
    return SD_LOCK_UNLOCK_FAILED;
  }

  if(CardType == SDIO_MULTIMEDIA_CARD || CardType == SDIO_HIGH_SPEED_MULTIMEDIA_CARD)
  {
    SDIO_CmdInitStructure.SDIO_Argu =  start_addr;
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_ERASE_GRP_START; //CMD35
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_ERASE_GRP_START); //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }

    SDIO_CmdInitStructure.SDIO_Argu =  end_addr;
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_ERASE_GRP_END;//CMD36
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_ERASE_GRP_END); //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }

    SDIO_CmdInitStructure.SDIO_Argu =  0;
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_ERASE;//CMD38
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_ERASE); //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }
  }
  else
  {
    SDIO_CmdInitStructure.SDIO_Argu =  start_addr;
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SD_ERASE_GRP_START; //CMD32
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_START); //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }

    SDIO_CmdInitStructure.SDIO_Argu =  end_addr;
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SD_ERASE_GRP_END; //CMD33
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SD_ERASE_GRP_END);   //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }

    SDIO_CmdInitStructure.SDIO_Argu =  0;
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_ERASE; //CMD38
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_ERASE); //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }
  }

  errorstatus = IsCardProgramming(&cardstate);

  while((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return errorstatus;
}

/**
  * @brief  Allows to read one block from a specified address in a card. The Data
  *         transfer can be managed by DMA mode or Polling mode.
  * @note   This operation should be followed by two functions to check if the
  *         DMA Controller and SD Card status.
  *          - SD_ReadWaitOperation(): this function insure that the DMA
  *            controller has finished all data transfer.
  *          - SD_GetStatus(): to check that the SD Card has finished the
  *            data transfer and it is ready for data.
  * @param  readbuff: pointer to the buffer that will contain the received data
  * @param  ReadAddr: Address from where data are to be read.
  * @param  BlockSize: the SD card Data block size. The Block size should be 512.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_ReadBlock(u8 *readbuff, long long ReadAddr, u16 BlockSize)
{
  SD_Error errorstatus = SD_OK;
  u8 power;

  if(NULL == readbuff)
  {
    return SD_INVALID_PARAMETER;
  }

  SDIO2->DTCTRL = 0x0; //??????????????????(??DMA)

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) //????????
  {
    BlockSize = 512;
    ReadAddr >>= 9;
  }

  SDIO_DataInitStructure.SDIO_DataBlkSize = SDIO_DataBlkSize_1b; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = 0;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  if(SDIO2->RSP1 & SD_CARD_LOCKED) //??????
  {
    return SD_LOCK_UNLOCK_FAILED;
  }

  if((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    SDIO_CmdInitStructure.SDIO_Argu =  BlockSize;
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure); //????CMD16+??????????????blksize,??????

    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);   //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }
  }
  else
  {
    return SD_INVALID_PARAMETER;
  }

  SDIO_DataInitStructure.SDIO_DataBlkSize = power << 4 ; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = BlockSize ;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;

  SDIO_CmdInitStructure.SDIO_Argu =  ReadAddr;
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_READ_SINGLE_BLOCK;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;

  StopCondition = 0;
  return SDIO_SendDataCommand(&SDIO_CmdInitStructure, &SDIO_DataInitStructure, (u32 *)readbuff);
}

/**
  * @brief  Allows to read blocks from a specified address  in a card.  The Data
  *         transfer can be managed by DMA mode or Polling mode.
  * @note   This operation should be followed by two functions to check if the
  *         DMA Controller and SD Card status.
  *          - SD_ReadWaitOperation(): this function insure that the DMA
  *            controller has finished all data transfer.
  *          - SD_GetStatus(): to check that the SD Card has finished the
  *            data transfer and it is ready for data.
  * @param  readbuff: pointer to the buffer that will contain the received data.
  * @param  ReadAddr: Address from where data are to be read.
  * @param  BlockSize: the SD card Data block size. The Block size should be 512.
  * @param  NumberOfBlocks: number of blocks to be read.
  * @retval SD_Error: SD Card Error code.
  */
__align(4) u32 *tempbuff;
SD_Error SD_ReadMultiBlocks(u8 *readbuff, long long ReadAddr, u16 BlockSize, u32 NumberOfBlocks)
{
  SD_Error errorstatus = SD_OK;
  u8 power;

  SDIO2->DTCTRL = 0x0; //??????????????????(??DMA)

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) //????????
  {
    BlockSize = 512;
    ReadAddr >>= 9;
  }

  SDIO_DataInitStructure.SDIO_DataBlkSize = 0; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = 0;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  if(SDIO2->RSP1 & SD_CARD_LOCKED) //??????
  {
    return SD_LOCK_UNLOCK_FAILED;
  }

  if((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    SDIO_CmdInitStructure.SDIO_Argu =  BlockSize; //????CMD16+??????????????blksize,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);   //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }
  }
  else
  {
    return SD_INVALID_PARAMETER;
  }

  if(NumberOfBlocks * BlockSize > SD_MAX_DATA_LENGTH) //????????????????????????
  {
    return SD_INVALID_PARAMETER;
  }

  SDIO_DataInitStructure.SDIO_DataBlkSize = power << 4; //nblks*blksize,512??????,??????????
  SDIO_DataInitStructure.SDIO_DataLen = NumberOfBlocks * BlockSize ;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;

  SDIO_CmdInitStructure.SDIO_Argu =  ReadAddr;//????CMD18+??addr??????????????,??????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_READ_MULT_BLOCK;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;

  StopCondition = 1;
  return SDIO_SendDataCommand(&SDIO_CmdInitStructure, &SDIO_DataInitStructure, (u32 *)readbuff);
}

/**
  * @brief  Allows to write one block starting from a specified address in a card.
  *         The Data transfer can be managed by DMA mode or Polling mode.
  * @note   This operation should be followed by two functions to check if the
  *         DMA Controller and SD Card status.
  *          - SD_ReadWaitOperation(): this function insure that the DMA
  *            controller has finished all data transfer.
  *          - SD_GetStatus(): to check that the SD Card has finished the
  *            data transfer and it is ready for data.
  * @param  writebuff: pointer to the buffer that contain the data to be transferred.
  * @param  WriteAddr: Address from where data are to be read.
  * @param  BlockSize: the SD card Data block size. The Block size should be 512.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_WriteBlock(u8 *writebuff, long long WriteAddr, u16 BlockSize)
{
  SD_Error errorstatus = SD_OK;
  u8  power = 0, cardstate = 0;
  u32 timeout = 0;
  u32 cardstatus = 0;

  if(writebuff == NULL) //????????
  {
    return SD_INVALID_PARAMETER;
  }

  SDIO2->DTCTRL = 0x0; //??????????????????(??DMA)

  SDIO_DataInitStructure.SDIO_DataBlkSize = 0; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = 0;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  if(SDIO2->RSP1 & SD_CARD_LOCKED) //??????
  {
    return SD_LOCK_UNLOCK_FAILED;
  }

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) //????????
  {
    BlockSize = 512;
    WriteAddr >>= 9;
  }

  if((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    SDIO_CmdInitStructure.SDIO_Argu = BlockSize; //????CMD16+??????????????blksize,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);   //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }

  }
  else
  {
    return SD_INVALID_PARAMETER;
  }

  timeout = SD_DATATIMEOUT;

  do
  {
    timeout--;
    errorstatus = SD_SendStatus(&cardstatus);
  }
  while(((cardstatus & 0x00000100) == 0) && (timeout > 0)); //????READY_FOR_DATA??????????

  if(timeout == 0)
  {
    return SD_ERROR;
  }

  SDIO_CmdInitStructure.SDIO_Argu = WriteAddr; //????CMD24,??????????,??????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_WRITE_SINGLE_BLOCK;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;

  SDIO_DataInitStructure.SDIO_DataBlkSize = power << 4; //blksize, ??????????
  SDIO_DataInitStructure.SDIO_DataLen = BlockSize;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;

  StopCondition = 0; //??????,??????????????????????
  errorstatus = SDIO_SendDataCommand(&SDIO_CmdInitStructure, &SDIO_DataInitStructure, (u32 *)writebuff);

  if(errorstatus != SD_OK)
  {
    return errorstatus;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????
  errorstatus = IsCardProgramming(&cardstate);

  while((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return errorstatus;
}

/**
  * @brief  Allows to write blocks starting from a specified address in a card.
  *         The Data transfer can be managed by DMA mode only.
  * @note   This operation should be followed by two functions to check if the
  *         DMA Controller and SD Card status.
  *          - SD_ReadWaitOperation(): this function insure that the DMA
  *            controller has finished all data transfer.
  *          - SD_GetStatus(): to check that the SD Card has finished the
  *            data transfer and it is ready for data.
  * @param  writebuff: pointer to the buffer that contain the data to be transferred.
  * @param  WriteAddr: Address from where data are to be read.
  * @param  BlockSize: the SD card Data block size. The Block size should be 512.
  * @param  NumberOfBlocks: number of blocks to be written.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_WriteMultiBlocks(u8 *writebuff, long long WriteAddr, u16 BlockSize, u32 NumberOfBlocks)
{
  SD_Error errorstatus = SD_OK;
  u8  power = 0, cardstate = 0;
  u32 timeout = 0;
  u32 cardstatus = 0;

  if(writebuff == NULL)
  {
    return SD_INVALID_PARAMETER;
  }

  SDIO2->DTCTRL = 0x0; //??????????????????(??DMA)

  SDIO_DataInitStructure.SDIO_DataBlkSize = 0; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = 0;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  if(SDIO2->RSP1 & SD_CARD_LOCKED) //??????
  {
    return SD_LOCK_UNLOCK_FAILED;
  }

  if(CardType == SDIO_HIGH_CAPACITY_SD_CARD) //????????
  {
    BlockSize = 512;
    WriteAddr >>= 9;
  }

  if((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
  {
    power = convert_from_bytes_to_power_of_two(BlockSize);

    SDIO_CmdInitStructure.SDIO_Argu = BlockSize;//????CMD16+??????????????blksize,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_BLOCKLEN;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);   //????R1????

    if(errorstatus != SD_OK) //????????
    {
      return errorstatus;
    }

  }
  else
  {
    return SD_INVALID_PARAMETER;
  }

  if(NumberOfBlocks * BlockSize > SD_MAX_DATA_LENGTH)
  {
    return SD_INVALID_PARAMETER;
  }

  timeout = SD_DATATIMEOUT;

  do
  {
    timeout--;
    errorstatus = SD_SendStatus(&cardstatus);
  }
  while(((cardstatus & 0x00000100) == 0) && (timeout > 0)); //????READY_FOR_DATA??????????

  if(timeout == 0)
  {
    return SD_ERROR;
  }

  if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType))
  {
    SDIO_CmdInitStructure.SDIO_Argu = (u32)RCA << 16;   //????ACMD55
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_APP_CMD;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_APP_CMD); //????R1????

    if(errorstatus != SD_OK)
    {
      return errorstatus;
    }

    SDIO_CmdInitStructure.SDIO_Argu = NumberOfBlocks; //????CMD23
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_BLOCK_COUNT;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_SET_BLOCK_COUNT); //????R1????

    if(errorstatus != SD_OK)
    {
      return errorstatus;
    }
  }

  SDIO_CmdInitStructure.SDIO_Argu = WriteAddr;  //????CMD25
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_WRITE_MULT_BLOCK;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;

  SDIO_DataInitStructure.SDIO_DataBlkSize = power << 4; //blksize, ??????????
  SDIO_DataInitStructure.SDIO_DataLen = NumberOfBlocks * BlockSize ;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  StopCondition = 1;    //CMD12 is needed
  errorstatus = SDIO_SendDataCommand(&SDIO_CmdInitStructure, &SDIO_DataInitStructure, (u32 *)writebuff);

  if(errorstatus != SD_OK)
  {
    return errorstatus;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS);
  errorstatus = IsCardProgramming(&cardstate);

  while((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return errorstatus;
}

/**
  * @brief  Read MMC card data stream.
  * @param  buf: buffer of saving data from MMC card
  * @param  addr: start address of data from MMC card
  * @param  tlen: data size
  * @retval SD_Error: SD Card Error code.
  */
SD_Error MMC_StreamRead(u8 *buf, long long addr, u32 tlen)
{
  SDIO2->DTCTRL = 0x0; //??????????????????(??DMA)

  SDIO_DataInitStructure.SDIO_DataBlkSize = 0; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = 0;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Stream;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  if(SDIO2->RSP1 & SD_CARD_LOCKED)
  {
    return SD_LOCK_UNLOCK_FAILED;  //??????
  }

  SDIO_CmdInitStructure.SDIO_Argu =  addr; //????CMD11+??addr??????????????,??????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_READ_DAT_UNTIL_STOP;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;

  SDIO_DataInitStructure.SDIO_DataBlkSize = 5 << 4;
  SDIO_DataInitStructure.SDIO_DataLen = tlen;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Stream;

  StopCondition = 1;    //CMD12 is needed
  return SDIO_SendDataCommand(&SDIO_CmdInitStructure, &SDIO_DataInitStructure, (u32 *)buf);
}

/**
  * @brief  Read MMC card data stream.
  * @param  buf: data that writing to MMC card
  * @param  addr: start address MMC card
  * @param  tlen: data size
  * @retval SD_Error: SD Card Error code.
  */
SD_Error MMC_StreamWrite(u8 *buf, long long addr, u32 tlen)
{
  SD_Error errorstatus = SD_OK;
  u8 cardstate = 0;

  if(buf == NULL)
  {
    return SD_INVALID_PARAMETER;  //????????
  }

  SDIO2->DTCTRL = 0x0;                          //??????????????????(??DMA)

  SDIO_DataInitStructure.SDIO_DataBlkSize = 0; ;    //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = 0 ;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Stream;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  if(SDIO2->RSP1 & SD_CARD_LOCKED)
  {
    return SD_LOCK_UNLOCK_FAILED;  //??????
  }

  SDIO_CmdInitStructure.SDIO_Argu = addr;   //????CMD20,??????????,??????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_WRITE_DAT_UNTIL_STOP;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;

  SDIO_DataInitStructure.SDIO_DataBlkSize = 15 << 4;
  SDIO_DataInitStructure.SDIO_DataLen = tlen;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToCard;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Stream;

  StopCondition = 1;    //CMD12 is needed
  errorstatus = SDIO_SendDataCommand(&SDIO_CmdInitStructure, &SDIO_DataInitStructure, (u32 *)buf);

  if(errorstatus != SD_OK)
  {
    return errorstatus;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????
  errorstatus = IsCardProgramming(&cardstate);

  while((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)))
  {
    errorstatus = IsCardProgramming(&cardstate);
  }

  return errorstatus;
}

/**
  * @brief  SDIO2 ISR.
  * @param  None.
  * @retval None.
  */
void SDIO2_IRQHandler(void)
{
  SD_ProcessIRQSrc();//????????SDIO????????
}

/**
  * @brief  Allows to process all the interrupts that are high.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_ProcessIRQSrc(void)
{
  if(SDIO2->STS & (1 << 8)) //????????????
  {
    if (StopCondition == 1)
    {
      SDIO_CmdInitStructure.SDIO_Argu = 0; //????CMD12+????????
      SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_STOP_TRANSMISSION;
      SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
      SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
      SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
      SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

      TransferError = CmdResp1Error(SD_CMD_STOP_TRANSMISSION);
    }
    else
    {
      TransferError = SD_OK;
    }

    SDIO2->INTCLR |= 1 << 8;
    SDIO2->INTEN &= ~((1 << 1) | (1 << 3) | (1 << 8) | (1 << 14) | (1 << 15) | (1 << 4) | (1 << 5) | (1 << 9)); //??????
    TransferEnd = 1;
    return(TransferError);
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_DTFAIL) != RESET) //????CRC????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_DTFAIL); //??????????
    SDIO2->INTEN &= ~((1 << 1) | (1 << 3) | (1 << 8) | (1 << 14) | (1 << 15) | (1 << 4) | (1 << 5) | (1 << 9));
    TransferError = SD_DATA_FAIL;
    return(SD_DATA_FAIL);
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_DTTIMEOUT) != RESET) //????????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_DTTIMEOUT); //??????????
    SDIO2->INTEN &= ~((1 << 1) | (1 << 3) | (1 << 8) | (1 << 14) | (1 << 15) | (1 << 4) | (1 << 5) | (1 << 9)); //??????
    TransferError = SD_DATA_TIMEOUT;
    return(SD_DATA_TIMEOUT);
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_RXERRO) != RESET) //FIFO????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_RXERRO);             //??????????
    SDIO2->INTEN &= ~((1 << 1) | (1 << 3) | (1 << 8) | (1 << 14) | (1 << 15) | (1 << 4) | (1 << 5) | (1 << 9)); //??????
    TransferError = SD_RX_OVERRUN;
    return(SD_RX_OVERRUN);
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_TXERRU) != RESET) //FIFO????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_TXERRU);             //??????????
    SDIO2->INTEN &= ~((1 << 1) | (1 << 3) | (1 << 8) | (1 << 14) | (1 << 15) | (1 << 4) | (1 << 5) | (1 << 9)); //??????
    TransferError = SD_TX_UNDERRUN;
    return(SD_TX_UNDERRUN);
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_SBITERR) != RESET) //??????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_SBITERR); //??????????
    SDIO2->INTEN &= ~((1 << 1) | (1 << 3) | (1 << 8) | (1 << 14) | (1 << 15) | (1 << 4) | (1 << 5) | (1 << 9)); //??????
    TransferError = SD_START_BIT_ERR;
    return(SD_START_BIT_ERR);
  }

  return(SD_OK);
}

/**
  * @brief  Checks for error conditions for CMD0.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdError(void)
{
  SD_Error errorstatus = SD_OK;
  u32 timeout = SDIO_CMD0TIMEOUT;

  while(timeout--)
  {
    if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDCMPL) != RESET)
    {
      break;
    }
  }

  if(timeout == 0)
  {
    return SD_CMD_RSP_TIMEOUT;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????
  return errorstatus;
}

/**
  * @brief  Checks for error conditions for R7 response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdResp7Error(void)
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 timeout = SDIO_CMD0TIMEOUT;

  while(timeout--)
  {
    status = SDIO2->STS;

    if(status & ((1 << 0) | (1 << 2) | (1 << 6))) //CRC????/????????????/????????????
    {
      break;
    }
  }

  if((timeout == 0) || (status & (1 << 2))) //????????
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return errorstatus;
  }

  if(status & 1 << 6)   //????????
  {
    errorstatus = SD_OK;
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDRSPCMPL);
  }

  return errorstatus;
}

/**
  * @brief  Checks for error conditions for R1 response.
  * @param  cmd: The sent command index.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdResp1Error(u8 cmd)
{
  u32 status;

  while(1)
  {
    status = SDIO2->STS;

    if(status & ((1 << 0) | (1 << 2) | (1 << 6))) //CRC????/????????????/????????????
    {
      break;
    }
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDTIMEOUT) != RESET) //????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return SD_CMD_RSP_TIMEOUT;
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDFAIL) != RESET)  //CRC????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDFAIL);
    return SD_CMD_FAIL;
  }

  if(SDIO2->RSPCMD != cmd)
  {
    return SD_ILLEGAL_CMD;  //??????????
  }

  SDIO2->INTCLR = 0X5FF;
  return (SD_Error)(SDIO2->RSP1 & SD_OCR_ERRORBITS);
}

/**
  * @brief  Checks for error conditions for R3 (OCR) response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdResp3Error(void)
{
  u32 status;

  while(1)
  {
    status = SDIO2->STS;

    if(status & ((1 << 0) | (1 << 2) | (1 << 6))) //CRC????/????????????/????????????
    {
      break;
    }
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDTIMEOUT) != RESET) //????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return SD_CMD_RSP_TIMEOUT;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS);
  return SD_OK;
}

/**
  * @brief  Checks for error conditions for R2 (CID or CSD) response.
  * @param  None
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdResp2Error(void)
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 timeout = SDIO_CMD0TIMEOUT;

  while(timeout--)
  {
    status = SDIO2->STS;

    if(status & ((1 << 0) | (1 << 2) | (1 << 6))) //CRC????/????????????/????????????
    {
      break;
    }
  }

  if((timeout == 0) || (status & (1 << 2))) //????????
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return errorstatus;
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDFAIL) != RESET)  //CRC????
  {
    errorstatus = SD_CMD_FAIL;
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDFAIL);
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????
  return errorstatus;
}

/**
  * @brief  Checks for error conditions for R4 response.
  * @param  cmd: The sent command index.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdResp4Error(u8 cmd)
{
  u32 status;

  while(1)
  {
    status = SDIO2->STS;

    if(status & ((1 << 0) | (1 << 2) | (1 << 6))) //CRC????/????????????/????????????
    {
      break;
    }
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDTIMEOUT) != RESET)   //????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return SD_CMD_RSP_TIMEOUT;
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDFAIL) != RESET)  //CRC????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDFAIL);
    return SD_CMD_FAIL;
  }

  if(SDIO2->RSPCMD != cmd) //??????????
  {
    return SD_ILLEGAL_CMD;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????

  return SD_OK;
}

/**
  * @brief  Checks for error conditions for R5 response.
  * @param  cmd: The sent command index.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdResp5Error(u8 cmd)
{
  u32 status;

  while(1)
  {
    status = SDIO2->STS;

    if(status & ((1 << 0) | (1 << 2) | (1 << 6))) //CRC????/????????????/????????????
    {
      break;
    }
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDTIMEOUT) != RESET) //????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return SD_CMD_RSP_TIMEOUT;
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDFAIL) != RESET)  //CRC????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDFAIL);
    return SD_CMD_FAIL;
  }

  if(SDIO2->RSPCMD != cmd) //??????????
  {
    return SD_ILLEGAL_CMD;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS);

  if (SDIO2->RSP1 & SD_R5_OUT_OF_RANGE)
  {
    return SD_CMD_OUT_OF_RANGE;
  }

  if (SDIO2->RSP1 & SD_R5_FUNCTION_NUMBER)
  {
    return SD_SDIO_UNKNOWN_FUNC;
  }

  if (SDIO2->RSP1 & SD_R5_ERROR)
  {
    return SD_GENERAL_UNKNOWN_ERROR;
  }

  return SD_OK;
}

/**
  * @brief  Checks for error conditions for R6 (RCA) response.
  * @param  cmd: The sent command index.
  * @param  prca: pointer to the variable that will contain the SD card relative
  *         address RCA.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error CmdResp6Error(u8 cmd, u16*prca)
{
  SD_Error errorstatus = SD_OK;
  u32 status;
  u32 rspr1;

  while(1)
  {
    status = SDIO2->STS;

    if(status & ((1 << 0) | (1 << 2) | (1 << 6))) //CRC????/????????????/????????????
    {
      break;
    }
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDTIMEOUT) != RESET) //????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return SD_CMD_RSP_TIMEOUT;
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDFAIL) != RESET)  //CRC????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDFAIL);
    return SD_CMD_FAIL;
  }

  if(SDIO2->RSPCMD != cmd) //????????????cmd????
  {
    return SD_ILLEGAL_CMD;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????

  rspr1 = SDIO2->RSP1;

  if(SD_ALLZERO == (rspr1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_CMD_CRC_FAILED)))
  {
    *prca = (u16)(rspr1 >> 16);
    return errorstatus;
  }

  if(rspr1 & SD_R6_GENERAL_UNKNOWN_ERROR)
  {
    return SD_GENERAL_UNKNOWN_ERROR;
  }

  if(rspr1 & SD_R6_ILLEGAL_CMD)
  {
    return SD_ILLEGAL_CMD;
  }

  if(rspr1 & SD_R6_CMD_CRC_FAILED)
  {
    return SD_CMD_CRC_FAILED;
  }

  return errorstatus;
}

/**
  * @brief  Enables or disables the SDIO wide bus mode.
  * @param  enx: new state of the SDIO wide bus mode.
  *         This parameter can be: ENABLE or DISABLE.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SDEnWideBus(u8 enx)
{
  SD_Error errorstatus = SD_OK;
  u8 arg = 0X00;

  if(enx)
  {
    arg = 0X02;
  }
  else
  {
    arg = 0X00;
  }

  if(SDIO2->RSP1 & SD_CARD_LOCKED) //SD??????LOCKED????
  {
    return SD_LOCK_UNLOCK_FAILED;
  }

  if(SDCardInfo.SD_scr.SD_BUS_WIDTHS)   //??????????
  {
    SDIO_CmdInitStructure.SDIO_Argu = (uint32_t) SDCardInfo.RCA << 16;//????CMD55+RCA,??????
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_APP_CMD;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

    if(errorstatus != SD_OK)
    {
      return errorstatus;
    }

    SDIO_CmdInitStructure.SDIO_Argu = arg; //????ACMD6,??????,????:10,4??;00,1??.
    SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_APP_SD_SET_BUSWIDTH;
    SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
    SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
    SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
    SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

    errorstatus = CmdResp1Error(SD_CMD_APP_SD_SET_BUSWIDTH);
    return errorstatus;
  }
  else
  {
    return SD_REQ_NOT_APPLICABLE;  //????????????????
  }
}

/**
  * @brief  Switch MMC card speed to high speed
  * @param  set: new state of the SDIO wide bus mode.
  * @param  index: offset
  * @param  value: wanted speed
  * @retval SD_Error: SD Card Error code.
  */
SD_Error MMC_Switch(u8 set, u8 index, u8 value)
{
  SD_Error errorstatus = SD_OK;
  u32 cardstatus = 0;
  u32 timeout = 0;

  SDIO_CmdInitStructure.SDIO_Argu = (EXT_CSD_Write_byte << 24) | (index << 16) | (value << 8) | set;
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_HS_SWITCH;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_HS_SWITCH);    //????R1????

  if(errorstatus != SD_OK)
  {
    ;
  }

  cardstatus = SDIO2->RSP1;

  if(cardstatus & MMC_SWITCH_ERROR)
  {
    return SD_SWITCH_ERROR;
  }

  timeout = SD_DATATIMEOUT;

  do
  {
    timeout--;
    errorstatus = SD_SendStatus(&cardstatus);
  }
  while(((cardstatus & 0x00000100) == 0) && (timeout > 0)); //????READY_FOR_DATA??????????

  if(timeout == 0)
  {
    return SD_ERROR;
  }

  return errorstatus;
}

/**
  * @brief  Switch SD card speed to high speed
  * @param  mode: polling mode or DMA mode
  * @param  group: selected group
  * @param  value: wanted speed
  * @param  resp: switch status
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_Switch(u32 mode, u32 group, u8 value, u8 *resp)
{
  SD_Error errorstatus = SD_OK;
  u8 power;
  u16 blksize;

  if(NULL == resp)
  {
    return SD_INVALID_PARAMETER;
  }

  SDIO2->DTCTRL = 0x0;  //??????????????????(??DMA)

  blksize = 64;
  power = convert_from_bytes_to_power_of_two(blksize);

  SDIO_CmdInitStructure.SDIO_Argu =  blksize;
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_BLOCKLEN;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure); //????CMD16+??????????????blksize,??????

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN); //????R1????

  if(errorstatus != SD_OK)
  {
    return errorstatus;  //????????
  }

  SDIO_DataInitStructure.SDIO_DataBlkSize = power << 4 ; //????DPSM??????????
  SDIO_DataInitStructure.SDIO_DataLen = blksize ;
  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT ;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;

  SDIO_CmdInitStructure.SDIO_Argu = mode << 31 | 0x00FFFFFF;
  SDIO_CmdInitStructure.SDIO_Argu &= ~(0xF << (group * 4));
  SDIO_CmdInitStructure.SDIO_Argu |= value << (group * 4);
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_HS_SWITCH;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;

  StopCondition = 0;
  return SDIO_SendDataCommand(&SDIO_CmdInitStructure, &SDIO_DataInitStructure, (u32 *)resp);
}


/**
  * @brief  Checks if the SD card is in programming state.
  * @param  pstatus: pointer to the variable that will contain the SD card state.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error IsCardProgramming(u8 *pstatus)
{
  vu32 respR1 = 0, status = 0;

  SDIO_CmdInitStructure.SDIO_Argu = (uint32_t) RCA << 16; //??????????????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SEND_STATUS;//????CMD13
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

  status = SDIO2->STS;

  while(!(status & ((1 << 0) | (1 << 6) | (1 << 2))))
  {
    status = SDIO2->STS;  //????????????
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDFAIL) != RESET)  //CRC????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDFAIL);
    return SD_CMD_FAIL;
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_CMDTIMEOUT) != RESET)   //????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_CMDTIMEOUT);
    return SD_CMD_RSP_TIMEOUT;
  }

  if(SDIO2->RSPCMD != SD_CMD_SEND_STATUS)
  {
    return SD_ILLEGAL_CMD;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS);

  respR1 = SDIO2->RSP1;

  *pstatus = (u8)((respR1 >> 9) & 0x0000000F);

  return SD_OK;
}

/**
  * @brief  Read current card status.
  * @param  pcardstatus: Card status.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error SD_SendStatus(u32 *pcardstatus)
{
  SD_Error errorstatus = SD_OK;

  if(pcardstatus == NULL)
  {
    errorstatus = SD_INVALID_PARAMETER;
    return errorstatus;
  }

  SDIO_CmdInitStructure.SDIO_Argu = (uint32_t) RCA << 16;//????CMD13,??????
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SEND_STATUS;
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);
  errorstatus = CmdResp1Error(SD_CMD_SEND_STATUS);  //????????????

  if(errorstatus != SD_OK)
  {
    return errorstatus;
  }

  *pcardstatus = SDIO2->RSP1; //??????????

  return errorstatus;
}

/**
  * @brief  Returns information about SD card.
  * @param  None.
  * @retval SDCardState: SD Card State code.
  */
SDCardState SD_GetState(void)
{
  u32 resp1 = 0;

  if(SD_SendStatus(&resp1) != SD_OK)
  {
    return SD_CARD_ERROR;
  }
  else
  {
    return (SDCardState)((resp1 >> 9) & 0x0F);
  }
}

/**
  * @brief  Find the SD card SCR register value.
  * @param  rca: selected card address.
  * @param  pscr: pointer to the buffer that will contain the SCR value.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error FindSCR(void)
{
  u32 index = 0;
  SD_Error errorstatus = SD_OK;
  u32 *tempscr;

  tempscr = (u32 *) & (SDCardInfo.SD_scr);
  SDIO_CmdInitStructure.SDIO_Argu = (uint32_t)8;     //????CMD16
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SET_BLOCKLEN; //cmd16
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;  //r1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SET_BLOCKLEN);

  if(errorstatus != SD_OK)
  {
    return errorstatus;
  }

  SDIO_CmdInitStructure.SDIO_Argu = (uint32_t) SDCardInfo.RCA << 16;
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_APP_CMD;//????CMD55
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_APP_CMD);

  if(errorstatus != SD_OK)
  {
    return errorstatus;
  }

  SDIO_DataInitStructure.SDIO_DataTimeOut = SD_DATATIMEOUT;
  SDIO_DataInitStructure.SDIO_DataLen = 8;
  SDIO_DataInitStructure.SDIO_DataBlkSize = SDIO_DataBlkSize_8b;  //??????8byte
  SDIO_DataInitStructure.SDIO_TransferDir = SDIO_TransferDir_ToSDIO;
  SDIO_DataInitStructure.SDIO_TransferMode = SDIO_TransferMode_Block;
  SDIO_DataInitStructure.SDIO_DPSM = SDIO_DPSM_Enable;
  SDIO_DataConfig(SDIO2, &SDIO_DataInitStructure);

  SDIO_CmdInitStructure.SDIO_Argu = 0x0;
  SDIO_CmdInitStructure.SDIO_CmdIdx = SD_CMD_SD_APP_SEND_SCR;   //????ACMD51
  SDIO_CmdInitStructure.SDIO_Resp = SDIO_Rsp_Short;  //r1
  SDIO_CmdInitStructure.SDIO_Wait = SDIO_Wait_No;
  SDIO_CmdInitStructure.SDIO_CPSM = SDIO_CPSM_Enable;
  SDIO_SendCommand(SDIO2, &SDIO_CmdInitStructure);

  errorstatus = CmdResp1Error(SD_CMD_SD_APP_SEND_SCR);

  if(errorstatus != SD_OK)
  {
    return errorstatus;
  }

  while(!(SDIO2->STS & (SDIO_FLG_RXERRO | SDIO_FLG_DTFAIL | SDIO_FLG_DTTIMEOUT | SDIO_FLG_DTBLKCMPL | SDIO_FLG_SBITERR)))
  {
    if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_RXBUF) != RESET) //????FIFO????????
    {
      *(tempscr + index) = SDIO_ReadData(SDIO2);    //????FIFO????
      index++;
    }
  }

  if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_DTTIMEOUT) != RESET)    //????????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_DTTIMEOUT);
    return SD_DATA_TIMEOUT;
  }
  else if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_DTFAIL) != RESET)  //??????CRC????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_DTFAIL);
    return SD_DATA_FAIL;
  }
  else if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_RXERRO) != RESET)  //????fifo????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_RXERRO);
    return SD_RX_OVERRUN;
  }
  else if(SDIO_GetFlagStatus(SDIO2, SDIO_FLG_SBITERR) != RESET)     //??????????????
  {
    SDIO_ClearFlag(SDIO2, SDIO_FLG_SBITERR);
    return SD_START_BIT_ERR;
  }

  SDIO_ClearFlag(SDIO2, SDIO_STATIC_FLAGS); //????????????

  return errorstatus;
}

/**
  * @brief  Set bus speed
  * @param  speed: 0,Normal speed; 1,High speed.
  * @retval SD_Error: SD Card Error code.
  */
SD_Error ChangeSpeed(u8 speed)
{
  SD_Error errorstatus = SD_OK;
  u8 SwitchSts[64];

  if (speed > 1)
  {
    return SD_ERROR;
  }

  if((SDIO_STD_CAPACITY_SD_CARD_V1_1 == CardType) || (SDIO_STD_CAPACITY_SD_CARD_V2_0 == CardType) || (SDIO_HIGH_CAPACITY_SD_CARD == CardType)) //??????????
  {
    /* Version 1.01 Card does not support CMD6 */
    if (SDCardInfo.SD_scr.SD_SPEC == 0)
    {
      return SD_ERROR;
    }

    /* Check Group 1 function support speed*/
    errorstatus = SD_Switch(0, 0, speed, SwitchSts);

    if (errorstatus != 0)
    {
      return errorstatus;
    }

    if ((SwitchSts[13] & (1 << speed)) == 0)
    {
      return SD_ERROR;
    }

    errorstatus = SD_Switch(1, 0, speed, SwitchSts);

    if (errorstatus != 0)
    {
      return errorstatus;
    }

    /* Read it back for confirmation */
    if ((SwitchSts[16] & 0xF) != speed)
    {
      return SD_ERROR;
    }
  }
  else if(SDIO_HIGH_SPEED_MULTIMEDIA_CARD == CardType)
  {
    errorstatus = MMC_Switch(EXT_CSD_CMD_SET_NORMAL, EXT_CSD_BUS_WIDTH, (u8)speed);

    if (errorstatus != 0)
    {
      return errorstatus;
    }
  }

  return errorstatus;
}

/**
  * @brief  Converts the number of bytes in power of two and returns the power.
  * @param  NumberOfBytes: number of bytes.
  * @retval None
  */
u8 convert_from_bytes_to_power_of_two(u16 NumberOfBytes)
{
  u8 count = 0;

  while(NumberOfBytes != 1)
  {
    NumberOfBytes >>= 1;
    count++;
  }

  return count;
}

/**
  * @brief  Set DMA configuation for SDIO
  * @param  mbuf buffer address
  * @param  bufsize transmission data size
  * @param  DMA_DIR DMA direction, it is DMA_DIR_PERIPHERALDST(writing data) or DMA_DIR_PERIPHERALSRC(read data)
  * @retval None
  */
void SD_DMA_Config(u32*mbuf, u32 bufsize, u32 DMA_DIR)
{
  DMA_InitType DMA_InitStructure;

  RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA2, ENABLE); //????DMA2????

  DMA_Reset(DMA2_Channel5); //????DMA2????5
  DMA_ChannelEnable(DMA2_Channel5, DISABLE); //????DMA2????5

  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&SDIO2->BUF;  //DMA??????????
  DMA_InitStructure.DMA_MemoryBaseAddr = (u32)mbuf;  //DMA??????????
  DMA_InitStructure.DMA_Direction = DMA_DIR;  //??????????????????????????????????
  DMA_InitStructure.DMA_BufferSize = bufsize / 4; //DMA??????DMA??????????
  DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;  //??????????????????
  DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;  //??????????????????
  DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_WORD;  //??????????32??
  DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_WORD; //??????????32??
  DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;  //??????????????????
  DMA_InitStructure.DMA_Priority = DMA_PRIORITY_HIGH; //DMA???? x????????????
  DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;  //DMA????x????????????????????????
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);

  DMA_ChannelEnable(DMA2_Channel5, ENABLE); //????DMA2 ????5
}


/**
  * @}
  */

/**
  * @}
  */


