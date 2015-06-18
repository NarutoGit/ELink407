#include "rng.h"
#include "delay.h"
#include "stm32f4xx.h"

int init_rng(void)
{
	u16 retry=0; 
	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);  //���� RNG ʱ��   
	RNG_Cmd(ENABLE);  //ʹ�� RNG
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET&&retry<10000)//�ȴ�����
	{  
		retry++; 
		delay_ms(1); 
	}
	if(retry>=10000)return 1;//���������������������
	return 0;
}

//�õ������
//����ֵ:��ȡ���������
unsigned int get_RandomNum(void)
{ 
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)==RESET);  //�ȴ����������
	return RNG_GetRandomNumber(); 
}

//����[min,max]��Χ�������
unsigned int get_RandomRange(int min,int max)
{ 
	return get_RandomNum()%(max-min+1) +min;
}

