#include "cc11xx_driver.h"
#include "stdio.h"
/*******************************************************************************************************************************
1.固定包长模式－－包长被设置在PKTLEN寄存器中
2.可变包长模式－－包长被设置在SYCN WORD后的第一个字节中，这个字节决定了PAYLOAD DATA的长度，而不包括length和CRC字段，
  此时，PKTLEN寄存器被用来设置在RX中允许的maximum packet length.任何被接收到的包的length字节的值大于PKTLEN的值时都
  将忽略.
	
Tx FIFO 内容有(包格式)：	Pre bits(n bytes)||Sync word(2~4bytes)||length field(1byte)||Address field(1byte)||Payload(user data)||CRC-16(2bytes)
模块晶体频率:26MHZ(最大27MHZ)
CC1101频率范围分如下3个频段：
														300~348MHZ，
														387~464MHZ，
														779~928MHZ
********************************************************************************************************************************/
uint8_t send_buff[1] = {0};	
uint8_t receive_buff[1] = {0};

uint8_t powerTable[8] = {0xc0,0xC8,0x84,0x60,0x34,0x1D,0x0E,0x12};  //10dBm,7dBm,5dBm,0dBm,-10dBm,-15dBm,-20dBm,-30dBm

uint8_t cc11xx_reg_init[][2]= 
{
		{CC11XX_REG_IOCFG2,			0x29},
		{CC11XX_REG_IOCFG1,			0x2e},
		{CC11XX_REG_IOCFG0,			0x06},
		{CC11XX_REG_FIFOTHR,		0x07},
		{CC11XX_REG_SYNC1,			0xD3},
		{CC11XX_REG_SYNC0,			0x91},
		{CC11XX_REG_PKTLEN,			0xFF},
		{CC11XX_REG_PKTCTRL1,		0x04},
		{CC11XX_REG_PKTCTRL0,		0x05},
		{CC11XX_REG_ADDR,				0x00},
		{CC11XX_REG_CHANNR,			0x00},
		{CC11XX_REG_FSCTRL1,		0x0C},
		{CC11XX_REG_FSCTRL0,		0x00},
		{CC11XX_REG_FREQ2,			0x10},
		{CC11XX_REG_FREQ1,			0xB1},
		{CC11XX_REG_FREQ0,			0x3B},
		{CC11XX_REG_MDMCFG4,		0x2D},
		{CC11XX_REG_MDMCFG3,		0x3B},
		{CC11XX_REG_MDMCFG2,		0x13},
		{CC11XX_REG_MDMCFG1,		0x22},
		{CC11XX_REG_MDMCFG0,		0xF8},
		{CC11XX_REG_DEVIATN,		0x62},
		{CC11XX_REG_MCSM2,			0x07},
		{CC11XX_REG_MCSM1,			0x30},
		{CC11XX_REG_MCSM0,			0x18},
		{CC11XX_REG_FOCCFG,			0x1D},
		{CC11XX_REG_BSCFG,			0x1C},
		{CC11XX_REG_AGCTRL2,		0xC7},
		{CC11XX_REG_AGCTRL1,		0x00},
		{CC11XX_REG_AGCTRL0,		0xB0},
		{CC11XX_REG_WOREVT1,		0x87},
		{CC11XX_REG_WOREVT0,		0x6B},
		{CC11XX_REG_WORCTRL,		0xFB},
		{CC11XX_REG_FREND1,			0xB6},
		{CC11XX_REG_FREND0,			0x10},
		{CC11XX_REG_FSCAL3,			0xEA},
		{CC11XX_REG_FSCAL2,			0x2A},
		{CC11XX_REG_FSCAL1,			0x00},
		{CC11XX_REG_FSCAL0,			0x1F},
		{CC11XX_REG_RCCTRL1,		0x41},
		{CC11XX_REG_RCCTRL0,		0x00},
		{CC11XX_REG_FSTEST,			0x59},
		{CC11XX_REG_PTEST,			0x7F},
		{CC11XX_REG_AGCTEST,		0x3F},
		{CC11XX_REG_TEST2,			0x88},
		{CC11XX_REG_TEST1,			0x31},
		{CC11XX_REG_TEST0,			0x09}
};

