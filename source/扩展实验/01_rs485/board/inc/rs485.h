#ifndef __RS485_H
#define __RS485_H			 
 								  
	  	
extern unsigned char RS485_RX_BUF[64]; 		//���ջ���,���64���ֽ�
extern unsigned char RS485_RX_CNT;   			//���յ������ݳ���

//ģʽ����
#define RS485_TX_EN		GPIO_ResetBits(GPIOB,GPIO_Pin_8)	//485ģʽ����.0,����;1,����.
//����봮���жϽ��գ�����EN_USART2_RXΪ1����������Ϊ0
#define EN_USART3_RX 	1			//0,������;1,����.

														 
void RS485_Init(unsigned int pclk2,unsigned int  bound);
void RS485_Send_Data(unsigned char *buf,unsigned char len);
void RS485_Receive_Data(unsigned char *buf,unsigned char *len);		 
#endif	   
















