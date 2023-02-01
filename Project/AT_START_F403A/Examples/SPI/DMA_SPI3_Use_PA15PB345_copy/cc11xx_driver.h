#ifndef		__CC11XX_DRIVER_H
#define		__CC11XX_DRIVER_H

#include "at32f4xx.h"
#include "UserType.h"

#define		SPI_MOSI_Port						    GPIOC			//GPIOC  	out
#define		SPI_MISO_Port						    GPIOC			//GPIOC		in
#define		SPI_SCK_Port							GPIOC			//GPIOD		out
#define 	SPI_CSn_Port							GPIOD		    //GPIOD		out
#define		CC11XX_GDO0_Port						GPIOD			//GPIOD		in
#define		CC11XX_GDO2_Port						GPIOD			//GPIOD		in

#define		SPI_MOSI_Pin						GPIO_Pins_12			//GPIOC  	out
#define		SPI_MISO_Pin						GPIO_Pins_11			//GPIOC		in
#define		SPI_SCK_Pin							GPIO_Pins_10			//GPIOD		out
#define 	SPI_CSn_Pin							GPIO_Pins_0			//GPIOD		out
#define		CC11XX_GDO0_Pin						GPIO_Pins_1			//GPIOD		in
#define		CC11XX_GDO2_Pin						GPIO_Pins_5			//GPIOD		in


#define  	SET_SPI_MOSI_Pin()					GPIO_WriteBit(SPI_MOSI_Port,SPI_MOSI_Pin,GPIO_PIN_SET)
#define  	SET_SPI_MISO_Pin()					GPIO_WriteBit(SPI_MOSI_Port,SPI_MISO_Pin,GPIO_PIN_SET)
#define  	SET_SPI_SCK_Pin()					GPIO_WriteBit(SPI_SCK_Port,SPI_SCK_Pin,GPIO_PIN_SET)
#define  	SET_SPI_CSn_Pin()					GPIO_WriteBit(SPI_CSn_Port,SPI_CSn_Pin,GPIO_PIN_SET)
#define  	SET_SPI_GDO0_Pin()					GPIO_WriteBit(CC11XX_GDO0_Port,CC11XX_GDO0_Pin,GPIO_PIN_SET)
#define  	SET_SPI_GDO2_Pin()					GPIO_WriteBit(CC11XX_GDO2_Port,CC11XX_GDO2_Pin,GPIO_PIN_SET)

#define  	CLR_SPI_MOSI_Pin()					GPIO_WriteBit(SPI_MOSI_Port,SPI_MOSI_Pin,GPIO_PIN_RESET)
#define  	CLR_SPI_MISO_Pin()					GPIO_WriteBit(SPI_MISO_Port,SPI_MISO_Pin,GPIO_PIN_RESET)
#define  	CLR_SPI_SCK_Pin()					GPIO_WriteBit(SPI_SCK_Port,SPI_SCK_Pin,GPIO_PIN_RESET)
#define  	CLR_SPI_CSn_Pin()					GPIO_WriteBit(SPI_CSn_Port,SPI_CSn_Pin,GPIO_PIN_RESET)
#define  	CLR_SPI_GDO0_Pin()					GPIO_WriteBit(CC11XX_GDO0_Port,CC11XX_GDO0_Pin,GPIO_PIN_RESET)
#define  	CLR_SPI_GDO2_Pin()					GPIO_WriteBit(CC11XX_GDO2_Port,CC11XX_GDO2_Pin,GPIO_PIN_RESET)

#define		GET_SPI_MISO_PIN_STATUS()		    GPIO_ReadInputDataBit(SPI_MISO_Port,SPI_MISO_Pin)
#define		GET_SPI_GDO0_PIN_STATUS()		    GPIO_ReadInputDataBit(CC11XX_GDO0_Port,CC11XX_GDO0_Pin)
#define		GET_SPI_GDO2_PIN_STATUS()		    GPIO_ReadInputDataBit(CC11XX_GDO2_Port,CC11XX_GDO2_Pin)


#define SPI_READ_ENABLE				1
#define SPI_WRITE_ENABLE			0

//#define RF_FIFO_READ_ENABLE		1
//#define RF_FIFO_WRITE_ENABLE	0