//CC11xx控制IO初始化
void cc11xx_ctrl_gpio_config(void)
{
        struct 
        {
            GPIO_Type* GPIOx;
            uint16_t GPIO_Pins;
            GPIOMaxSpeed_Type GPIO_MaxSpeed;
            GPIOMode_Type GPIO_Mode;
        }CC11xx_GPIO_Init[] = 
        {
            {.GPIOx = SPI_MOSI_Port, .GPIO_Pins = SPI_MOSI_Pin    , .GPIO_Mode = GPIO_Mode_OUT_PP      , .GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz},
            {.GPIOx = SPI_CSn_Port, .GPIO_Pins = SPI_CSn_Pin     , .GPIO_Mode = GPIO_Mode_OUT_PP      , .GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz},
            {.GPIOx = SPI_SCK_Port, .GPIO_Pins = SPI_SCK_Pin     , .GPIO_Mode = GPIO_Mode_OUT_PP      , .GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz},
            {.GPIOx = SPI_MISO_Port, .GPIO_Pins = SPI_MISO_Pin    , .GPIO_Mode = GPIO_Mode_IN_FLOATING , .GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz},
            {.GPIOx = CC11XX_GDO0_Port, .GPIO_Pins = CC11XX_GDO0_Pin , .GPIO_Mode = GPIO_Mode_IN_FLOATING , .GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz},
            {.GPIOx = CC11XX_GDO2_Port, .GPIO_Pins = CC11XX_GDO2_Pin , .GPIO_Mode = GPIO_Mode_IN_FLOATING , .GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz},            
        };
	
		CLR_SPI_MOSI_Pin();
		CLR_SPI_MISO_Pin();
		CLR_SPI_SCK_Pin();
		CLR_SPI_CSn_Pin();
		CLR_SPI_GDO0_Pin();
		CLR_SPI_GDO2_Pin();
        
        for(uint8_t cnt = 0; cnt < ARRAY_SIZE(CC11xx_GPIO_Init); cnt++)
        {
            GPIO_InitType GPIO_InitStruct = 
            {
                .GPIO_Pins = CC11xx_GPIO_Init[cnt].GPIO_Pins,
                .GPIO_Mode = CC11xx_GPIO_Init[cnt].GPIO_Mode,
                .GPIO_MaxSpeed = CC11xx_GPIO_Init[cnt].GPIO_MaxSpeed,
            };
            GPIO_Init(CC11xx_GPIO_Init[cnt].GPIOx, &GPIO_InitStruct);
        }
}

//spi写一个byte(MSB高位先)
uint8_t cc11xx_spi_write_read_one_byte(uint8_t wrByteData)
{
		uint8_t rByteData = 0;
	
		for(uint8_t i = 0;i < 8;i++)
		{
				if((wrByteData << i) & 0x80)
				{
						SET_SPI_MOSI_Pin();
				}
				else
				{
						CLR_SPI_MOSI_Pin();
				}	
				
				SET_SPI_SCK_Pin();
				rByteData <<= 1;
				if(GET_SPI_MISO_PIN_STATUS())		 	//读取CC11xx返回的状态或数据
				{
						rByteData++;
				}
				
				CLR_SPI_SCK_Pin();
		}
		return rByteData;
}

//读一个寄存器
uint8_t cc11xx_spi_read_one_register(uint8_t registerAddr)
{
		uint8_t registerData = 0;

		CLR_SPI_CSn_Pin();
	
		while(GET_SPI_MISO_PIN_STATUS());			//等待MISO＝L
		cc11xx_spi_write_read_one_byte(registerAddr | READ_SINGLE);
		registerData = cc11xx_spi_write_read_one_byte(0xFF);		//此时写任意值CC11xx(从机)将返回 1 byte 寄存器的值
	
		SET_SPI_CSn_Pin();
	
		return registerData;
}

//连续读取N个寄存器
void cc11xx_spi_read_multi_register(uint8_t registerAddr,uint8_t* pReadBuf,uint8_t len)
{
		CLR_SPI_CSn_Pin();
	
		while(GET_SPI_MISO_PIN_STATUS());			//等待MISO＝L
	
		cc11xx_spi_write_read_one_byte(registerAddr | READ_BURST);
		for(uint8_t i = 0;i < len;i++)
		{
				*(pReadBuf + i) = cc11xx_spi_write_read_one_byte(0xFF);	//此时写任意值CC11xx(从机)将返回 1 byte 寄存器的值
		}
	
		SET_SPI_CSn_Pin();
}

