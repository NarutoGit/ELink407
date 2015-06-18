#include "stm32f4xx.h"
#include "led.h"
#include "button_noexti.h"
#include "delay.h"
#include "stdio.h"
#include "uart.h"
#include "iwdg.h"

#include "wwdg.h"

//ģ��һ�η�����ָ���
void error_code()
{
	printf("ģ���������������,��ʱ3s��ι�����۲쿴�Ź���λ\r\n");
	mydelay(3000);  

}

int main()
{
	int button_value=0;
	
	init_led();
	
	init_delay();
	
	init_uart();
	
	init_button();
	
	printf("ϵͳ��λ�������������Ź�\r\n");
	
	init_iwdg(4,1500); //���Ƶ��Ϊ 64,����ֵΪ 500,���ʱ��Ϊ 1s
	
	init_wwdg(0x7F,0X5F,WWDG_Prescaler_8);
	
	while(1)
	{
		
		feed_iwdg();
		
		button_value=button_read();
	
		if(button_value>0)
		{
			mydelay(1);		//��ʱȥ��
		
			if(button_value==button_read())
			{
					switch(button_value)
					{
						case 1:
							led_on(LED1);
							printf("k1 ������\r\n");
							error_code();
							break;
					
						case 2:
							led_on(LED2);
							printf("k2 ������,�رմ��ڿ��Ź��ж�\r\n");
							wwdg_IRQ(DISABLE);
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
	
		mydelay(1000);
		
	}

	
	
}


