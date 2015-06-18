#include "stm32f4xx.h"
#include "sst25vf.h"
#include "stm32f4xx_spi.h"

void SST25V_Init(void)   //��ʼ��flashоƬ
{
	SST25V_CS_HIGH();                                 
	SST25V_WriteStatusRegister(0x02); 
	SST25V_DBSY();                  
}
u8 SST25V_ByteRead(u32 ReadAddr)     //��ReadAddr��ȡһ���ֽ�����
{
	u8 Temp = 0;
	SST25V_CS_LOW();                                  
	SPI_Flash_SendByte(Read_Data);                    
	SPI_Flash_SendByte((ReadAddr & 0xFF0000) >> 16);  //24λ��ַ
	SPI_Flash_SendByte((ReadAddr& 0xFF00) >> 8);
	SPI_Flash_SendByte(ReadAddr & 0xFF);
 
	Temp = SPI_Flash_ReceiveByte();                   
	SST25V_CS_HIGH();                      
	return Temp;                       
}

void SST25V_BufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)//��ReadAddr��ȡNumByteToRead�ֽ�����
{
	SST25V_CS_LOW();                                  
	SPI_Flash_SendByte(Read_Data);                 
	SPI_Flash_SendByte((ReadAddr & 0xFF0000) >> 16);  
	SPI_Flash_SendByte((ReadAddr& 0xFF00) >> 8);
	SPI_Flash_SendByte(ReadAddr & 0xFF);
	while(NumByteToRead--)             
	{
		*pBuffer = SPI_Flash_ReceiveByte(); 
		pBuffer++;                      
	}
	SST25V_CS_HIGH();     
}

u8 SST25V_HighSpeedByteRead(u32 ReadAddr)  //���ٴ�ReadAddr��ȡһ���ֽ�����
{
	u32 Temp = 0;
	SST25V_CS_LOW();                                   
	SPI_Flash_SendByte(HighSpeedReadData);             
	SPI_Flash_SendByte((ReadAddr & 0xFF0000) >> 16);   
	SPI_Flash_SendByte((ReadAddr& 0xFF00) >> 8);
	SPI_Flash_SendByte(ReadAddr & 0xFF);
	SPI_Flash_SendByte(Dummy_Byte);          
	Temp = SPI_Flash_ReceiveByte(); 
	SST25V_CS_HIGH(); 
	return Temp; 
}
/*��ReadAddr��ַ��ʼ���ٶ�ȡNumByteToRead�ֽ�����,������pBuffer*/
void SST25V_HighSpeedBufferRead(u8* pBuffer, u32 ReadAddr, u16 NumByteToRead)
{
	SST25V_CS_LOW();          
	SPI_Flash_SendByte(HighSpeedReadData);  
	SPI_Flash_SendByte((ReadAddr & 0xFF0000) >> 16);
	SPI_Flash_SendByte((ReadAddr& 0xFF00) >> 8);
	SPI_Flash_SendByte(ReadAddr & 0xFF);
	SPI_Flash_SendByte(Dummy_Byte);  
	while(NumByteToRead--)   
	{
		*pBuffer = SPI_Flash_ReceiveByte(); 
		pBuffer++;    
	}
	SST25V_CS_HIGH();  
}

u8 SPI_Flash_SendByte(u8 byte)   //SPI��������
{
	while(SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);     //�ȴ�SPI1��Tx bufferΪ��
	SPI_SendData(SPI1, byte);                                  //���������ֽ�byte
	while(SPI_GetFlagStatus(SPI1, SPI_FLAG_RXNE) == RESET);    //�ȴ�SPI1��ɽ�������
	return SPI_ReceiveData(SPI1);                              //���ض���������
}

u8 SPI_Flash_ReceiveByte(void)    //SPI��������
{
	return (SPI_Flash_SendByte(Dummy_Byte));    //�������ӽڣ��ṩ��������ʱ��
}

