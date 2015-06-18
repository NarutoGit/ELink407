#include "stm32f4xx.h"
#include "stdio.h"

void init_uart(void) 
{ 
	GPIO_InitTypeDef GPIO_InitStructure; 
	USART_InitTypeDef USART_InitStructure; 
	 
	/* �� GPIO ʱ�� */ 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/* �� UART ʱ�� */ 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 
	
	/* �� PA9 ӳ��Ϊ USART1_TX */ 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1); 
	/* �� PA10 ӳ��Ϊ USART1_RX */ 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); 
	
	/* ���� USART Tx Ϊ���ù��� */ 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; /* �������Ϊ���� */ 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  /* �ڲ���������ʹ�� */ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  /* ����ģʽ */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	/* ���� USART Rx Ϊ���ù��� */ 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; 
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	/*  ���ô���Ӳ������ */ 
	USART_InitStructure.USART_BaudRate = 115200;  /* ������ */ 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No ; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	
		
	USART_Cmd(USART1, ENABLE);   /* ʹ�ܴ��� */ 

	USART_ClearFlag(USART1, USART_FLAG_TC); /* �巢����ɱ�־ */ 
} 


int fputc(int ch, FILE *f) 
{ 
	/* дһ���ֽڵ�USART1 */ 
	USART_SendData(USART1, (uint8_t) ch); 
	
	/* �ȴ����ͽ��� */ 
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) 
	{
	} 
	return ch; 
} 


int fgetc(FILE *f) 
{ 
	/* �ȴ�����1�������� */ 
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET); 
	return (int)USART_ReceiveData(USART1); 
}

//#define UART_DMA

#ifdef UART_DMA

#define TIMEOUT_MAX 1000 /* �������������ֵ */

void USART_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize) 
{ 
	DMA_InitTypeDef DMA_InitStructure; 
	__IO uint32_t Timeout = TIMEOUT_MAX; 
	/* ��λ DMA Stream �Ĵ��� (���ڵ���Ŀ��) */ 
	/* DMA_DeInit(USARTx_TX_DMA_STREAM); */ 
	/* 
	ʹ��ǰ���DMA�Ƿ񱻽�ֹ��   ��ע�⣬��ʹ����ͬ�������Ķ�δ���ʱ����һ�������ã�ʹ�ܣ�Ȼ���ֹ��Ȼ��ʹ  ��..... ����������£�ֻ�������ݴ��������ʱ��DMA�Ľ�ֹ����Ч����ȷ��ʹ�� 
	λ��Ӳ�����֮ǰ��ȷ���ǲ����ܵģ����DMA����ֻ��һ�Σ���һ�����Ժ��ԡ� 
	*/ 
	DMA_Cmd(DMA2_Stream7, DISABLE); 
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE) 
	{ 
	} 
	DMA_InitStructure.DMA_BufferSize = BufferSize; 
	/* ����DMA��С */ 
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; 
	/* �������������ʹ�ܻ��߽�ֹ�����Ե� */ 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; 
	/* ���÷�ֵ */ 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ; 
	/* �����ڴ�Ϊ���ֽ�ͻ��ģʽ */ 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	/* �����ڴ����ݵ�λ�����ֽ� */ 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	/* ʹ�ܵ�ַ���� */ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 

	/* ����DMA������ģʽ */ 
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART1->DR)) ; 
	/* ���������ַ */ 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	/* ��������Ϊ���ֽ�ͻ��ģʽ */ 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;; 
	/* �����������ݵ�λ�����ֽ� */ 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/* ��ֹ�����ַ���� */ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	/* �������ȼ� */ 
	/* ���� TX DMA */ 
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ; 
	/* ���÷���ͨ�� */ 
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral ; 
	/* ���ô��ڴ浽���� */ 
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)BufferSRC ; 
	/* �����ڴ��ַ */ 
	DMA_Init(DMA2_Stream7,&DMA_InitStructure); 
	/* ʹ�� DMA USART TX Stream */ 
	DMA_Cmd(DMA2_Stream7, ENABLE); 
	/* 
	���DMA Stream �Ƿ���ȷ��ʹ��.���DMA�Ĳ������ô����ˣ���ôDMA Stream   ��ʹ��λ�ᱻӲ����������Ӷ�ʹ�ô���ֹͣ����FIFO�ķ�ֵ���ô���   */ 
	Timeout = TIMEOUT_MAX; 
	while ((DMA_GetCmdStatus(DMA2_Stream7) != ENABLE) && (Timeout-- > 0)) 
	{ 
	} 
	/* ����Ƿ�ʱ */ 
	if (Timeout == 0) 
	{	 
		/* ������� */ 
		//TimeOut_UserCallback(); 
	} 
} 