//写一个寄存器
void cc11xx_spi_write_one_register(uint8_t registerAddr,uint8_t registerData)
{
		CLR_SPI_CSn_Pin();
	
		while(GET_SPI_MISO_PIN_STATUS());			//等待MISO＝L
	
		cc11xx_spi_write_read_one_byte(registerAddr | WRITE_SINGLE);
		cc11xx_spi_write_read_one_byte(registerData);
	
		SET_SPI_CSn_Pin();
}

//连续写n个寄存器
void cc11xx_spi_write_multi_register(uint8_t registerAddr,uint8_t* pWriteBuff,uint8_t len)
{
		CLR_SPI_CSn_Pin();
	
		while(GET_SPI_MISO_PIN_STATUS());			//等待MISO＝L
	
		cc11xx_spi_write_read_one_byte(registerAddr | WRITE_BURST);
		
		for(uint8_t i = 0;i < len;i++)
		{
				cc11xx_spi_write_read_one_byte(*(pWriteBuff + i));
		}
		
		SET_SPI_CSn_Pin();
}

//CC11xx写命令(command strobes)
void cc11xx_write_command(uint8_t cmdStrobe)
{
		CLR_SPI_CSn_Pin();
	
		while(GET_SPI_MISO_PIN_STATUS());			//等待MISO＝L
	
		cc11xx_spi_write_read_one_byte(cmdStrobe); //cmdStrobe=1bit r/w + 1 bit burst + 6 bits addr 
	
		SET_SPI_CSn_Pin();
}

//设置地址检查模式(广播模式)
void cc11xx_set_addr_check_mode(CC11xx_AddrCheckMode mode)
{
		uint8_t reg = 0;
		reg = cc11xx_spi_read_one_register(CC11XX_REG_PKTCTRL1) & 0xFC;		//读取地址检查模式
	
		switch(mode)
		{
				case 	ADDR_NO_CHECK_BROAD:		reg &= 0xFC;	//广播，不检查地址	
																			break;
			
				case ADDR_CHECK_NO_BROAD: 		reg |= 0x01;	//非广播(点对点通信)，检查地址
																			break;
			
				case ADDR_0_CHECK_BROAD: 			reg |= 0x02;	//使用地址0进行广播
																			break;
				case ADDR_0_255_CHECK_BROAD: 	reg |= 0x03;	//使用地址0和255进行广播
																			break;
		}
		cc11xx_spi_write_one_register(CC11XX_REG_PKTCTRL1,reg);	
}

//CC11xx设置设备地址
void cc11xx_set_device_addr(uint8_t devAddr)
{
		cc11xx_spi_write_one_register(CC11XX_REG_ADDR,devAddr);						//设置设备地址
}

//设置CC11xx的rx/tx模式
void cc11xx_switch_tx_rx_mode(CC11xx_Tx_Rx_Mode rfMode)
{
		if(RF_TX_MODE == rfMode)
		{
				//cc11xx_spi_write_one_register(CC11XX_REG_IOCFG0,0x46);
				cc11xx_write_command(CC11XX_CMD_STX);
		}
		else if(RF_RX_MODE == rfMode)
		{
				//cc11xx_spi_write_one_register(CC11XX_REG_IOCFG0,0x46);
				cc11xx_write_command(CC11XX_CMD_SRX);
		}
		while(0 != GET_SPI_GDO0_PIN_STATUS());		//等待发送 或 接收开始
}

//CC11xx设置为 idle 模式
void cc11xx_set_idle_mode(void)
{
		cc11xx_write_command(CC11XX_CMD_SIDLE);
}

//CC11xx清发送缓存
void cc11xx_flus_tx_fifo(void)
{
		cc11xx_set_idle_mode();											//首先进入IDLE模式
    cc11xx_write_command(CC11XX_CMD_SFTX);			//写入清发送缓冲区命令	
}

//CC11xx清接收缓存
void cc11xx_flus_rx_fifo(void)
{
		cc11xx_set_idle_mode();											//首先进入IDLE模式
    cc11xx_write_command(CC11XX_CMD_SFRX);			//写入清接收缓冲区命令
}


