#include "spi_dma.h"
#include "at32f4xx.h"

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
  SPI_InitStructure.SPI_MCLKP = SPI_MCLKP_8;
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
    Dspi1Init();
}
/*===============spi2Init end=================*/