#if 0 
void USART_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize) 
{ 
	DMA_InitTypeDef DMA_InitStructure; 
	__IO uint32_t Timeout = TIMEOUT_MAX; 
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2 ʱ��ʹ��
	
	/* 
	ʹ��ǰ���DMA�Ƿ񱻽�ֹ��   ��ע�⣬��ʹ����ͬ�������Ķ�δ���ʱ����һ�������ã�ʹ�ܣ�Ȼ���ֹ��Ȼ��ʹ  ��..... ����������£�ֻ�������ݴ��������ʱ��DMA�Ľ�ֹ����Ч����ȷ��ʹ 
	��λ��Ӳ�����֮ǰ��ȷ���ǲ����ܵģ����DMA����ֻ��һ�Σ���һ�����Ժ��ԡ� 
	*/ 
	DMA_Cmd(DMA2_Stream7, DISABLE); 
	while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE) 
	{	
		
	} 
	DMA_InitStructure.DMA_BufferSize = BufferSize; 
	/* ����DMA��С */ 
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; 

	/* �������������ʹ�ܻ��߽�ֹ�����Ե� */ 
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full; 
	/* ���÷�ֵ */ 
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single ; 
	/* �����ڴ�Ϊ���ֽ�ͻ��ģʽ */ 
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; 
	/* �����ڴ����ݵ�λ�����ֽ� */ 
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
	/* ʹ�ܵ�ַ���� */ 
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal; 
	/* ����DMA������ģʽ */ 
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t) (&(USART1->DR)) ; 
	/* ���������ַ */ 
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 
	/* ��������Ϊ���ֽ�ͻ��ģʽ */ 
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; 
	/* �����������ݵ�λ�����ֽ� */ 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
	/* ��ֹ�����ַ���� */ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; 
	/* �������ȼ� */ 
	/* ���� RX DMA */ 
	DMA_InitStructure.DMA_Channel = DMA_Channel_4 ; 
	/* ���ý���ͨ�� */ 
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory ; 
	/* ���ô����赽�ڴ� */ 
	DMA_InitStructure.DMA_Memory0BaseAddr =(uint32_t)BufferDST ; 
	/* �����ڴ��ַ */ 
	DMA_Init(DMA2_Stream7,&DMA_InitStructure); 
	/* ʹ�� DMA USART TX Stream */ 
	DMA_Cmd(DMA2_Stream7, ENABLE); 
	/* 
	���DMA Stream �Ƿ���ȷ��ʹ��.���DMA�Ĳ������ô����ˣ���ôDMA Stream   ��ʹ��λ�ᱻӲ����������Ӷ�ʹ�ô���ֹͣ����FIFO�ķ�ֵ���ô���   */ 
	Timeout = TIMEOUT_MAX; 
	while ((DMA_GetCmdStatus(DMA2_Stream7) != ENABLE) && (Timeout-- > 0)) 
	{ 
	} 
	/* ����Ƿ�ʱ */ 
	if (Timeout == 0) 
	{ 
	/* ������� */ 
	;//TimeOut_UserCallback(); 
	} 

	NVIC_InitTypeDef NVIC_InitStructure;

	/* ʹ�ܷ��ʹ�������ж� */ 
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE); 
	/* ʹ�ܽ��մ�������ж� */ 
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE); 
	/* ʹ�� DMA Stream �ж�ͨ�� */ 
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 

	/* ʹ�� DMA Stream �ж�ͨ�� */ 
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure); 
}
#endif 

//DMA ���жϷ������ 

void DMA2_Stream7_IRQHandler(void) 
{ 
 /* 
 ����DMA_GetITStatus��ʹ���ϴ������⣬������־�ˣ�����޷�ͨ��������ʹ�ú�  ��DMA_GetFlagStatusȴ���ԣ������¼һ��������⣬����������� 
 */ 
  if (DMA_GetFlagStatus(DMA2_Stream7,DMA_FLAG_TCIF7)==SET) 
//  if (DMA_GetITStatus(USARTx_TX_DMA_STREAM,USARTx_TX_DMA_FLAG_TCIF) == SET) 
  { 
   /* ���DMA������ɱ�־ */  //   
	  DMA_ClearITPendingBit(DMA2_Stream7, DMA_FLAG_TCIF7);   
  } 
 /* ������üĴ���ֱ�Ӳ��� */ 
// if(DMA2->HISR & USARTx_TX_DMA_FLAG_TCIF) 
//{ 
//  DMA2->HIFCR = USARTx_TX_DMA_FLAG_TCIF; 
//} 
} 

void USARTx_DMA_RX_IRQHandler(void) 
{ 
 /* ������üĴ���ֱ�Ӳ��� */ 
 if(DMA2->LISR & DMA_FLAG_TCIF7) 
{ 
  DMA2->LIFCR = DMA_FLAG_TCIF7; 
  /* ģ��һ���������� */ 
  //bsp_PutKey(KEY_4_DOWN); 
} 
} 


#endif

