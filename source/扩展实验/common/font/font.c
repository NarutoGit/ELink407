
#include <stdio.h>
#include <string.h>
#include "font.h"
#include "delay.h"

//font ��ʱδʹ��
//str �ַ���
//buf ��������
//off ���ַ���ȡ�ַ�����

extern unsigned char FontHZ16_Start[];
extern unsigned char FontHZ16_End[];

extern unsigned char FontAsc16_Start[];
extern unsigned char FontAsc16_End[];

unsigned char * FONT_GetDataGB2312(FontData *data, const unsigned char *str)
{
	unsigned int offset;
	if (str && data)
	{
		if (*str >= (unsigned char)0x80)
		{
			//hz
			unsigned int qu, wei;
			qu = *str - 0xa1;
			wei = *(str+1) - 0xa1;
			offset = (qu * 94 + wei) * 32;

			if (FontHZ16_Start + offset < FontHZ16_End)
			{
				data->str_bytes = 2;
				data->w = 16;
				data->h = 16;
				data->data = &(FontHZ16_Start[offset]);
				return &(FontHZ16_Start[offset]);
			}
		}
		else
		{
			//ascii
			data->str_bytes = 1;
			data->w = 8;
			data->h = 16;
			data->data = &(FontAsc16_Start[*str * 16]);
			return &(FontAsc16_Start[*str * 16]);
		}
	}
	return 0;
}

#if 0


USDOS 16*16 �����ֿ⣬��ַ�� [(����-1)��94 �� λ�� �� 1]��32
���� �� �����һ�ֽ� �� A0H
λ�� �� ����ڶ��ֽ� �� A0H

����: hz һ������
�����data ��������

bool GetHZ(unsigned char hz[2], unsigned char data[32])
{
    int Offset, qu, wei;
    FILE *fp;

    if ((fp = fopen("c:\\ucdos\\hzk16", "rb"))==NULL)
    {
        ::MessageBox(NULL, "Open hzk16 error", "error", 0);
        return false;
    }

    if (strlen(hz) < 2)
    {
        return false;
    }

    qu = hz[0] - 0xa0;
    wei = hz[1] - 0xa0;
    if (qu < 0 || wei < 0)
    {
        return false;
    }
    Offset = ((qu - 1) * 94 + wei - 1) * 32;
    fseek(fp, Offset, SEEK_SET);
    fread(data, 1, 32, fp);
    fclose(fp);
    return true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
    int n = 0;
    unsigned char  hz[2], data[32];
    char s[256];

    if (Edit1->Text.Length() < 2)
    {
        return;
    }
    memcpy(hz, Edit1->Text.c_str(),2);

    //ȡ��������
    if (GetHZ(hz, data))
    {
        //��16������ʾ����
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                sprintf(s + j * 5, "%02XH, ", data[i * 8 + j]);
            }
            Memo1->Lines->Add(s);
        }
    }

}
#endif

#if 0

void LCD_SetFont(sFONT *fonts,uint8_t Mode)
{
	LCD_Currentfonts = fonts;
	LCD_Currentfonts->Mode=Mode;
}

/************************************************************************************************
 * ����   : ��ȡ��ǰ����.
 * ����   : ��
 * ����   : ��ǰ����ʹ�õ�����.
 * ˵��   :
 *************************************************************************************************/
sFONT *LCD_GetFont(void)
{
	return LCD_Currentfonts;
}

/************************************************************************************************
 * ����   : ָ��λ��дһ���ַ�
 * ����   : X,Y�� X����,Y����.
 *          char�� ָ���ַ�ascii�����ָ��
 * ����   : ��
 * ˵��   :
 *************************************************************************************************/
void LCD_ShowChar(uint16_t x,uint8_t y,uint8_t Char)
{   
	uint8_t temp,dy,dx;//λ��ƫ��

	if(Char>31)		   
		Char=Char-32;//�õ�ƫ�ƺ��ֵ
	else  //�����ַ����ԣ���Ϊ�ո�
		Char=0;

	for(dy=0;dy<LCD_Currentfonts->Height;dy++)
	{
		temp=LCD_Currentfonts->table[Char][dy];//��������
		for(dx=0;dx<LCD_Currentfonts->Height/2;dx++)
		{                 
			if(temp&0x01)
			{ 
				LCD_SetCursor(x+dx,y+dy);//���ù��λ�� 
				LCD_WriteRAM_Prepare(); 
				LCD_WriteRAM(TextColor);
			}
			else
				if(LCD_Currentfonts->Mode)
				{
					LCD_SetCursor(x+dx,y+dy);//���ù��λ�� 
					LCD_WriteRAM_Prepare(); 
					LCD_WriteRAM(BackColor);
				}     
			temp>>=1; 
		}
	}

}
/************************************************************************************************
 * ����   : ��LCD����ʾһ���ַ�(�ַ�������һά����),����ֻ������ʾ����
 * ����   : _Xpos,_Ypos��X����,Y����.
 *          width������Ŀ�ȣ��ַ���8��������16
 *          ptr:   ָ���ַ������ָ��
 * ����   : ��
 * ˵��   :
 *************************************************************************************************/