void SST25V_ByteWrite(u8 Byte, u32 WriteAddr)   //д��һ���ֽ�����
{
	SST25V_WriteEnable();                                
	SST25V_CS_LOW();                                    
	SPI_Flash_SendByte(Byte_Program);                   
	SPI_Flash_SendByte((WriteAddr & 0xFF0000) >> 16);   
	SPI_Flash_SendByte((WriteAddr & 0xFF00) >> 8);
	SPI_Flash_SendByte(WriteAddr & 0xFF);
 
	SPI_Flash_SendByte(Byte);
	SST25V_CS_HIGH();
	SST25V_WaitForWriteEnd(); 
}

/*��pBuffer�е�BufferLength������д��Addr��ʼ�ĵ�ַ*/
void SST25V_BufferWrite(u8 *pBuffer,u32 Addr,u16 BufferLength)
{
	while(BufferLength--)                   
	{
		SST25V_ByteWrite(*pBuffer,Addr); 
		pBuffer++;        
		Addr++;   
	}
}

/*��WriteAddrΪ��ʼ��ַ,д��ByteLength��Byte����(��ͬ��Byte)*/
void SST25V_WriteBytes(u8 Byte, u32 WriteAddr,u32 ByteLength)  
{
    while(ByteLength--)
	{
		SST25V_ByteWrite(Byte,WriteAddr);
		WriteAddr++;  
	}
}

/*��AddrΪ��ʼ��ַ,��AAIģʽ,д��ByteLength����ͬ����Byte��ByteLength����Ϊż��*/
void SST25V_AAI_WriteBytes(u8 Byte, u32 Addr,u32 ByteLength)
{
	u32 Length = 0;
	Length = (ByteLength/2)-1;
	SST25V_WriteEnable();                            
	SST25V_CS_LOW();   
	SPI_Flash_SendByte(AAI_WordProgram);  
	SPI_Flash_SendByte((Addr & 0xFF0000) >> 16); 
	SPI_Flash_SendByte((Addr & 0xFF00) >> 8);
	SPI_Flash_SendByte(Addr & 0xFF);
	SPI_Flash_SendByte(Byte);        
	SPI_Flash_SendByte(Byte);        
	SST25V_CS_HIGH();                            
	SST25V_WaitForWriteEnd();                  
 
	while(Length--)                             
	{
		SST25V_CS_LOW();                         
		SPI_Flash_SendByte(AAI_WordProgram);          
		SPI_Flash_SendByte(Byte);                    
		SPI_Flash_SendByte(Byte);      
		SST25V_CS_HIGH();        
		SST25V_WaitForWriteEnd();      
	}
	SST25V_WriteDisable();  
}

/*��AAIģʽ,��pBuffer�е�ByteLength����д����AddrΪ��ʼ��ַ,��ByteLength����Ϊż��*/
void SST25V_AAI_BufferProgram(u8 *pBuffer,u32 Addr,u16 BufferLength)
{
	u16 Length = 0;
	Length = (BufferLength/2)-1;  
 
	SST25V_WriteEnable(); 
	SST25V_CS_LOW();   
	SPI_Flash_SendByte(AAI_WordProgram); 
	SPI_Flash_SendByte((Addr & 0xFF0000) >> 16); 
	SPI_Flash_SendByte((Addr & 0xFF00) >> 8);
	SPI_Flash_SendByte(Addr & 0xFF);
 
	SPI_Flash_SendByte(*pBuffer);  
	pBuffer++;        
	SPI_Flash_SendByte(*pBuffer); 
	pBuffer++;           
 
	SST25V_CS_HIGH();            
	SST25V_WaitForWriteEnd();   
 
	while(Length--)
	{
		SST25V_CS_LOW();   
		SPI_Flash_SendByte(AAI_WordProgram); 
		SPI_Flash_SendByte(*pBuffer);  
		pBuffer++;       
		SPI_Flash_SendByte(*pBuffer);            
		pBuffer++;          
		SST25V_CS_HIGH();      
		SST25V_WaitForWriteEnd(); 
	}
	SST25V_WriteDisable(); 
}

