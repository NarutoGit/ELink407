#include "led.h"

void init_led(void)
{
	
	
	RCC->AHB1ENR|=1<<0;									//ʹ��PORTAʱ�� 
	RCC->AHB1ENR|=1<<1;									//ʹ��PORTBʱ�� 
	
	GPIOA_Reg->	MODER&=~(3<<(8*2));  		//����� PA8 ԭ��������
	GPIOA_Reg->	MODER|=1<<(8*2);    		//���� PA8 Ϊ���ģʽ
	GPIOA_Reg->	OTYPER&=~(1<<8) ;    		//��� PA8 ԭ��������
	GPIOA_Reg->	OTYPER|=0<<8;      			//���� PA8 Ϊ�������
	GPIOA_Reg-> OSPEEDR&=~(3<<(8*2)); 	//����� PA8 ԭ��������
	GPIOA_Reg-> OSPEEDR|=3<<(8*2);    	//���� PA8 ����ٶ�Ϊ 100Mhz 
	GPIOA_Reg-> PUPDR&=~(3<<(8*2));  		//����� PA8 ԭ��������
	GPIOA_Reg-> PUPDR|=0<<(8*2);    		//���� PA8 ����������
	GPIOA_Reg->	ODR|=1<<8;        			//���� PA8 ��� 1���ߵ�ƽ,LED1��ʼΪ�ر�״̬��
	
	GPIOB_Reg->	MODER&=~(3<<(8*2));  		//����� PB8 ԭ��������
	GPIOB_Reg->	MODER|=1<<(8*2);    		//���� PB8 Ϊ���ģʽ
	GPIOB_Reg->	OTYPER&=~(1<<8) ;    		//��� PB8 ԭ��������
	GPIOB_Reg->	OTYPER|=0<<8;      			//���� PB8 Ϊ�������
	GPIOB_Reg-> OSPEEDR&=~(3<<(8*2)); 	//����� PB8 ԭ��������
	GPIOB_Reg-> OSPEEDR|=3<<(8*2);    	//���� PB8 ����ٶ�Ϊ 100Mhz 
	GPIOB_Reg-> PUPDR&=~(3<<(8*2));  		//����� PB8 ԭ��������
	GPIOB_Reg-> PUPDR|=0<<(8*2);    		//���� PB8 ����������
	GPIOB_Reg->	ODR|=1<<8;        			//���� PB8 ��� 1���ߵ�ƽ,LED1��ʼΪ�ر�״̬��
	
	
}


void led_on(int ledx)
{
#ifdef	BIT_BAND_ENB	
	if(LED1==ledx)
	{
		PAout(8)=0;        								//���� PA8 ��� 0���͵�ƽ,LED1����
	
	}
	else if(LED2==ledx)
	{
		PBout(8)=0;;        							//���� PB8 ��� 0���͵�ƽ,LED2����
		
	}
	else
	{
		;																	//ledx �����ڴ���
	}
	
#else
	
	if(LED1==ledx)
	{
		GPIOA_Reg->	ODR &=~(1<<8);        //���� PA8 ��� 0���͵�ƽ,LED1����
	
	}
	else if(LED2==ledx)
	{
		GPIOB_Reg->	ODR &=~(1<<8);        //���� PB8 ��� 0���͵�ƽ,LED2����
		
	}
	else
	{
		;																	//ledx �����ڴ���
	}
	

#endif
}

void led_off(int ledx)
{
#ifdef	BIT_BAND_ENB	
	
	if(LED1==ledx)
	{
		PAout(8)=1;        								//���� PA8 ��� 1���ߵ�ƽ,LED1��
	
	}
	else if(LED2==ledx)
	{
		PBout(8)=1;        								//���� PB8 ��� 1���ߵ�ƽ,LED2��
		
	}
	else
	{
		;																	//ledx �����ڴ���
	}
	
	#else
	
	if(LED1==ledx)
	{
		GPIOA_Reg->	ODR|=1<<8;        		//���� PA8 ��� 1���ߵ�ƽ,LED1��
	
	}
	else if(LED2==ledx)
	{
		GPIOB_Reg->	ODR|=1<<8;        		//���� PB8 ��� 1���ߵ�ƽ,LED2��
		
	}
	else
	{
		;																	//ledx �����ڴ���
	}
	
	
#endif
	
}   
