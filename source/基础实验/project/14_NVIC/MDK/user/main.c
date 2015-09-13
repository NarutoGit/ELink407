#include "led.h"
#include "button.h"
#include "button_noexti.h"
#include "stm32f4xx.h"
#include "uart.h"
#include "delay.h"

typedef  unsigned char uint8_t;
typedef  unsigned int uint32_t;
/* �������ļ��ڵ��õĺ������� */

void Delay(uint32_t nCount);

 __IO uint8_t ubPreemptionOccurred;
 __IO uint8_t ubPreemptionPriorityValue = 0;

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	
	init_uart(); 	/* ��ʼ������ */
	init_button();
	EXTIX_Init();
	init_delay();
	/* ��Բ�ͬ��Ӧ�ó��������Ҫ�ĵײ�����ģ���ʼ������ */

	init_led(); 		/* ��ʼLEDָʾ�ƶ˿� */
	

	/* ������શ�ʱ������ռ���ȼ��������ȼ� */
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), !ubPreemptionPriorityValue, 0));
	/* ����������ѭ���� */
	while (1)
	{
		if(ubPreemptionOccurred != 0)
		{
			/* ��ת LED1 */
			Led_toggle(1);
			Delay(0x5FFFF);

			/* ��ת LED2 */
			Led_toggle(2);
			Delay(0x5FFFF);

		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: Delay
*	����˵��: �ӳٺ���
*	��    �Σ�nCount  �ӳټ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void Delay( uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

