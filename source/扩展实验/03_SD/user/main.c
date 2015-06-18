#include "stdio.h"
#include "delay.h"  
#include "uart.h"   
#include "led.h"
#include "button_noexti.h"    
#include "malloc.h" 

#include "sdio_sdcard.h"    

//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  	printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}

int main(void)
{        
	u8 key;		 
	u32 sd_size;
	u8 t=0,ledblink=0;	
	u8 *buf;

	
	init_delay();			//��ʱ��ʼ��  
	init_uart();		//��ʼ�����ڲ�����Ϊ115200 
	init_led();					//��ʼ��LED  
 	init_button();					//������ʼ��  
	
	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ�� 
	my_mem_init(SRAMCCM);		//��ʼ��CCM�ڴ��
	
 	 
	printf("Card test\r\n");
	 
 	while(SD_Init())//��ⲻ��SD��
	{
		printf("SD Card Error\r\n");
		
		delay_ms(500);					
		
		ledblink^=1;
		if(ledblink)
			led_on(LED1);//��ʾϵͳ��������
		else
			led_off(LED1);
	}
	show_sdcard_info();	//��ӡSD�������Ϣ
 	 
	//���SD���ɹ�
	printf("SD Card Size:%d\r\n",(int)SDCardInfo.CardCapacity);	
	
	while(1)
	{
		key=button_read();
		if((1<<0)==key)//KEY0����,����һ������
		{
			buf=mymalloc(0,512);		//�����ڴ�
			if(SD_ReadDisk(buf,0,1)==0)	//��ȡ0����������
			{	
				printf("��ȡ0����������\r\n");
				
				printf("SECTOR 0 DATA:\r\n");
				for(sd_size=0;sd_size<512;sd_size++)printf("%x ",buf[sd_size]);//��ӡ0��������    	   
				printf("\r\nDATA ENDED\r\n");
				
			}
			myfree(0,buf);//�ͷ��ڴ�	   
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
		}
	} 
}
















