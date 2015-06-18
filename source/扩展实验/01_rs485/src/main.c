#include "delay.h"  
#include "uart.h"   
#include "led.h"
#include "button.h"  
#include "button_noexti.h"  
#include "rs485.h"
#include "stm32f4xx.h"


int main(void)
{   
	u8 key;
	u8 i=0,t=0,ledblink=0;
	u8 cnt=0;
	u8 rs485buf[5]; 
	
	init_delay();			//��ʱ��ʼ��  
	init_uart();		//��ʼ�����ڲ�����Ϊ115200  	 
	init_led();					//��ʼ��LED 
	init_button(); 				//������ʼ��  

	RS485_Init(42,9600);		//��ʼ��RS485
	 									  
	while(1)
	{
		key=button_read();
		if((1<<0)==key)//KEY0����,����һ������
		{
			for(i=0;i<5;i++)
			{
				rs485buf[i]=cnt+i;//��䷢�ͻ�����
 			}
			RS485_Send_Data(rs485buf,5);//����5���ֽ� 									   
		}	
		
		RS485_Receive_Data(rs485buf,&key);
		if(key)//���յ�������
		{
			if(key>5)
				key=5;//�����5������.
 			for(i=0;i<key;i++)
				;
 		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			ledblink^=1;
			if(ledblink)
				led_on(LED1);//��ʾϵͳ��������
			else
				led_off(LED1);
			t=0;
			cnt++;
		}		   
	}   
}

















