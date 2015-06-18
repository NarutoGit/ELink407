#include "stm32f4xx.h"
#include "delay.h"  
#include "uart.h"   
#include "led.h"
#include "button_noexti.h"  
 
#include "malloc.h" 
#include "w25qxx.h"    
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	 
#include "wm8978.h"	 
#include "audioplay.h"	


 
int main(void)
{        
 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	init_delay();  //��ʼ����ʱ����
	init_uart();		//��ʼ�����ڲ�����Ϊ115200
	init_led();					//��ʼ��LED 
	//usmart_dev.init(84);		//��ʼ��USMART
 	
 	init_button();					//������ʼ��  
	
	WM8978_Init();				//��ʼ��WM8978
	WM8978_HPvol_Set(40,40);	//������������
	WM8978_SPKvol_Set(50);		//������������
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ�� 
	exfuns_init();				//Ϊfatfs��ر��������ڴ�  
  	f_mount(fs[0],"0:",1); 		//����SD��  
	   
    delay_ms(1000);	

	while(1)
	{ 
		audio_play();
	} 
}







