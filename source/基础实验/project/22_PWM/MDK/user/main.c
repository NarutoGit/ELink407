#include "stm32f4xx.h"
#include "uart.h"
#include "pwm.h"
#include "delay.h"

int main()
{
	u16 led0pwmval=0;       
	u8 dir=1; 

	init_uart();
	init_delay();
	
	timer_pwm_init(500-1,84-1);  //��ʱ��1ʱ��Ϊ168M����Ƶϵ��Ϊ168�����Լ���Ƶ�� 
	//Ϊ168M/168=1Mhz,��װ��ֵ500������PWMƵ��Ϊ:1M/500=2Khz,��������Ϊ��1/2000=0.5ms
	while(1)    
	{ 
		delay_ms(10);    
		if(dir)
			led0pwmval++;//dir==1 led0pwmval ���� 
		else 
			led0pwmval--; //dir==0 led0pwmval�ݼ�   
		if(led0pwmval>300){
			dir=0;//led0pwmval ����300�󣬷���Ϊ�ݼ� 
		}
		if(led0pwmval==0){
			dir=1;  //led0pwmval �ݼ���0�󣬷����Ϊ���� 
		}
		TIM_SetCompare3(TIM4,led0pwmval);  //�޸ıȽ�ֵ���޸�ռ�ձ� 
	} 
}
