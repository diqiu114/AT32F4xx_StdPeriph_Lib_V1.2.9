#include "bsp.h"

uint32_t bsp_GetRCCofGPIO(GPIO_Type* GPIOx)
{
	uint32_t rcc = 0;

	if (GPIOx == GPIOA)
	{
		rcc = RCC_APB2PERIPH_GPIOA;
	}
	else if (GPIOx == GPIOB)
	{
		rcc = RCC_APB2PERIPH_GPIOB;
	}
	else if (GPIOx == GPIOC)
	{
		rcc = RCC_APB2PERIPH_GPIOC;
	}
	else if (GPIOx == GPIOD)
	{
		rcc = RCC_APB2PERIPH_GPIOD;
	}
	return rcc;
}

uint32_t bsp_GetRCCofTMR(TMR_Type* TMRx)
{
	uint32_t rcc = 0;


	if (TMRx == TMR1)
	{
		rcc = RCC_APB2PERIPH_TMR1;
	}
	else if (TMRx == TMR8)
	{
		rcc = RCC_APB2PERIPH_TMR8;
	}
	else if (TMRx == TMR9)
	{
		rcc = RCC_APB2PERIPH_TMR9;
	}
	else if (TMRx == TMR10)
	{
		rcc = RCC_APB2PERIPH_TMR10;
	}
	else if (TMRx == TMR11)
	{
		rcc = RCC_APB2PERIPH_TMR11;
	}

	else if (TMRx == TMR2)
	{
		rcc = RCC_APB1PERIPH_TMR2;
	}
	else if (TMRx == TMR3)
	{
		rcc = RCC_APB1PERIPH_TMR3;
	}
	else if (TMRx == TMR4)
	{
		rcc = RCC_APB1PERIPH_TMR4;
	}
	else if (TMRx == TMR5)
	{
		rcc = RCC_APB1PERIPH_TMR5;
	}

	return rcc;
}


void bsp_ConfigTMRGpio(GPIO_Type* GPIOx, uint16_t GPIO_PinX, TMR_Type* TMRx, uint8_t _ucChannel)
{
	GPIO_InitType  GPIO_InitStructure;


	RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx), ENABLE);

 
	if ((TMRx == TMR1) || (TMRx == TMR8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTMR(TMRx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTMR(TMRx), ENABLE);
	}

	
	GPIO_InitStructure.GPIO_Pins = GPIO_PinX;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure); 
}


void bsp_ConfigGpioOut(GPIO_Type* GPIOx, uint16_t GPIO_PinX)
{
	GPIO_InitType  GPIO_InitStructure;

	
	RCC_APB2PeriphClockCmd(bsp_GetRCCofGPIO(GPIOx), ENABLE);


	GPIO_InitStructure.GPIO_Pins        = GPIO_PinX;		
	GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_OUT_PP;	
	GPIO_InitStructure.GPIO_MaxSpeed    = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOx, &GPIO_InitStructure);
}


void bsp_SetTMROutPWM(GPIO_Type* GPIOx, uint16_t GPIO_Pin, TMR_Type* TMRx, uint8_t _ucChannel,
	 uint32_t _ulFreq, uint32_t _ulDutyCycle)
{
	TMR_TimerBaseInitType  TMR_TimeBaseStructure;
	TMR_OCInitType  TMR_OCInitStructure;
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTMRxCLK;

	if (_ulDutyCycle == 0)
	{		
		TMR_Cmd(TMRx, DISABLE);		
		bsp_ConfigGpioOut(GPIOx, GPIO_Pin);		
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);		
		return;
	}
	else if (_ulDutyCycle == 10000)
	{
		TMR_Cmd(TMRx, DISABLE);		

		bsp_ConfigGpioOut(GPIOx, GPIO_Pin);			
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);	
		return;
	}
	

	bsp_ConfigTMRGpio(GPIOx, GPIO_Pin, TMRx, _ucChannel);	
	
	uiTMRxCLK = SystemCoreClock/2;	

	if (_ulFreq < 100)
	{
		usPrescaler = 10000 - 1;					
		usPeriod =  (uiTMRxCLK / 10000) / _ulFreq  - 1;		
	}
	else if (_ulFreq < 3000)
	{
		usPrescaler = 100 - 1;					
		usPeriod =  (uiTMRxCLK / 100) / _ulFreq  - 1;		
	}
	else	
	{
		usPrescaler = 0;					
		usPeriod = uiTMRxCLK / _ulFreq - 1;	
	}

	
	TMR_TimeBaseStructure.TMR_Period    = usPeriod;
	TMR_TimeBaseStructure.TMR_DIV       = usPrescaler;
	TMR_TimeBaseStructure.TMR_ClockDivision = 0;
	TMR_TimeBaseStructure.TMR_CounterMode   = TMR_CounterDIR_Up;
	TMR_TimeBaseStructure.TMR_RepetitionCounter = 0;	
	TMR_TimeBaseInit(TMRx, &TMR_TimeBaseStructure);


	TMR_OCStructInit(&TMR_OCInitStructure);		
	
	TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;
	TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Enable;
	TMR_OCInitStructure.TMR_Pulse = (_ulDutyCycle * usPeriod) / 10000;
	TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;
	
	TMR_OCInitStructure.TMR_OutputNState = TMR_OutputNState_Disable;		
	TMR_OCInitStructure.TMR_OCNPolarity = TMR_OCNPolarity_High;				
	TMR_OCInitStructure.TMR_OCIdleState = TMR_OCIdleState_Reset;		
	TMR_OCInitStructure.TMR_OCNIdleState = TMR_OCNIdleState_Reset;		
	
	if (_ucChannel == 1)
	{
		TMR_OC1Init(TMRx, &TMR_OCInitStructure);
		TMR_OC1PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}
	else if (_ucChannel == 2)
	{
		TMR_OC2Init(TMRx, &TMR_OCInitStructure);
		TMR_OC2PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}
	else if (_ucChannel == 3)
	{
		TMR_OC3Init(TMRx, &TMR_OCInitStructure);
		TMR_OC3PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}
	else if (_ucChannel == 4)
	{
		TMR_OC4Init(TMRx, &TMR_OCInitStructure);
		TMR_OC4PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}

	TMR_ARPreloadConfig(TMRx, ENABLE);

	
	TMR_Cmd(TMRx, ENABLE);


	if ((TMRx == TMR1) || (TMRx == TMR8))
	{
		TMR_CtrlPWMOutputs(TMRx, ENABLE);
	}
}


void bsp_SetTIMOutPWM_N(GPIO_Type* GPIOx, uint16_t GPIO_Pin, TMR_Type* TMRx, uint8_t _ucChannel,
	 uint32_t _ulFreq, uint32_t _ulDutyCycle)
{
	TMR_TimerBaseInitType  TMR_TimeBaseStructure;
	TMR_OCInitType  TMR_OCInitStructure;
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTMRxCLK;

	if (_ulDutyCycle == 0)
	{		
		TMR_Cmd(TMRx, DISABLE);		
		bsp_ConfigGpioOut(GPIOx, GPIO_Pin);			
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);	
		return;
	}
	else if (_ulDutyCycle == 10000)
	{
		TMR_Cmd(TMRx, DISABLE);		

		bsp_ConfigGpioOut(GPIOx, GPIO_Pin);		
		GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);		
		return;
	}
	

	bsp_ConfigTMRGpio(GPIOx, GPIO_Pin, TMRx, _ucChannel);	
	
	uiTMRxCLK = SystemCoreClock/2;	

	if (_ulFreq < 100)
	{
		usPrescaler = 10000 - 1;					
		usPeriod =  (uiTMRxCLK / 10000) / _ulFreq  - 1;		
	}
	else if (_ulFreq < 3000)
	{
		usPrescaler = 100 - 1;					
		usPeriod =  (uiTMRxCLK / 100) / _ulFreq  - 1;		
	}
	else	
	{
		usPrescaler = 0;					
		usPeriod = uiTMRxCLK / _ulFreq - 1;
	}

	
	TMR_TimeBaseStructure.TMR_Period = usPeriod;
	TMR_TimeBaseStructure.TMR_DIV = usPrescaler;
	TMR_TimeBaseStructure.TMR_ClockDivision = 0;
	TMR_TimeBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
	TMR_TimeBaseStructure.TMR_RepetitionCounter = 0;	
	TMR_TimeBaseInit(TMRx, &TMR_TimeBaseStructure);


	TMR_OCStructInit(&TMR_OCInitStructure);		
	
	TMR_OCInitStructure.TMR_OCMode = TMR_OCMode_PWM1;
	TMR_OCInitStructure.TMR_OutputState = TMR_OutputState_Disable;		
	TMR_OCInitStructure.TMR_Pulse = (_ulDutyCycle * usPeriod) / 10000;
	TMR_OCInitStructure.TMR_OCPolarity = TMR_OCPolarity_High;
	
	TMR_OCInitStructure.TMR_OutputNState = TMR_OutputNState_Enable;		
	TMR_OCInitStructure.TMR_OCNPolarity = TMR_OCNPolarity_High;					
	TMR_OCInitStructure.TMR_OCIdleState = TMR_OCIdleState_Reset;		
	TMR_OCInitStructure.TMR_OCNIdleState = TMR_OCNIdleState_Reset;		
	
	if (_ucChannel == 1)
	{
		TMR_OC1Init(TMRx, &TMR_OCInitStructure);
		TMR_OC1PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}
	else if (_ucChannel == 2)
	{
		TMR_OC2Init(TMRx, &TMR_OCInitStructure);
		TMR_OC2PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}
	else if (_ucChannel == 3)
	{
		TMR_OC3Init(TMRx, &TMR_OCInitStructure);
		TMR_OC3PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}
	else if (_ucChannel == 4)
	{
		TMR_OC4Init(TMRx, &TMR_OCInitStructure);
		TMR_OC4PreloadConfig(TMRx, TMR_OCPreload_Enable);
	}

	TMR_ARPreloadConfig(TMRx, ENABLE);

	
	TMR_Cmd(TMRx, ENABLE);


	if ((TMRx == TMR1) || (TMRx == TMR8))
	{
		TMR_CtrlPWMOutputs(TMRx, ENABLE);
	}
}



