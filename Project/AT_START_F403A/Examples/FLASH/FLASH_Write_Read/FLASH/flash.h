#ifndef __FLASH_H__
#define __FLASH_H__

#include "at32_board.h"
#include "stdio.h"

#define FLASH_SIZE 1024			//��ѡAT32F4xx��FLASH������С(��λΪK)
#define FLASH_WREN 1				//ʹ��FLASHд��(0��������;1��ʹ��)

uint16_t FLASH_ReadHalfWord(uint32_t faddr);		  //��������  
void FLASH_WriteLenByte(uint32_t WriteAddr,uint32_t DataToWrite,uint16_t Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 FLASH_ReadLenByte(uint32_t ReadAddr,u16 Len);										//ָ����ַ��ʼ��ȡָ����������
void FLASH_Write(uint32_t WriteAddr,uint16_t *pBuffer,uint16_t NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void FLASH_Read(uint32_t ReadAddr,uint16_t *pBuffer,uint16_t NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

#endif

