/**************cc11xx register*************/
#define		CC11XX_REG_IOCFG2			0x00
#define		CC11XX_REG_IOCFG1			0x01
#define		CC11XX_REG_IOCFG0			0x02
#define		CC11XX_REG_FIFOTHR		0x03
#define		CC11XX_REG_SYNC1			0x04
#define		CC11XX_REG_SYNC0			0x05
#define		CC11XX_REG_PKTLEN			0x06
#define		CC11XX_REG_PKTCTRL1		0x07
#define		CC11XX_REG_PKTCTRL0		0x08
#define		CC11XX_REG_ADDR				0x09
#define		CC11XX_REG_CHANNR			0x0A
#define		CC11XX_REG_FSCTRL1		0x0B
#define		CC11XX_REG_FSCTRL0		0x0C
#define		CC11XX_REG_FREQ2			0x0D
#define		CC11XX_REG_FREQ1			0x0E
#define		CC11XX_REG_FREQ0			0x0F
#define		CC11XX_REG_MDMCFG4		0x10
#define		CC11XX_REG_MDMCFG3		0x11
#define		CC11XX_REG_MDMCFG2		0x12
#define		CC11XX_REG_MDMCFG1		0x13
#define		CC11XX_REG_MDMCFG0		0x14
#define		CC11XX_REG_DEVIATN		0x15
#define		CC11XX_REG_MCSM2			0x16
#define		CC11XX_REG_MCSM1			0x17
#define		CC11XX_REG_MCSM0			0x18
#define		CC11XX_REG_FOCCFG			0x19
#define		CC11XX_REG_BSCFG			0x1A
#define		CC11XX_REG_AGCTRL2		0x1B
#define		CC11XX_REG_AGCTRL1		0x1C
#define		CC11XX_REG_AGCTRL0		0x1D
#define		CC11XX_REG_WOREVT1		0x1E
#define		CC11XX_REG_WOREVT0		0x1F
#define		CC11XX_REG_WORCTRL		0x20
#define		CC11XX_REG_FREND1			0x21
#define		CC11XX_REG_FREND0			0x22
#define		CC11XX_REG_FSCAL3			0x23
#define		CC11XX_REG_FSCAL2			0x24
#define		CC11XX_REG_FSCAL1			0x25
#define		CC11XX_REG_FSCAL0			0x26
#define		CC11XX_REG_RCCTRL1		0x27
#define		CC11XX_REG_RCCTRL0		0x28
#define		CC11XX_REG_FSTEST			0x29
#define		CC11XX_REG_PTEST			0x2A
#define		CC11XX_REG_AGCTEST		0x2B
#define		CC11XX_REG_TEST2			0x2C
#define		CC11XX_REG_TEST1			0x2D
#define		CC11XX_REG_TEST0			0x2E

//状态寄存器                                      
#define CC11XX_STATUS_REG_PARTNUM      0x30	//CC1101器件号
#define CC11XX_STATUS_REG_VERSION      0x31	//当前版本号
#define CC11XX_STATUS_REG_FREQEST      0x32	//频偏估值
#define CC11XX_STATUS_REG_LQI          0x33
#define CC11XX_STATUS_REG_RSSI         0x34	//接收信号强度指示
#define CC11XX_STATUS_REG_MARCSTATE    0x35	//控制状态机状态(工作状态)
#define CC11XX_STATUS_REG_WORTIME1     0x36
#define CC11XX_STATUS_REG_WORTIME0     0x37
#define CC11XX_STATUS_REG_PKTSTATUS    0x38
#define CC11XX_STATUS_REG_VCO_VC_DAC   0x39
#define CC11XX_STATUS_REG_TXBYTES      0x3A
#define CC11XX_STATUS_REG_RXBYTES      0x3B
#define CC11XX_STATUS_REG_RCCTRL1      0x3C
#define CC11XX_STATUS_REG_RCCTRL0      0x3D

