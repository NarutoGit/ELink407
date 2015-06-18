#include "stm32f4xx.h"
#include "stdio.h"

u8 WWDG_CNT = 0X7F;

void wwdg_IRQ(FunctionalState NewState)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel=WWDG_IRQn;   		//���ڿ��Ź��ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02;//��ռ���ȼ�Ϊ 2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03;    	//��Ӧ���ȼ�Ϊ 3
	NVIC_InitStructure.NVIC_IRQChannelCmd=NewState;   		//ʹ�ܴ��ڿ��Ź�
	NVIC_Init(&NVIC_InitStructure);

}

void init_wwdg(u8 tr,u8 wr,u32 fprer)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG,ENABLE); 	//ʹ�ܴ��ڿ��Ź�ʱ��
	
	WWDG_CNT=tr&WWDG_CNT;    								//��ʼ�� WWDG_CNT. 
	WWDG_SetPrescaler(fprer);								//���÷�Ƶֵ
	WWDG_SetWindowValue(wr); 								//���ô���ֵ
	WWDG_SetCounter(WWDG_CNT);								//���ü���ֵ
	WWDG_Enable(WWDG_CNT);   								//�������Ź�

	wwdg_IRQ(ENABLE);

	WWDG_ClearFlag();										//�����ǰ�����жϱ�־λ
	WWDG_EnableIT();										//������ǰ�����ж�
}

//���ڿ��Ź��жϷ������ 
void WWDG_IRQHandler(void)
{
	WWDG_SetCounter (WWDG_CNT); 							//���贰�ڿ��Ź�ֵ
	WWDG_ClearFlag();										//�����ǰ�����жϱ�־λ
	
	printf("ι���ڿ��Ź�\r\n");
}