//将数据包发送出去
//Tx FIFO 内容有(包格式)：	1010xxPre bits(n bytes)||Sync word(2~4bytes)||length field(1byte)||Address field(1byte)||Payload(user data)||CRC-16(2bytes)
void cc11xx_send_packet(uint8_t* pTxBuff,uint8_t len)
{
		cc11xx_flus_tx_fifo();
		cc11xx_spi_write_one_register(CC11XX_CMD_TXFIFO,len+1);
		cc11xx_spi_write_one_register(CC11XX_CMD_TXFIFO,RX_DEV_ADDRESS);
		cc11xx_spi_write_multi_register(CC11XX_CMD_TXFIFO,pTxBuff,len);	

		cc11xx_switch_tx_rx_mode(RF_TX_MODE);
		while(!GET_SPI_GDO0_PIN_STATUS());		//等待
		while(GET_SPI_GDO0_PIN_STATUS());			//等待
		//cc11xx_flus_tx_fifo();
}

//读取空中接收到的数据包
//RXFIFO中的数据内容有：length field(1byte)||Address field(1byte)||Payload(user data)||CRC status(2bytes)
uint8_t cc11xx_receive_packet(uint8_t* pRxBuff)
{
		uint8_t crc_ok = 0;
		uint8_t len = 0;
		uint8_t addrMode = 0;

		uint8_t addr = 0;
	
		uint8_t status[2] = {0,0};
		cc11xx_switch_tx_rx_mode(RF_RX_MODE);
		//len = cc11xx_receive_count();
		
		if(cc11xx_receive_count() != 0)
		{
				len = cc11xx_spi_read_one_register(CC11XX_CMD_RXFIFO);		//读取长度信息
				
				addrMode = cc11xx_spi_read_one_register(CC11XX_REG_PKTCTRL1) & 0x03;
				if(ADDR_NO_CHECK_BROAD != addrMode)  											//如果不是广播不检查地址的模式
				{
						addr = cc11xx_spi_read_one_register(CC11XX_CMD_RXFIFO);		//读取地址字段
						
						len--;//减去地址字段长度(1byte)				
				}
				cc11xx_spi_read_multi_register(CC11XX_CMD_RXFIFO,pRxBuff,len);
				cc11xx_spi_read_multi_register(CC11XX_CMD_RXFIFO,status,2);
				cc11xx_flus_rx_fifo();
				
				#if DEBUG_ENABLE
						printf("len:%d,addr:0x%x,0x%x 0x%x\r\n",len,addr,status[0],status[1]);
				#endif
                addr++; // 修复警告
				if(CRC_OK == (status[1] & 0x80))
				{
						crc_ok = 1;
				}
				else
				{
						crc_ok = 0;
				}
		}
		return crc_ok;		
}

//读一个状态寄存器
uint8_t cc11xx_get_reg_status(uint8_t registerAddr)
{
		uint8_t regStatus = 0;
	
		CLR_SPI_CSn_Pin();
	
		while(GET_SPI_MISO_PIN_STATUS());			//等待MISO＝L
	
		cc11xx_spi_write_read_one_byte(registerAddr | READ_BURST);	//burst位用于区分status或command
    regStatus = cc11xx_spi_write_read_one_byte(0xFF);						//读取状态
	
		SET_SPI_CSn_Pin();
	
		return regStatus;
}

//获取空中接收到的字节数
uint8_t cc11xx_receive_count(void)
{
		uint8_t byteCounts = 0;
	
		byteCounts = cc11xx_get_reg_status(CC11XX_STATUS_REG_RXBYTES) & BYTES_IN_RXFIFO;
	
		return byteCounts;
}

//CC11xx设置发射功率
void cc11xx_set_output_power(CC11xx_TxPower xdBm)
{
		uint8_t regTemp = 0;
		regTemp = cc11xx_spi_read_one_register(CC11XX_REG_FREND0) & 0xF8;
		regTemp |= xdBm;
		cc11xx_spi_write_one_register(CC11XX_REG_FREND0,regTemp);	
}

//CC11xx写入同步字段(MSB)
void cc11xx_write_sync_field(uint16_t sync)
{
		cc11xx_spi_write_one_register(CC11XX_REG_SYNC1, (uint8_t)(sync >> 8));
    cc11xx_spi_write_one_register(CC11XX_REG_SYNC0, (uint8_t)(0x00FF & sync)); 	//写入同步字段 16Bit
}


