#include "adc.h"
#include "delay.h"

__IO uint16_t ADC1ConvertedValue = 0;

/*�˲��㷨��ADC����10�Σ�ȥ����ߺ���ͣ���ȡʣ��8��ƽ��ֵ*/
uint32_t ADC_filter(void)
{
	uint32_t i,min,max,sum;
	uint32_t arr[10]={0};
	sum=0;
	for(i=0;i<10;i++){
		arr[i]=ADC1ConvertedValue *3300/0xFFF;
		delay_ms(100);
		sum += arr[i];
	}
	
	min=arr[0];
	max=arr[0];
	for(i=0;i<10;i++){
		if(arr[i]<min)
			min=arr[i];
		if(arr[i]>max)
			max=arr[i];
	}
	
	return (sum-min-max)>>3;
}

/*ADC1channel_8 with DMA configuration*/
void ADC1_CH8_DMA_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef       DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;

	/* Enable ADC1, DMA2 and GPIO clocks ****************************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2 | RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	/* DMA2 Stream0 channel0 configuration **************************************/
	/* ѡ��DMAƵ��*/
	DMA_InitStructure.DMA_Channel = DMA_Channel_0; 
	/*DMA�����ַ����ADC���ݼĴ�����ַ*/
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_ADDRESS;
	/*�Զ�����������׵�ַ*/
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&ADC1ConvertedValue;
	/*���ݴ��䷽�򣬴����赽�ڴ�*/
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	/*�������ݻ������Ĵ�С����λ���������С�����������ݴ�СΪhalfword����Ӧ����16λ*/
	DMA_InitStructure.DMA_BufferSize = 1;
	/*��ֹ����Ĵ����Զ�����*/
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	/*��ֹ�ڴ��ַ����*/
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
	/*�������ݵĴ�С*/
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	/*�ڴ����ݵĴ�С�������������ݻ�������С��*/
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	/*����ģʽ��ѭ��*/
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	/*���ȼ�high*/
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	/*��ֹFIFO���Ƚ��ȳ���*/
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	/*FIFO��ֵ*/
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	/*�����ڴ�����贫��ķ�ʽ����һ�Ĵ��䷽ʽ*/
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	/*��ʼ��DMA2_stream0*/
	DMA_Init(DMA2_Stream0, &DMA_InitStructure);
	/*ʹ��DMA2_stream*/
	DMA_Cmd(DMA2_Stream0, ENABLE);

	/* Configure ADC1 Channel8 pin��PB0�� as analog input ******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* ADC Common Init*/
	//����ģʽ
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//����ADCʱ��Ƶ��
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
	//��ֹ��ADCʹ��DMAģʽ
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	//��������֮����ӳ�
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);

	/* ADC1 Init ****************************************************************/
	//�ֱ���
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//��ֹɨ��ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//ʹ������ת������ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	//����ת��������ʽ
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//�������ݶ��䷽ʽ
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//ת��ͨ��
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channel_8 configuration *************************************/
	//����ͨ�����ã�ѡ��channel8���������˳��1-16��ת��ʱ��28������
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_28Cycles);

	/* Enable DMA request after last transfer (Single-ADC mode) */
	ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);

	/* Enable ADC1 DMA */
	ADC_DMACmd(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
}
