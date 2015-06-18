#ifndef __DHT11_H
#define __DHT11_H 
#include "delay.h" 
#include "stm32f4xx.h"


//IO�����������

#define DHT11_IO_IN()  {GPIOB->MODER&=~(3<<(0*2));GPIOB->MODER|=0<<0*2;}	//PB0����ģʽ
#define DHT11_IO_OUT() {GPIOB->MODER&=~(3<<(0*2));GPIOB->MODER|=1<<0*2;} 	//PB0���ģʽ 

u8 init_DHT11(void);	//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);	//����Ƿ����DHT11
void DHT11_Rst(void);	//��λDHT11    
#endif