//command strobles(命令寄存器)
#define CC11XX_CMD_SRES         0x30  	//power on reset command	      
#define CC11XX_CMD_SFSTXON      0x31 		//当MCSM0.FS_AUTOCAL＝1此命令将使能校准频率合成器                                            
#define CC11XX_CMD_SXOFF        0x32 		//关闭晶振       
#define CC11XX_CMD_SCAL         0x33    //校准和关闭频率合成器                                           
#define CC11XX_CMD_SRX          0x34		//切换为Rx mode                          
#define CC11XX_CMD_STX          0x35    //切换为Tx mode                                            
#define CC11XX_CMD_SIDLE        0x36 		//退出Rx/Tx mode，关闭频率合成器，如果WOR模式可用也将退出                                          
#define CC11XX_CMD_SAFC         0x37       
#define CC11XX_CMD_SWOR         0x38		//启动Wake-On-Radio mode       
#define CC11XX_CMD_SPWD         0x39 		//当CSn=1时，进入power down       
#define CC11XX_CMD_SFRX         0x3A 		//清Rx FIFO       
#define CC11XX_CMD_SFTX         0x3B  	//清Tx FIFO      
#define CC11XX_CMD_SWORRST      0x3C       
#define CC11XX_CMD_SNOP         0x3D 		//空操作       
#define CC11XX_CMD_PATABLE      0x3E		//写power table
#define CC11XX_CMD_TXFIFO       0x3F		//tx fifo addr
#define CC11XX_CMD_RXFIFO       0x3F		//rx fifo addr


#define WRITE_SINGLE      			0x00			//写单个
#define WRITE_BURST     				0x40			//写多个
#define READ_SINGLE     				0x80			//读单个
#define READ_BURST      				0xC0			//读多个
#define BYTES_IN_RXFIFO     		0x7F  		//接收缓冲区有效字节
#define CRC_OK              		0x80 			//CRC校验通过标志位


//#define SINGLE_BYTE_ACCESS_TX_FIFO_HEADER_BYTE		0x3F			//r/w bit = 0, burst bit = 0
//#define	BURST_ACCESS_TX_FIFO_HEADER_BYTE					0x7F			//r/w bit = 0, burst bit = 1
//#define	SINGLE_BYTE_ACCESS_RX_FIFO_HEADER_BYTE		0xBF			//r/w bit = 1, burst bit = 0
//#define	BURST_ACCESS_RX_FIFO_HEADER_BYTE					0xFF			//r/w bit = 1, burst bit = 1



#define CC11xx_CARRIER_ENABLE		0


#define TX_DEV_ADDRESS			0x05
#define RX_DEV_ADDRESS			TX_DEV_ADDRESS//0x06

#define SYNC_WORD_FIELD 		0x1234

typedef enum 
{ 
		RF_TX_MODE, 
		RF_RX_MODE 
}CC11xx_Tx_Rx_Mode;

typedef enum 
{ 
		ADDR_NO_CHECK_BROAD,			//广播，不检查地址		 
		ADDR_CHECK_NO_BROAD, 			//非广播(点对点通信)，检查地址
		ADDR_0_CHECK_BROAD, 			//使用地址0进行广播
		ADDR_0_255_CHECK_BROAD 		//使用地址0和255进行广播
}CC11xx_AddrCheckMode;

typedef enum 
{ 
		ADDR_NO_CHECK, 
		ADDR_CHECK
} CC11xx_TxDataMode;

//10dBm,7dBm,5dBm,0dBm,-10dBm,-15dBm,-20dBm,-30dBm
typedef enum
{
		POWER_10dBm,
		POWER_7dBm,
		POWER_5dBm,
		POWER_0dBm,
		POWER_n10dBm,
		POWER_n15dBm,
		POWER_n20dBm,
		POWER_n30dBm
}CC11xx_TxPower;


extern uint8_t send_buff[1];	
extern uint8_t receive_buff[1];

void cc11xx_ctrl_gpio_config(void);
uint8_t cc11xx_spi_write_read_one_byte(uint8_t wrByteData);
void cc11xx_write_command(uint8_t cmdStrobe);
void cc11xx_power_on_reset(void);
void cc11xx_spi_write_one_register(uint8_t registerAddr,uint8_t registerData);
uint8_t cc11xx_spi_read_one_register(uint8_t registerAddr);
void cc11xx_set_addr_check_mode(CC11xx_AddrCheckMode mode);
void cc11xx_set_device_addr(uint8_t devAddr);
void c11xx_wor_enable(void);
void cc11xx_flus_tx_fifo(void);
void cc11xx_flus_rx_fifo(void);
void cc11xx_switch_tx_rx_mode(CC11xx_Tx_Rx_Mode rfMode);
void cc11xx_send_packet(uint8_t* pTxBuff,uint8_t len);
uint8_t cc11xx_receive_packet(uint8_t* pRxBuff);
uint8_t cc11xx_get_reg_status(uint8_t registerAddr);
uint8_t cc11xx_receive_count(void);
void cc11xx_config(void);

void cc11xx_init(void);

#endif

