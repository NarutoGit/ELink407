#include "rs485.h"	 
#include "delay.h"
#include "stm32f4xx.h"


//GPIO��������
//GPIOx:GPIOA~GPIOI.
//BITx:0~15,����IO���ű��.
//AFx:0~15,����AF0~AF15.
//AF0~15�������(��������г����õ�,��ϸ�����407�����ֲ�,56ҳTable 7):
//AF0:MCO/SWD/SWCLK/RTC   AF1:TIM1/TIM2;            AF2:TIM3~5;               AF3:TIM8~11
//AF4:I2C1~I2C3;          AF5:SPI1/SPI2;            AF6:SPI3;                 AF7:USART1~3;
//AF8:USART4~6;           AF9;CAN1/CAN2/TIM12~14    AF10:USB_OTG/USB_HS       AF11:ETH
//AF12:FSMC/SDIO/OTG/HS   AF13:DCIM                 AF14:                     AF15:EVENTOUT
void GPIO_AF_Set(GPIO_TypeDef* GPIOx,u8 BITx,u8 AFx)
{  
	GPIOx->AFR[BITx>>3]&=~(0X0F<<((BITx&0X07)*4));
	GPIOx->AFR[BITx>>3]|=(u32)AFx<<((BITx&0X07)*4);
}  

//GPIOͨ������ 
//GPIOx:GPIOA~GPIOI.
//BITx:0X0000~0XFFFF,λ����,ÿ��λ����һ��IO,��0λ����Px0,��1λ����Px1,��������.����0X0101,����ͬʱ����Px0��Px8.
//MODE:0~3;ģʽѡ��,0,����(ϵͳ��λĬ��״̬);1,��ͨ���;2,���ù���;3,ģ������.
//OTYPE:0/1;�������ѡ��,0,�������;1,��©���.
//OSPEED:0~3;����ٶ�����,0,2Mhz;1,25Mhz;2,50Mhz;3,100Mh. 
//PUPD:0~3:����������,0,����������;1,����;2,����;3,����.
//ע��:������ģʽ(��ͨ����/ģ������)��,OTYPE��OSPEED������Ч!!
void GPIO_Set(GPIO_TypeDef* GPIOx,u32 BITx,u32 MODE,u32 OTYPE,u32 OSPEED,u32 PUPD)
{  
	u32 pinpos=0,pos=0,curpin=0;
	for(pinpos=0;pinpos<16;pinpos++)
	{
		pos=1<<pinpos;	//һ����λ��� 
		curpin=BITx&pos;//��������Ƿ�Ҫ����
		if(curpin==pos)	//��Ҫ����
		{
			GPIOx->MODER&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->MODER|=MODE<<(pinpos*2);	//�����µ�ģʽ 
			if((MODE==0X01)||(MODE==0X02))	//��������ģʽ/���ù���ģʽ
			{  
				GPIOx->OSPEEDR&=~(3<<(pinpos*2));	//���ԭ��������
				GPIOx->OSPEEDR|=(OSPEED<<(pinpos*2));//�����µ��ٶ�ֵ  
				GPIOx->OTYPER&=~(1<<pinpos) ;		//���ԭ��������
				GPIOx->OTYPER|=OTYPE<<pinpos;		//�����µ����ģʽ
			}  
			GPIOx->PUPDR&=~(3<<(pinpos*2));	//�����ԭ��������
			GPIOx->PUPDR|=PUPD<<(pinpos*2);	//�����µ�������
		}
	}
} 


//����NVIC����
//NVIC_Group:NVIC���� 0~4 �ܹ�5�� 		   
void MY_NVIC_PriorityGroupConfig(u8 NVIC_Group)	 
{ 
	u32 temp,temp1;	  
	temp1=(~NVIC_Group)&0x07;//ȡ����λ
	temp1<<=8;
	temp=SCB->AIRCR;  //��ȡ��ǰ������
	temp&=0X0000F8FF; //�����ǰ����
	temp|=0X05FA0000; //д��Կ��
	temp|=temp1;	   
	SCB->AIRCR=temp;  //���÷���	    	  				   
}

