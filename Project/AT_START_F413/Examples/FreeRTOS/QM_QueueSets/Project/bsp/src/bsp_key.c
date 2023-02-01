
#include "bsp.h"

#define RCC_ALL_KEY 	RCC_APB2PERIPH_GPIOA 

//High level is valid
#define GPIO_PORT_K1    GPIOA
#define GPIO_PIN_K1	    GPIO_Pins_0

static KEY_T s_tBtn[KEY_COUNT];
static KEY_FIFO_T s_tKey;		

static void bsp_InitKeyVar(void);
static void bsp_InitKeyHard(void);
static void bsp_DetectKey(uint8_t i);

//1:pressed, 0: no pressed
static uint8_t IsKeyDown1(void)
{
	if ((GPIO_PORT_K1->IPTDT & GPIO_PIN_K1) == 1)
		return 1;
	else 
		return 0;
}

void bsp_InitKey(void)
{
	bsp_InitKeyVar();		
	bsp_InitKeyHard();		
}

//store one key value to key FIFO
void bsp_PutKey(uint8_t _KeyCode)
{
	s_tKey.Buf[s_tKey.Write] = _KeyCode;

	if (++s_tKey.Write  >= KEY_FIFO_SIZE)
	{
		s_tKey.Write = 0;
	}
}

//obtain one key value from key FIFO
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_tKey.Read == s_tKey.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_tKey.Buf[s_tKey.Read];

		if (++s_tKey.Read >= KEY_FIFO_SIZE)
		{
			s_tKey.Read = 0;
		}
		return ret;
	}
}

//obtain key state. 1: pressed, 0: no pressed
uint8_t bsp_GetKeyState(KEY_ID_E _ucKeyID)
{
	return s_tBtn[_ucKeyID].State;
}

void bsp_SetKeyParam(uint8_t _ucKeyID, uint16_t _LongTime, uint8_t  _RepeatSpeed)
{
	s_tBtn[_ucKeyID].LongTime       = _LongTime;		//0: don't detect long pressed event
	s_tBtn[_ucKeyID].RepeatSpeed    = _RepeatSpeed;		//0: don't support continuous send key value
	s_tBtn[_ucKeyID].RepeatCount    = 0;						
}


//clear key FIFO
void bsp_ClearKey(void)
{
	s_tKey.Read = s_tKey.Write;
}

//initialize GPIO for keys
static void bsp_InitKeyHard(void)
{
	GPIO_InitType GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_ALL_KEY, ENABLE);

	GPIO_InitStructure.GPIO_MaxSpeed    = GPIO_MaxSpeed_50MHz;
	GPIO_InitStructure.GPIO_Mode        = GPIO_Mode_IN_PD;	
	
	GPIO_InitStructure.GPIO_Pins = GPIO_PIN_K1;
	GPIO_Init(GPIO_PORT_K1, &GPIO_InitStructure);
}

//initialize key and FIFO structure
static void bsp_InitKeyVar(void)
{
	uint8_t i;

	//clear FIFO operation pointer
	s_tKey.Read     = 0;
	s_tKey.Write    = 0;

	for (i = 0; i < KEY_COUNT; i++)
	{
		s_tBtn[i].LongTime = KEY_LONG_TIME;			
		s_tBtn[i].Count = KEY_FILTER_TIME / 2;		
		s_tBtn[i].State = 0;							
		s_tBtn[i].RepeatSpeed = 0;						
		s_tBtn[i].RepeatCount = 0;						
	}

	s_tBtn[KID_K1].IsKeyDownFunc = IsKeyDown1;
}

//scan the specified key
static void bsp_DetectKey(uint8_t i)
{
	KEY_T *pBtn;

	pBtn = &s_tBtn[i];
	if (pBtn->IsKeyDownFunc())//key pressed
	{
		if (pBtn->Count < KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count < 2 * KEY_FILTER_TIME)
		{
			pBtn->Count++;
		}
		else
		{
			if (pBtn->State == 0)
			{
				pBtn->State = 1;

				
				bsp_PutKey((uint8_t)(3 * i + 1));
			}

			if (pBtn->LongTime > 0)
			{
				if (pBtn->LongCount < pBtn->LongTime)
				{
					
					if (++pBtn->LongCount == pBtn->LongTime)
					{
						
						bsp_PutKey((uint8_t)(3 * i + 3));
					}
				}
				else
				{
					if (pBtn->RepeatSpeed > 0)
					{
						if (++pBtn->RepeatCount >= pBtn->RepeatSpeed)
						{
							pBtn->RepeatCount = 0;
							
							bsp_PutKey((uint8_t)(3 * i + 1));
						}
					}
				}
			}
		}
	}
	else //key up
	{
		if(pBtn->Count > KEY_FILTER_TIME)
		{
			pBtn->Count = KEY_FILTER_TIME;
		}
		else if(pBtn->Count != 0)
		{
			pBtn->Count--;
		}
		else
		{
			if (pBtn->State == 1)
			{
				pBtn->State = 0;

				
				bsp_PutKey((uint8_t)(3 * i + 2));
			}
		}

		pBtn->LongCount = 0;
		pBtn->RepeatCount = 0;
	}
}

//scan all key
void bsp_KeyScan(void)
{
	uint8_t i;

	for (i = 0; i < KEY_COUNT; i++)
	{
		bsp_DetectKey(i);
	}
}