void bsp_SetTMRforInt(TMR_Type* TMRx, uint32_t _ulFreq, uint8_t _PreemptionPriority, uint8_t _SubPriority)
{
	TMR_TimerBaseInitType  TMR_TimeBaseStructure;
	uint16_t usPeriod;
	uint16_t usPrescaler;
	uint32_t uiTMRxCLK;

  
	if ((TMRx == TMR1) || (TMRx == TMR8))
	{
		RCC_APB2PeriphClockCmd(bsp_GetRCCofTMR(TMRx), ENABLE);
	}
	else
	{
		RCC_APB1PeriphClockCmd(bsp_GetRCCofTMR(TMRx), ENABLE);
	}

	if (_ulFreq == 0)
	{
		TMR_Cmd(TMRx, DISABLE);		

	
		{
			NVIC_InitType NVIC_InitStructure;	
			uint8_t irq = 0;	

			if (TMRx == TMR1)
				irq = TMR1_OV_TMR10_IRQn;
			else if (TMRx == TMR2)
				irq = TMR2_GLOBAL_IRQn;
			else if (TMRx == TMR3)
				irq = TMR3_GLOBAL_IRQn;
			else if (TMRx == TMR4)
				irq = TMR4_GLOBAL_IRQn;
			else if (TMRx == TMR5)
				irq = TMR5_GLOBAL_IRQn;
			else if (TMRx == TMR8)
				irq = TMR8_OV_TMR13_IRQn;

			NVIC_InitStructure.NVIC_IRQChannel                      = irq;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = _PreemptionPriority;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority           = _SubPriority;
			NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
			NVIC_Init(&NVIC_InitStructure);
		}		
		return;
	}

	uiTMRxCLK = SystemCoreClock/2;

	if (_ulFreq < 100)
	{
		usPrescaler = 10000 - 1;					
		usPeriod =  (uiTMRxCLK / 10000) / _ulFreq  - 1;	
	}
	else if (_ulFreq < 3000)
	{
		usPrescaler = 100 - 1;					
		usPeriod =  (uiTMRxCLK / 100) / _ulFreq  - 1;		
	}
	else	
	{
		usPrescaler = 0;				
		usPeriod = uiTMRxCLK / _ulFreq - 1;	
	}

	TMR_TimeBaseStructure.TMR_Period            = usPeriod;
	TMR_TimeBaseStructure.TMR_DIV               = usPrescaler;
	TMR_TimeBaseStructure.TMR_ClockDivision     = 0;
	TMR_TimeBaseStructure.TMR_CounterMode       = TMR_CounterDIR_Up;
	TMR_TimeBaseStructure.TMR_RepetitionCounter = 0;

	TMR_TimeBaseInit(TMRx, &TMR_TimeBaseStructure);

	TMR_ARPreloadConfig(TMRx, ENABLE);

	TMR_INTConfig(TMRx, TMR_INT_Overflow, ENABLE);

	TMR_Cmd(TMRx, ENABLE);

    //configure NVIC for TMRx
	{
		NVIC_InitType NVIC_InitStructure;	
		uint8_t irq = 0;	

		if (TMRx == TMR1)
			irq = TMR1_OV_TMR10_IRQn;
		else if (TMRx == TMR2)
			irq = TMR2_GLOBAL_IRQn;
		else if (TMRx == TMR3)
			irq = TMR3_GLOBAL_IRQn;
		else if (TMRx == TMR4)
			irq = TMR4_GLOBAL_IRQn;
		else if (TMRx == TMR5)
			irq = TMR5_GLOBAL_IRQn;
		else if (TMRx == TMR8)
			irq = TMR8_OV_TMR13_IRQn;

		NVIC_InitStructure.NVIC_IRQChannel = irq;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = _PreemptionPriority;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = _SubPriority;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
	}
}

