#include "stm32f4xx.h"
#include "stdio.h"
#include "delay.h"  
#include "uart.h"   
#include "led.h"
#include "button.h"  
#include "button_noexti.h" 

#include "can.h"

    
int main(void)
{   
	u8 key;
	u8 i=0,t=0,ledblink=0;
	u8 cnt=0;
	u8 canbuf[8];
	u8 res;
	u8 mode=1;//CAN����ģʽ;0,��ͨģʽ;1,����ģʽ
	
	init_delay();			//��ʱ��ʼ��  
	init_uart();		//��ʼ�����ڲ�����Ϊ115200  	 
	init_led();					//��ʼ��LED 
 	init_button(); 				//������ʼ��  
 	
	CAN1_Mode_Init(1,6,7,6,1);	//CAN��ʼ��,������500Kbps    

 		
 	while(1)
	{
		key=button_read();
		if((1<<0)==key)//KEY0����,����һ������
		{
			for(i=0;i<8;i++)
			{
				canbuf[i]=cnt+i;//��䷢�ͻ�����
				printf("sentData:0x%x",canbuf[i]);
 			}
			res=CAN1_Send_Msg(canbuf,8);//����8���ֽ� 
			if(res)printf("Failed\r\n");		//��ʾ����ʧ��
			else printf("OK\r\n"); 	 		//��ʾ���ͳɹ�								   
		}else if((1<<1)==key)//KEY1���£��ı�CAN�Ĺ���ģʽ
		{	   
			mode=!mode;
			CAN1_Mode_Init(1,6,7,6,mode);	//CAN��ͨģʽ��ʼ��,��ͨģʽ,������500Kbps
  			 
			if(mode==0)//��ͨģʽ����Ҫ2��������
			{
				printf("Nnormal Mode \r\n");
					    
			}else //�ػ�ģʽ,һ��������Ϳ��Բ�����.
			{
				printf("LoopBack Mode\r\n");
 				
			}
 			
		}		 
		key=CAN1_Receive_Msg(canbuf);
		if(key)//���յ�������
		{			
			
 			for(i=0;i<key;i++)
			{									    
				printf("rev :%d\r\n",canbuf[i]);
			}
				
		}
		t++; 
		delay_ms(10);
		if(t==20)
		{
			ledblink^=1;
			if(ledblink)
				led_on(LED1);//��ʾϵͳ��������	
			t=0;
			cnt++;
			
		}		   
	} 
}
