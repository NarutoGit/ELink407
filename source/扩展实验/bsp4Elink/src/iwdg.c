#include "iwdg.h"
#include "stdio.h"
#include "stm32f4xx_iwdg.h"

//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е� 3 λ��Ч!)    rlr:�Զ���װ��ֵ,0~0XFFF.
//��Ƶ����=4*2^prer.�����ֵֻ���� 256!
//rlr:��װ�ؼĴ���ֵ:�� 11 λ��Ч. 

//ʱ�����(���):Tout=((4*2^prer)*rlr)/32 (ms).

void init_iwdg(unsigned char prer,short rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); 	//ȡ���Ĵ���д����
	IWDG_SetPrescaler(prer); 						//���� IWDG ��Ƶϵ��
	IWDG_SetReload(rlr);    						//���� IWDG װ��ֵ
	IWDG_ReloadCounter(); 							//reload
	IWDG_Enable();        							//ʹ�ܿ��Ź�
}

//ι�������Ź�
void feed_iwdg(void)
{
	printf("ι��\r\n");
	IWDG_ReloadCounter();//reload
}
