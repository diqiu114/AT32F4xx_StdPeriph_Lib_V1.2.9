#include "spi_dma.h"
#include "at32f4xx.h"
#include "../lcd/st7789.h"
#include "stdio.h"

/*=================硬件加速 开始===================*/
static uint8_t* Disp_DMA_TragetPoint = NULL;
static uint8_t* Disp_DMA_CurrentPoint = NULL;

/*=================硬件加速 结束===================*/


/*===============spi2Init start=================*/
static void Dspi2RCC_Configuration()
{
    /* Enable DSPI2 clock and GPIO clock for DSPI2 and DSPI2 */
    RCC_APB2PeriphClockCmd(DSPI2_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO, ENABLE);
    /* Enable DSPI2 Periph clock */
    RCC_APB1PeriphClockCmd(DSPI2_CLK, ENABLE);
}

static void Dspi2GpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Configure DSPI2 pins: SCK, MISO and MOSI ---------------------------------*/
    GPIO_InitStructure.GPIO_Pins = DSPI2_PIN_SCK | DSPI2_PIN_MOSI;
    /* Configure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
    GPIO_Init(DSPI2_GPIO, &GPIO_InitStructure);
}

static void Dspi2Init(void)
{
    SPI_InitType SPI_InitStructure;
    /* SPI Config -------------------------------------------------------------*/
  SPI_DefaultInitParaConfig(&SPI_InitStructure);
  SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_TX_HALFDUPLEX;
  SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
  SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE;
  SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;
  SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
  SPI_InitStructure.SPI_CPOLY = 7;

    SPI_Init(DSPI2, &SPI_InitStructure);
    /* Enable SPIy */
    SPI_Enable(DSPI2, ENABLE);
    // int cnt = 8;
    // while(1) {
    //     while (SPI_I2S_GetFlagStatus(DSPI2, SPI_I2S_FLAG_TE) == RESET);
    //     /* Send SPIz data */
    //     SPI_I2S_TxData(DSPI2, 0x95);
    // }
}

void Spi2DmaInit(void)
{
    Dspi2RCC_Configuration();
    Dspi2GpioInit();
    Dspi2Init();
}
/*===============spi2Init end=================*/


/*===============spi1Init start=================*/
void Display_SPI_DMA_Send(const void* buf, uint32_t size)
{
    ST7789_Select();
    if(size > DISP_DMA_MAX_SIZE)
    {
        if(Disp_DMA_TragetPoint == NULL)
        {
            Disp_DMA_TragetPoint = (uint8_t*)buf + size;
        }
        Disp_DMA_CurrentPoint = (uint8_t*)buf + DISP_DMA_MAX_SIZE;
        size = DISP_DMA_MAX_SIZE;
    }
    else
    {
        Disp_DMA_CurrentPoint = NULL;
        Disp_DMA_TragetPoint = NULL;
    }

    DMA_ChannelEnable(DISP_DMA_CHANNEL, DISABLE);
    DISP_DMA_CHANNEL->CMBA = (uint32_t)buf;
    DISP_DMA_CHANNEL->TCNT = size;
    DMA_ChannelEnable(DISP_DMA_CHANNEL, ENABLE);
}

void DMA1_Channel3_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA1_INT_TC3) != RESET)
    {
        if(Disp_DMA_CurrentPoint < Disp_DMA_TragetPoint)
        {
            Display_SPI_DMA_Send(Disp_DMA_CurrentPoint, Disp_DMA_TragetPoint - Disp_DMA_CurrentPoint);
        }
        else
        {
            // digitalWrite_HIGH(CONFIG_SCREEN_CS_PIN);
            // for(int cnt = 0; cnt < 0xff; cnt++);
            while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_BUSY)==SET);
            ST7789_UnSelect();
            // if(Disp_Callback)
            // {
            //     Disp_Callback();
            // }
        }
        DMA_ClearITPendingBit(DMA1_INT_TC3);
    }
}

