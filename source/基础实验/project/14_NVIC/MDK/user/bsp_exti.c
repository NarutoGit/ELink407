#include "stm32f4xx.h"

//��ʼ��PD6��PE2~4Ϊ�ж�����
void EXTIX_Init(void)
{
	NVIC_InitTypeDef    NVIC_InitStructure;
	EXTI_InitTypeDef    EXTI_InitStructure;
	
	//init_button();//������Ӧ�� IO �ڳ�ʼ��
	
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



void bsp_InitKeyEXTI(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* ʹ��SYSCFGʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	/* ���� EXTI Line8 �� PI8 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource8);
	/* ���� EXTI Line13 �� PC8 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);
	/* ���� EXTI Line11 �� PI11 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOI, EXTI_PinSource11);
	/* ���� EXTI Line3 �� PH3 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	/* ���� EXTI Line15 �� PH15 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOH, EXTI_PinSource15);
	/* ���� EXTI Line7 �� PG7 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource7);
	/* ���� EXTI Line2 �� PH2 ���� */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	/* ���� EXTI Line11 �� PF11 ���� */
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);
	
	/* ���� EXTI LineXXX */
	EXTI_InitStructure.EXTI_Line = EXTI_Line2 | EXTI_Line3 | EXTI_Line7 | EXTI_Line8 \
	                              |EXTI_Line11| EXTI_Line13| EXTI_Line15;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	
 	/* ����NVIC���ȼ�����ΪGroup1��0-1��ռʽ���ȼ���0-7����Ӧʽ���ȼ� */
 	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	/* �ж����ȼ����� ������ȼ� ����һ��Ҫ�ֿ��������жϣ����ܹ��ϲ���һ���������� */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* K1���� PI8*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* K2���� PC13*/
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}