#if _USE_HZK16
static void TFT_DispChar(uint16_t _Xpos, uint16_t _Ypos, const uint8_t *c, uint8_t _width)
{
	uint32_t height = 0, i = 0;
	uint16_t x=0;
	uint8_t Yaddress;
	Yaddress = _Ypos;

	LCD_SetCursor(_Xpos, _Ypos);
	if (LCD_Currentfonts->Mode)//�ж��Ƿ����
	{
		for (height = 0; height < 16; height++)  //�ַ��߶�
		{
			uint16_t x = _Xpos;
			for (i = 0; i < _width; i++)	  //�ַ����
			{
				LCD_SetCursor(x, Yaddress);
				LCD_WriteRAM_Prepare();

				if ((c[height * (_width / 8) + i / 8] & (0x80 >> (i % 8))) == 0x00)
				{
					LCD_WriteRAM(BackColor);
				}
				else
				{
					LCD_WriteRAM(TextColor);
				}
				x++;
			}
			Yaddress++;			
		}
	}
	else
	{
		for (height = 0; height < 16; height++)   //�ַ��߶�
		{
			x= _Xpos;

			for (i = 0; i < _width; i++)	   //�ַ����
			{
				if ((c[height * (_width / 8) + i / 8] & (0x80 >> (i % 8))) != 0x00)
				{
					LCD_SetCursor(x, Yaddress);
					LCD_WriteRAM_Prepare();
					LCD->LCD_RAM = TextColor;
				}
				x++;
			}
			Yaddress++;
		}
	}
}

#endif  

/************************************************************************************************
 * ����   : ��ʾһ���ַ�.
 * ����   : Line,Column��������λ��.
 *          *ptr: ָ��Ҫ��ʾ���ַ�����ָ��.
 * ����   : ��
 * ˵��   :
 *************************************************************************************************/
uint16_t LCD_DisplayStringLine(uint8_t Line,uint16_t Column, uint8_t *ptr)
{
#if _USE_HZK16
	uint32_t address;
	uint16_t HZK16[16];
#endif
	uint16_t refcolumn =Column;
	uint8_t refLine =Line;
	uint8_t *P=ptr;
#if _USE_HZK16
	uint8_t code1,code2;
#endif

	while(*P++!='\0');P--;//�õ��ַ����Ľ���ָ��

	if(refLine>=LCD_Pixel_Height)
	{  
		return (P-ptr);//����δ����ʾ���ַ���
	}
	while (*ptr!='\0')
	{
#if _USE_HZK16	
		code1=*ptr;			     
		if(code1<0x80)	//�ж��Ǻ��ִ��룬����ascii���� 
		{
#endif
			if(refcolumn>=LCD_Pixel_Width)
			{	
				refcolumn=0;
				refLine+=LCD_Currentfonts->Height;//����
				if(refLine>=LCD_Pixel_Height)
				{  
					return (P-ptr);//����δ����ʾ���ַ���
				} 
			}
			if((*ptr!=13)&&(*ptr!=10))
			{  	
				LCD_ShowChar(refcolumn, refLine, *ptr);
				refcolumn += LCD_Currentfonts->Width;
			}
			else if(*ptr==13)//���з���
			{
				refcolumn=0;
				refLine+=LCD_Currentfonts->Height;//����
				if(refLine>=LCD_Pixel_Height)
				{  
					return (P-ptr);//����δ����ʾ���ַ���
				} 
			}
			else if(*ptr==10) //������ʾ�еĿ�����
			{
				LCD_ShowChar(refcolumn, refLine, 32);
				refcolumn += LCD_Currentfonts->Width;
				LCD_ShowChar(refcolumn, refLine, 32);
				refcolumn += LCD_Currentfonts->Width;
			}
#if  _USE_HZK16
		}
		//��������
		else
		{
			code2 = *++ptr;
			if(code2=='\0')
				return 1;//�а������û����ʾ
			/*---------�õ��ú�����ģ����------------------------------*/
			//����16�����ֵ����ַaddreSS=[(code1-0xa1)*94 + (code2-0xa1)]*32	
			address =((code1-0xa1) * 94 + (code2-0xa1))<<5;
			if(address<0x41600)//��ֹ��ַԽ��
			{  	       
				SRAM_ReadBuffer(HZK16, address, 16);
			}
			else//Խ����0
			{ 		
				for(code1=0;code1<16;code1++)
					HZK16[code1]=0x00;
			}
			/*--------------------------------------------------------*/
			if(refcolumn>=LCD_Pixel_Width)
			{	
				refcolumn=0;
				refLine+=LCD_Currentfonts->Height;//����
				if(refLine>=LCD_Pixel_Height)
				{  
					return (P-ptr+1);//����δ����ʾ���ַ���
				} 
			}
			TFT_DispChar(refcolumn, refLine, (const uint8_t *)HZK16, 16);
			refcolumn +=16;
		} 
#endif
		ptr++;
	}
	return 0;
}