/*��AAIģʽ��,��AddrΪ�׵�ַ,�ֱ�д��Byte1��Byte2,�����SST25V_AAI_WordsProgram()����ʹ��*/
void SST25V_AAI_WordProgram(u8 Byte1, u8 Byte2, u32 Addr)
{
	SST25V_WriteEnable();                 
	SST25V_CS_LOW();                           
	SPI_Flash_SendByte(AAI_WordProgram);         
	SPI_Flash_SendByte((Addr & 0xFF0000) >> 16);   
	SPI_Flash_SendByte((Addr & 0xFF00) >> 8);
	SPI_Flash_SendByte(Addr & 0xFF);

	SPI_Flash_SendByte(Byte1);                 
	SPI_Flash_SendByte(Byte2);      
 
	SST25V_CS_HIGH();          
	SST25V_WaitForWriteEnd();         
}

/*��AAIģʽ��,����д��Byte1��Byte2,�����SST25V_AAI_WordProgram()����ʹ��state==1�˳�AAIģʽ*/
void SST25V_AAI_WordsProgram(u8 state,u8 Byte1, u8 Byte2) 
{
	SST25V_CS_LOW();                   
	SPI_Flash_SendByte(AAI_WordProgram); 

	SPI_Flash_SendByte(Byte1);    
	SPI_Flash_SendByte(Byte2);   

	SST25V_CS_HIGH();         
	SST25V_WaitForWriteEnd();   
 
	if(state==1)      
	{
		SST25V_WriteDisable();
	}
}

void SST25V_SectorErase_4KByte(u32 Addr)    //����4KB���ݿռ�,Addr�ǲ�����ʼ��ַ
{
	SST25V_WriteEnable();              
	SST25V_CS_LOW();                          
	SPI_Flash_SendByte(SectorErace_4KB);      
	SPI_Flash_SendByte((Addr & 0xFF0000) >> 16); 
	SPI_Flash_SendByte((Addr & 0xFF00) >> 8);
	SPI_Flash_SendByte(Addr & 0xFF);
 
	SST25V_CS_HIGH();              
	SST25V_WaitForWriteEnd();     
}


void SST25V_BlockErase_32KByte(u32 Addr)   //����32KB���ݿռ�,Addr�ǲ�����ʼ��ַ
{
	SST25V_WriteEnable();                         
	SST25V_CS_LOW();                             
	SPI_Flash_SendByte(BlockErace_32KB);       
	SPI_Flash_SendByte((Addr & 0xFF0000) >> 16);  
	SPI_Flash_SendByte((Addr & 0xFF00) >> 8);
	SPI_Flash_SendByte(Addr & 0xFF);
 
	SST25V_CS_HIGH();                        
	SST25V_WaitForWriteEnd();               
}

void SST25V_BlockErase_64KByte(u32 Addr)    //����64KB���ݿռ�,Addr�ǲ�����ʼ��ַ
{
	SST25V_WriteEnable();               
	SST25V_CS_LOW();                    
	SPI_Flash_SendByte(BlockErace_64KB);     
	SPI_Flash_SendByte((Addr & 0xFF0000) >> 16); 
	SPI_Flash_SendByte((Addr & 0xFF00) >> 8);
	SPI_Flash_SendByte(Addr & 0xFF);
 
	SST25V_CS_HIGH();          
	SST25V_WaitForWriteEnd();   
}

void SST25V_ChipErase(void)            //��������оƬ���ݿռ�
{
	SST25V_WriteEnable();             
	SST25V_CS_LOW();       
	SPI_Flash_SendByte(ChipErace);  
	SST25V_CS_HIGH();    
	SST25V_WaitForWriteEnd();  
}

u8 SST25V_ReadStatusRegister(void)    //��ȡ״̬�Ĵ���
{
	u8 StatusRegister = 0;
	SST25V_CS_LOW();              
	SPI_Flash_SendByte(ReadStatusRegister);   
	StatusRegister = SPI_Flash_ReceiveByte();  
	SST25V_CS_HIGH();     
	return StatusRegister;  
}

void SST25V_WriteEnable(void)          //дʹ��
{
	SST25V_CS_LOW();                    
	SPI_Flash_SendByte(WriteEnable);
	SST25V_CS_HIGH(); 
}

void SST25V_WriteDisable(void)         //д��ֹ
{
	SST25V_CS_LOW();                   
	SPI_Flash_SendByte(WriteDisable);   
	SST25V_CS_HIGH();                
}

