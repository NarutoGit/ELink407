#include "stm32f4xx.h"
#include "stdio.h"

#include "uart.h"

int main()
{

	char helpstr[]=" \
------------------------------\r\n \
|                            |\r\n \
|      helloworld .v1.0      |\r\n \
|      'h'  --> 'hello'      |\r\n \
|      'w'  --> 'world'      |\r\n \
|                            |\r\n \
------------------------------\r\n" ;
	
	uint8_t cmdchar;  /* �������� */ 
	
	init_uart();   /* ���ڳ�ʼ�� */ 
	
	printf("%s",helpstr);	
		
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
