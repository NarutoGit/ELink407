#include "button.h"
#include "button_noexti.h"
#include "led.h"
#include "delay.h"
#include "stm32f4xx.h"

//��ʼ��PD6��PE2~4Ϊ�ж�����
void EXTIX_Init(void)
{
	NVIC_InitTypeDef    NVIC_InitStructure;
	EXTI_InitTypeDef    EXTI_InitStructure;
	
	init_button();//������Ӧ�� IO �ڳ�ʼ��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);			 //ʹ�� SYSCFG ʱ��
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);	//PE2 ������ 2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);	//PE3 ������ 3
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);	//PE4 ������ 4
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD, EXTI_PinSource6);	//PD6 ������ 6
	
	/*  ���� EXTI_Line6 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line6;						//LINE0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; 			//�����ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						//ʹ�� LINE0
	EXTI_Init(&EXTI_InitStructure);

	/*  ���� EXTI_Line2,3,4 */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line4;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;				//�ж��¼�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   		//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						//�ж���ʹ��
	EXTI_Init(&EXTI_InitStructure);									//����
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;				//�ⲿ�ж� 0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0x01;		//��ռ���ȼ� 1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//��Ӧ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);									//���� NVIC
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;				//�ⲿ�ж� 2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//��Ӧ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);									//���� NVIC
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;				//�ⲿ�ж� 3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;	//��ռ���ȼ� 3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//��Ӧ���ȼ� 2 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);									//���� NVIC
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;				//�ⲿ�ж� 4
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x04;	//��ռ���ȼ� 4
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;			//��Ӧ���ȼ� 2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);									//����  NVIC 
	
}


//PIN6�ⲿ�ж� 9_5 �������
void EXTI9_5_IRQHandler(void)
{ 
	mydelay(10); //����
	if(1==button_read()) 
	{ 
		led_on(LED1); //LED1 ��
	} 
	EXTI_ClearITPendingBit(EXTI_Line6); //��� LINE0 �ϵ��жϱ�־λ 
}

//�ⲿ�ж� 2 �������
void EXTI2_IRQHandler(void)
{ 
	mydelay(10); //����
	if(2==button_read()) 
	{  
		led_on(LED2); //LED2 ��
	} 
	EXTI_ClearITPendingBit(EXTI_Line2);//��� LINE2 �ϵ��жϱ�־λ 
}

//�ⲿ�ж� 3 �������
void EXTI3_IRQHandler(void)
{ 
	mydelay(10); //����
	if(4==button_read()) 
	{ 
		led_on(LED1);		//LED1 ��
		led_on(LED2);		//LED2 ��
	} 
	EXTI_ClearITPendingBit(EXTI_Line3);   //��� LINE3 �ϵ��жϱ�־λ 
}

//�ⲿ�ж� 4 �������
void EXTI4_IRQHandler(void)
{
	mydelay(10); //����
	
	if(8==button_read()) 
	{  
		led_off(LED1);		//LED1 ��
		led_off(LED2);		//LED2 ��
	} 
	EXTI_ClearITPendingBit(EXTI_Line4);//��� LINE4 �ϵ��жϱ�־λ 
}  

