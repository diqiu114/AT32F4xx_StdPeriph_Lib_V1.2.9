#ifndef __SPI_DMA_H
#define __SPI_DMA_H

#include "stdint.h"

#define DSPI2                   SPI2
#define DSPI2_CLK               RCC_APB1PERIPH_SPI2
#define DSPI2_GPIO              GPIOB
#define DSPI2_GPIO_CLK          RCC_APB2PERIPH_GPIOB
#define DSPI2_PIN_SCK           GPIO_Pins_13
#define DSPI2_PIN_MISO          GPIO_Pins_14
#define DSPI2_PIN_MOSI          GPIO_Pins_15 

#define DSPI1                   SPI1
#define DSPI1_CLK               RCC_APB2PERIPH_SPI1
#define DSPI1_GPIO              GPIOA
#define DSPI1_GPIO_CLK          RCC_APB2PERIPH_GPIOA
#define DSPI1_PIN_SCK           GPIO_Pins_5  // PA5
#define DSPI1_PIN_MISO          GPIO_Pins_6     // PA6
#define DSPI1_PIN_MOSI          GPIO_Pins_7   // PA7


#define DISP_DMA_CHANNEL     DMA1_Channel3
#define DISP_DMA_MAX_SIZE    65535

void Spi2DmaInit(void);
void Spi1DmaInit(void);

void Display_SPI_DMA_Send(const void* buf, uint32_t size);

#endif