void SST25V_EnableWriteStatusRegister(void)        //�����д״̬�Ĵ���
{
	SST25V_CS_LOW();                       
	SPI_Flash_SendByte(EnableWriteStatusRegister);  
	SST25V_CS_HIGH();   
}

void SST25V_WriteStatusRegister(u8 Byte)      //��д״̬�Ĵ�������״̬�Ĵ���д��byte
{
	SST25V_EnableWriteStatusRegister();     
	SST25V_CS_LOW();                        
	SPI_Flash_SendByte(WriteStatusRegister);   
	SPI_Flash_SendByte(Byte); 
	SST25V_CS_HIGH();   
}

void SST25V_WaitForWriteEnd(void)     //�ȴ�д�������
{
	u8 FLASH_Status = 0;
	SST25V_CS_LOW();                            
	do
	{
		FLASH_Status = SST25V_ReadStatusRegister(); 

	} while((FLASH_Status & 0x01) == SET);  
	SST25V_CS_HIGH();     
}

u32 SST25V_ReadJedecID(void)       //��ȡJEDEC ID
{
	u32 JEDECID = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	SST25V_CS_LOW();                                 
	SPI_Flash_SendByte(ReadJedec_ID);            
	Temp0 = SPI_Flash_ReceiveByte();     
	Temp1 = SPI_Flash_ReceiveByte();    
	Temp2 = SPI_Flash_ReceiveByte();    
	SST25V_CS_HIGH();  
	JEDECID = (Temp0 << 16) | (Temp1 << 8) | Temp2; 
	return JEDECID;     
}

 //��ȡManufacturerID��DeviceID,ReadManu_DeviceID_Addr��״̬������һ���ֽ���DeviceID����ManufacturerID
u16 SST25V_ReadManuID_DeviceID(u32 ReadManu_DeviceID_Addr)
{
	u16 ManuID_DeviceID = 0;   
	u8 ManufacturerID = 0,  DeviceID = 0;
	SST25V_CS_LOW();  
	SPI_Flash_SendByte(ReadDeviceID); 
 
	SPI_Flash_SendByte((ReadManu_DeviceID_Addr & 0xFF0000) >> 16); 
	SPI_Flash_SendByte((ReadManu_DeviceID_Addr & 0xFF00) >> 8);
	SPI_Flash_SendByte(ReadManu_DeviceID_Addr & 0xFF);
 
	if(ReadManu_DeviceID_Addr==1) 
	{
		DeviceID = SPI_Flash_ReceiveByte();  
		ManufacturerID = SPI_Flash_ReceiveByte();
	}
	else
	{
		ManufacturerID = SPI_Flash_ReceiveByte(); 
		DeviceID = SPI_Flash_ReceiveByte();  
	}
	ManuID_DeviceID = ((ManufacturerID<<8) | DeviceID); 
	SST25V_CS_HIGH();      
 
	return ManuID_DeviceID;    
}

void SST25V_EBSY()    //����AAIģʽ�ڼ�,so�������æ״̬
{
	SST25V_CS_LOW();           
	SPI_Flash_SendByte(EBSY); 
	SST25V_CS_HIGH();  
}

void SST25V_DBSY()            //��ֹAAIģʽ�ڼ�,SO�������æ״̬
{
	SST25V_CS_LOW();     
	SPI_Flash_SendByte(DBSY); 
	SST25V_CS_HIGH();  
}





void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB, ENABLE);
}

void GPIO_Configuration(void)
{	
	//PA5--SPI1_SCK    PB4--SPI1--MISO    PB5--SPI1--MOSI  PA15--SPI1CS
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 |GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	GPIO_Init(GPIOB, &GPIO_InitStructure);
			
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
 
	//PA15--SPI1_CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	

}

void SPI_configuration()  //SPI1 configuration
{
	SPI_InitTypeDef  SPI_InitStructure;
 
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;     
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;  
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;  
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;  
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1, ENABLE);  //Enable SPI1
}

void NVIC_Configuration(void)
{
#ifdef  VECT_TAB_RAM 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
#endif
}

void init_spi(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	SPI_configuration();
	NVIC_Configuration();
}