//����NVIC 
//NVIC_PreemptionPriority:��ռ���ȼ�
//NVIC_SubPriority       :��Ӧ���ȼ�
//NVIC_Channel           :�жϱ��
//NVIC_Group             :�жϷ��� 0~4
//ע�����ȼ����ܳ����趨����ķ�Χ!����������벻���Ĵ���
//�黮��:
//��0:0λ��ռ���ȼ�,4λ��Ӧ���ȼ�
//��1:1λ��ռ���ȼ�,3λ��Ӧ���ȼ�
//��2:2λ��ռ���ȼ�,2λ��Ӧ���ȼ�
//��3:3λ��ռ���ȼ�,1λ��Ӧ���ȼ�
//��4:4λ��ռ���ȼ�,0λ��Ӧ���ȼ�
//NVIC_SubPriority��NVIC_PreemptionPriority��ԭ����,��ֵԽС,Խ����	   
void MY_NVIC_Init(u8 NVIC_PreemptionPriority,u8 NVIC_SubPriority,u8 NVIC_Channel,u8 NVIC_Group)	 
{ 
	u32 temp;	  
	MY_NVIC_PriorityGroupConfig(NVIC_Group);//���÷���
	temp=NVIC_PreemptionPriority<<(4-NVIC_Group);	  
	temp|=NVIC_SubPriority&(0x0f>>NVIC_Group);
	temp&=0xf;								//ȡ����λ
	NVIC->ISER[NVIC_Channel/32]|=1<<NVIC_Channel%32;//ʹ���ж�λ(Ҫ����Ļ�,����ICER��ӦλΪ1����)
	NVIC->IP[NVIC_Channel]|=temp<<4;				//������Ӧ���ȼ����������ȼ�   	    	  				   
} 

#if EN_USART3_RX   		//���ʹ���˽���   	  
//���ջ����� 	
unsigned char RS485_RX_BUF[64];  	//���ջ���,���64���ֽ�.
//���յ������ݳ���
unsigned char RS485_RX_CNT=0;   
void USART3_IRQHandler(void)
{
	unsigned char res;	    
	if(USART3->SR&(1<<5))//���յ�����
	{	 
		res=USART3->DR; 			 
		if(RS485_RX_CNT<64)
		{
			RS485_RX_BUF[RS485_RX_CNT]=res;		//��¼���յ���ֵ
			RS485_RX_CNT++;						//������������1 
		} 
	}  											 
} 
#endif										 
//��ʼ��IO ����2
//pclk1:PCLK1ʱ��Ƶ��(Mhz),APB1һ��Ϊ42Mhz
//bound:������	  
void RS485_Init(unsigned int pclk1,unsigned int bound)
{  	 
	float temp;
	unsigned short  mantissa;
	unsigned short fraction;	   
	temp=(float)(pclk1*1000000)/(bound*16);//�õ�USARTDIV
	mantissa=temp;				 //�õ���������
	fraction=(temp-mantissa)*16; //�õ�С������	 
    mantissa<<=4;
	mantissa+=fraction; 
	
	//RCC->AHB1ENR|=1<<0;   		//ʹ��PORTA��ʱ��  
	//RCC->AHB1ENR|=1<<6;   		//ʹ��PORTG��ʱ��  
	//GPIO_Set(GPIOG,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PU); 		//PG8�������
	RCC->AHB1ENR|=1<<1;   		//ʹ��PORTB��ʱ��  
	GPIO_Set(GPIOB,GPIO_Pin_8,GPIO_Mode_OUT,GPIO_OType_PP,GPIO_Speed_100MHz,GPIO_PuPd_UP); 		//PG8�������
	GPIO_Set(GPIOB,GPIO_Pin_11|GPIO_Pin_10,GPIO_Mode_AF,GPIO_OType_PP,GPIO_Speed_50MHz,GPIO_PuPd_UP);	//PA2,PA3,���ù���,���� 
 	GPIO_AF_Set(GPIOB,11,7);		//PA2,AF7
	GPIO_AF_Set(GPIOB,10,7);		//PA3,AF7  	   
 
	RCC->APB1ENR|=1<<18;  		//ʹ�ܴ���3ʱ��  
	RCC->APB1RSTR|=1<<18;   	//��λ����3
	RCC->APB1RSTR&=~(1<<18);	//ֹͣ��λ	   	   
	//����������
 	USART3->BRR=mantissa; 		// ����������	 
	USART3->CR1|=0X200C;  		//1λֹͣ,��У��λ.
#if EN_USART2_RX		  		//���ʹ���˽���
	//ʹ�ܽ����ж� 
	USART3->CR1|=1<<2;  		//���ڽ���ʹ��
	USART3->CR1|=1<<5;    		//���ջ������ǿ��ж�ʹ��	    	
	MY_NVIC_Init(3,3,USART3_IRQn,2);//��2��������ȼ� 
#endif
	RS485_TX_EN;				//Ĭ��Ϊ����ģʽ	
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	RS485_TX_EN;			//����Ϊ����ģʽ
  	for(t=0;t<len;t++)		//ѭ����������
	{
		while((USART3->SR&0X40)==0);//�ȴ����ͽ���		  
		USART3->DR=buf[t];
	}	 
	while((USART3->SR&0X40)==0);//�ȴ����ͽ���	
	RS485_RX_CNT=0;	  
	RS485_TX_EN;				//����Ϊ����ģʽ	
}
//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen=RS485_RX_CNT;
	u8 i=0;
	*len=0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen==RS485_RX_CNT&&rxlen)//���յ�������,�ҽ��������
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=RS485_RX_CNT;	//��¼�������ݳ���
		RS485_RX_CNT=0;		//����
	}
}





