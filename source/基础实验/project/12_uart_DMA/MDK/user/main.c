#include "stm32f4xx.h"
#include "stdio.h"

#include "uart.h"


#define STRLEN 300

int main()
{

	char helpstr[STRLEN]=" \
------------------------------\r\n \
|                            |\r\n \
|      helloworld .v1.0      |\r\n \
|      'h'  --> 'hello'      |\r\n \
|      'w'  --> 'world'      |\r\n \
|                            |\r\n \
------------------------------\r\n" ;
	
	uint8_t cmdchar;  /* �������� */ 
	
	
	
	init_uart();   /* ���ڳ�ʼ�� */ 
	
	//printf("%s",helpstr);
		
	/* ʹ�� DMA clock */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 
	
	/* ʹ�� USART DMA TX ���� */ 
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE); 
	

	USART_DMA_TxConfig((uint32_t *)helpstr, STRLEN); 
   /* �ȴ����ݴ�����ɣ�����������Բ����õģ�ֱ���������DMA�ȴ����� */ 
   
	while(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7)==RESET); 
   /* ���DMA������ɱ�־ */ 
   DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7); 
   /* ������ڴ�����ɱ�־ */ 
   USART_ClearFlag(USART1,USART_FLAG_TC); 	
		
	while(1)
	{
		
		 
		printf("#");
		scanf("%c",&cmdchar);
		
		if(cmdchar)
		{
		
			switch(cmdchar)
			{
				case 'h':
					printf("hello \r\n");
					break;
			
				case 'w':
					printf("world \r\n");
					break;
			
				default:
					printf("command error \r\n");
					printf("%s",helpstr);
					break;
			}
		}
	}
	
}