/************************************************************************************************
 * ����   : ��ʾ{������ֵ(0~4294967295)}.
 * ����   : Line,Column��������λ��.
 *          num:��ֵ(0~4294967295).
 * ����   : ��
 * ˵��   :
 *************************************************************************************************/	 
void LCD_DisplayNum(uint8_t Line,uint16_t Column, uint32_t num)
{ 
	uint32_t temp=num;       	
	uint16_t refcolumn =Column;
	uint8_t  refLine =Line,len=0; 
	//������ֵ����
	do
	{
		temp = temp/10;
		len++;
	}
	while(temp); 
	len--;
	/********************************/
	refcolumn +=len*LCD_Currentfonts->Width;//�Ӻ���ǰ��ʾ
	//��ʾ����
	do
	{ 
		temp=num%10;
		LCD_ShowChar(refcolumn,refLine,temp+'0'); 
		refcolumn-=LCD_Currentfonts->Width;
		num=num/10;
	}
	while(len--);					   
}

/************************************************************************************************
 * ����   : ��ʾ������.
 * ����   : Line,Column��������λ��.
 *          num:��ֵ.
 * ����   : ��
 * ˵��   :��������С��2147483647����-2147483648��
 *         ���������ִ��һ��ֵ��С�����ֲ�׼ȷ�ˣ�С��λ����5λ
 *************************************************************************************************/
#if _USE_Float

void LCD_Display_FloatNum(uint8_t Line,uint16_t Column, double num)
{
	uint64_t Temp;
	uint32_t Temp1;
	uint8_t len=0;
	if(num<0)//����
	{
		num=-num;
		LCD_DisplayStringLine(Line,Column,"-");
		len++;
	}
	Temp=(uint64_t)(num*100000);//��Ϊ����
	//����������ʾ
	Temp1=(uint32_t)(Temp/100000);
	//��ʾ��������
	LCD_DisplayNum(Line,Column+Column(len),Temp1);
	//�����������ֳ���
	do
	{
		Temp1 = Temp1/10;
		len++;
	}
	while(Temp1);
	//С������
	Temp1=(uint32_t)(Temp%100000);
	//ȥ��ĩβ����0
	while(Temp1%10==0)
	{
		Temp1=Temp1/10;
	}
	//��ʾС����
	if(Temp1!=0)//�ж��Ƿ���С����������ʾ
	{
		LCD_DisplayStringLine(Line,Column+Column(len),".");
		len++;
		LCD_DisplayNum(Line,Column+Column(len),Temp1);
	}
}

#endif

#if _USE_GRAPH

/************************************************************************************************
 * ����   : ��LCD����ʾһ��bin��ʽ��ͼƬ.
 * ����   : ptr �� ͼƬ����ָ��.
 *          ptr �� ͼƬ���ݴ�С.
 * ����   : ��
 * ˵��   :
 *************************************************************************************************/
void LCD_WeBMP_SIZE(uint16_t *ptr,uint32_t SIZE)
{
	uint32_t index;

	for (index = 0; index < SIZE; index++)
	{
		LCD_WriteRAM(*ptr++)
	}
}

#endif

#endif