void c11xx_wor_enable(void)
{
		cc11xx_spi_write_one_register(CC11XX_REG_MCSM0,0x18);		
    cc11xx_spi_write_one_register(CC11XX_REG_WORCTRL,0x78); 
    cc11xx_spi_write_one_register(CC11XX_REG_MCSM2,0x00);
    cc11xx_spi_write_one_register(CC11XX_REG_WOREVT1,0x8C);
    cc11xx_spi_write_one_register(CC11XX_REG_WOREVT0,0xA0);
		cc11xx_write_command( CC11XX_CMD_SWORRST);							//写入WOR命令
}

//设备复位
void cc11xx_soft_reset(void)
{
		cc11xx_write_command(CC11XX_CMD_SRES);
}

//设备上电复位
void cc11xx_power_on_reset(void)
{
		SET_SPI_CSn_Pin();
		//delay_ms(1);//HAL_Delay(1);
		CLR_SPI_CSn_Pin();
		//delay_ms(1);//HAL_Delay(1);
		SET_SPI_CSn_Pin();
		for(uint16_t cnt = 0; cnt < 1000; cnt++);
		cc11xx_soft_reset();
}

void cc11xx_hw_check(void)
{
		cc11xx_power_on_reset();
		for(uint8_t i = 0;i < sizeof(cc11xx_reg_init)/2;i++)
		{
				cc11xx_spi_write_one_register(cc11xx_reg_init[i][0],cc11xx_reg_init[i][1]);		
		}
		uint8_t dat = 0;
		for(uint8_t i = 0;i < sizeof(cc11xx_reg_init)/2;i++)
		{
				dat = cc11xx_spi_read_one_register(cc11xx_reg_init[i][0]); 
				printf("0x%x ",dat);
		}
}

void cc11xx_carrier_check(void)
{
		cc11xx_switch_tx_rx_mode(RF_TX_MODE);																//carrier mode
		while(1);
}


//CC11xx设备初始化配置
void cc11xx_config(void)
{
    cc11xx_ctrl_gpio_config();
		cc11xx_power_on_reset();
		
		for(uint8_t i = 0;i < sizeof(cc11xx_reg_init)/2;i++)
		{
				cc11xx_spi_write_one_register(cc11xx_reg_init[i][0],cc11xx_reg_init[i][1]);		
		}
		cc11xx_set_addr_check_mode(ADDR_CHECK_NO_BROAD);											//设置点对点通信模式
		
		cc11xx_set_device_addr(RX_DEV_ADDRESS);																	//写入设备地址
		cc11xx_write_sync_field(SYNC_WORD_FIELD);															//写入同步字段
		
		cc11xx_spi_write_multi_register(CC11XX_CMD_PATABLE,powerTable,8);			//写入功率表
		cc11xx_set_output_power(POWER_n10dBm);																	//选择功率值
		
        //carrier mode
		#if CC11xx_CARRIER_ENABLE
                cc11xx_hw_check();
				cc11xx_carrier_check();
		#endif
		
		#if DEBUG_ENABLE==0
				uint8_t dat = 0;
				for(uint8_t i = 0;i < sizeof(cc11xx_reg_init)/2;i++)
				{
						dat = cc11xx_spi_read_one_register(cc11xx_reg_init[i][0]); 
						printf("0x%x ",dat);
				}	
				
				dat = cc11xx_spi_read_one_register(CC11XX_REG_FREND0) & 0x07;
				printf("\r\n0x%x\r\n",dat);
		#endif
}

void cc11xx_init(void)
{
		cc11xx_power_on_reset();
	
		for(uint8_t i = 0;i < sizeof(cc11xx_reg_init)/2;i++)
		{
				cc11xx_spi_write_one_register(cc11xx_reg_init[i][0],cc11xx_reg_init[i][1]);		
		}
		cc11xx_set_addr_check_mode(ADDR_CHECK_NO_BROAD);												//设置点对点通信模式
		
		cc11xx_set_device_addr(TX_DEV_ADDRESS);																	//写入设备地址
		cc11xx_write_sync_field(SYNC_WORD_FIELD);																//写入同步字段
	
		cc11xx_spi_write_multi_register(CC11XX_CMD_PATABLE,powerTable,8);				//写入功率表
		cc11xx_set_output_power(POWER_n10dBm);//(POWER_n30dBm);//(POWER_10dBm); //									//选择功率值
}