static void Display_SPI_DMA_Init()
{
    RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);

    DMA_Reset(DISP_DMA_CHANNEL);

    DMA_InitType DMA_InitStructure;
    DMA_DefaultInitParaConfig(&DMA_InitStructure);

    DMA_InitStructure.DMA_Mode = DMA_MODE_NORMAL;  //工作在正常缓存模式
    DMA_InitStructure.DMA_MTOM = DMA_MEMTOMEM_DISABLE;  //DMA通道 没有设置为内存到内存传输
    DMA_InitStructure.DMA_Direction = DMA_DIR_PERIPHERALDST;  //数据传输方向，从内存读取发送到外设
    DMA_InitStructure.DMA_Priority = DMA_PRIORITY_MEDIUM; //DMA通道 中优先级

    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;  //DMA内存基地址
    DMA_InitStructure.DMA_MemoryInc = DMA_MEMORYINC_ENABLE;  //内存地址寄存器自增
    DMA_InitStructure.DMA_MemoryDataWidth = DMA_MEMORYDATAWIDTH_BYTE; //数据宽度
    DMA_InitStructure.DMA_BufferSize = DISP_DMA_MAX_SIZE;  //DMA通道的DMA缓存的大小

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(SPI1->DT));  //DMA外设SPI基地址
    DMA_InitStructure.DMA_PeripheralInc = DMA_PERIPHERALINC_DISABLE;  //外设地址寄存器不变
    DMA_InitStructure.DMA_PeripheralDataWidth = DMA_PERIPHERALDATAWIDTH_BYTE;  //数据宽度

    DMA_Init(DISP_DMA_CHANNEL, &DMA_InitStructure);

    SPI_I2S_DMAEnable(SPI1, SPI_I2S_DMA_TX, ENABLE);

    NVIC_EnableIRQ(DMA1_Channel3_IRQn);

    DMA_INTConfig(DISP_DMA_CHANNEL, DMA_INT_TC, ENABLE);
}





static void Dspi1RCC_Configuration()
{
    /* Enable DSPI2 clock and GPIO clock for DSPI2 and DSPI2 */
    RCC_APB2PeriphClockCmd(DSPI1_GPIO_CLK | DSPI1_CLK, ENABLE);
}

static void Dspi1GpioInit(void)
{
    GPIO_InitType GPIO_InitStructure;

    /* Configure DSPI2 pins: SCK, MISO and MOSI ---------------------------------*/
    GPIO_InitStructure.GPIO_Pins = DSPI1_PIN_SCK | DSPI1_PIN_MOSI;
    /* Configure SCK and MOSI pins as Alternate Function Push Pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;

    GPIO_Init(DSPI1_GPIO, &GPIO_InitStructure);
}

static void Dspi1Init(void)
{
  SPI_InitType  SPI_InitStructure;

  SPI_DefaultInitParaConfig(&SPI_InitStructure);
  SPI_InitStructure.SPI_TransMode = SPI_TRANSMODE_TX_HALFDUPLEX;
  SPI_InitStructure.SPI_Mode = SPI_MODE_MASTER;
  SPI_InitStructure.SPI_FrameSize = SPI_FRAMESIZE_8BIT;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_LOW;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1EDGE;
  SPI_InitStructure.SPI_NSSSEL = SPI_NSSSEL_SOFT;
  SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_2;
  SPI_InitStructure.SPI_FirstBit = SPI_FIRSTBIT_MSB;
  SPI_InitStructure.SPI_CPOLY = 7;

  SPI_Init(DSPI1, &SPI_InitStructure);
  /* Enable SPI module */
  SPI_Enable(DSPI1, ENABLE);
    // int cnt = 8;
    // while(1) {
    //     while (SPI_I2S_GetFlagStatus(DSPI1, SPI_I2S_FLAG_TE) == RESET);
    //     /* Send SPIz data */
    //     SPI_I2S_TxData(DSPI1, 0x81);
    //     for(int i = 0; i < 0xff; i++){};
    // }
}

void Spi1DmaInit(void)
{
    Dspi1RCC_Configuration();
    Dspi1GpioInit();
    Display_SPI_DMA_Init();
    Dspi1Init();
}
/*===============spi2Init end=================*/
