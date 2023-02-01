/**
  **************************************************************************
  * File   : key.h
  * Version: V1.2.9
  * Date   : 2021-01-15
  * Brief  : none
  **************************************************************************
  */
 
#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pins_4)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pins_3)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pins_2)//��ȡ����2 
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pins_0)//��ȡ����3(WK_UP) 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����
#define KEY2_PRES	3	//KEY2����
#define WKUP_PRES   4	//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif