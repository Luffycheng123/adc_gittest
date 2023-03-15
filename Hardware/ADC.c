#include "stm32f10x.h"                  // Device header

#include "ADC.h"

void AD_Init(void)
{
		/*Initial GIPO 
		GPIO_Pin_A5
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //ADC clock signal define;
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_5;
	GPIO_Init(GPIOA,& GPIO_InitStruct);
	
	ADC_RegularChannelConfig(ADC1,ADC_Channel_5,1,ADC_SampleTime_55Cycles5);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode	= DISABLE;  
	ADC_InitStruct.ADC_DataAlign					= ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode								= ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel				= 1;
	ADC_InitStruct.ADC_ScanConvMode				= DISABLE;

	ADC_Init(ADC1,&ADC_InitStruct);
	
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1)==SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1)==SET);

}

uint16_t AD_GetValue(void)
{
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) == RESET );
	return ADC_GetConversionValue(ADC1);
	
	
}

