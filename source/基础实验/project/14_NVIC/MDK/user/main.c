#include "stm32f4xx.h"
#include "led.h"
#include "delay.h"
#include "button.h"

#include "button_noexti.h"

int main()
{
	int button_value=0;
	
	init_led();
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ����� 2
	 
	EXTIX_Init();       //��ʼ���ⲿ�ж�����
	
	
		while(1)
		{
		
#if 0
		button_value=button_read();
	
		if(button_value>0)
		{
			mydelay(10);		//��ʱȥ��
		
			if(button_value==button_read())
			{
					switch(button_value)
					{
						case 1:
							led_on(LED1);
							break;
					
						case 2:
							led_on(LED2);
							break;
					
						case 4:
							led_on(LED1);
							led_on(LED2);
							break;
					
						case 8:
							led_off(LED1);
							led_off(LED2);
							break;
					
						default:			//������ͬʱ���£�������
							break;
					}
				}
		}
	#endif
		mydelay(1000);
		
	}

	
	
}


