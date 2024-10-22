/*
 * bsp_adc.c
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */

#include "bsp_adc.h"

uint16_t Instant_Data[MAX_MUM_AD];
uint16_t ADC_Filter_Data[MAX_MUM_AD];

bsp_adc_str g_tAdcConfig[MAX_MUM_AD] =
{
	{power_15,    	 GPIOA,	   GPIO_PIN_0,	 ADC_CHANNEL_0},
	{pres2,   		 GPIOA,    GPIO_PIN_2,   ADC_CHANNEL_2},
	{pres1,  	  	 GPIOA,    GPIO_PIN_1,   ADC_CHANNEL_1},
	{pres4,       	 GPIOA,    GPIO_PIN_5,   ADC_CHANNEL_5},
	{pres3,     	 GPIOA,    GPIO_PIN_4,   ADC_CHANNEL_4},
	{adc_ref,	 	 GPIOA,    GPIO_PIN_3,   ADC_CHANNEL_3},
	{temp6,          GPIOB,    GPIO_PIN_1,   ADC_CHANNEL_9},
	{temp2,          GPIOA,    GPIO_PIN_7,   ADC_CHANNEL_7},
	{temp3,     	 GPIOC,    GPIO_PIN_4,   ADC_CHANNEL_14},
	{temp4,      	 GPIOC,    GPIO_PIN_5,   ADC_CHANNEL_15},
	{temp5,          GPIOB,    GPIO_PIN_0,   ADC_CHANNEL_8},
	{temp1,       	 GPIOA,    GPIO_PIN_6,   ADC_CHANNEL_6},
	{power_12,       GPIOC,    GPIO_PIN_0,   ADC_CHANNEL_10},
	{power_24,       GPIOC,    GPIO_PIN_1,   ADC_CHANNEL_11},
	{VREFINT_AD,	 NULL,	   NULL,		 ADC_CHANNEL_17},
};

void bsp_adc_init(void)
{
	uint8_t i = 0;

	GPIO_Config_T gpioConfig;
	ADC_Config_T adcConfig;

	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA|RCM_APB2_PERIPH_GPIOB|RCM_APB2_PERIPH_GPIOC|RCM_APB2_PERIPH_ADC1);
	RCM_ConfigADCCLK(RCM_PCLK2_DIV_4);

	for(i = 0; i < MAX_MUM_AD;i++)
	{
		if(g_tAdcConfig[i].ADCPort != NULL)
		{
			gpioConfig.mode    = GPIO_MODE_ANALOG;
			gpioConfig.pin     = g_tAdcConfig[i].ADCPin;
			GPIO_Config(g_tAdcConfig[i].ADCPort, &gpioConfig);
		}
	}

	ADC_Reset(ADC1);
	ADC_ConfigStructInit(&adcConfig);
	adcConfig.mode = ADC_MODE_INDEPENDENT;
	adcConfig.scanConvMode = DISABLE;
	adcConfig.continuosConvMode = DISABLE;
	adcConfig.externalTrigConv = ADC_EXT_TRIG_CONV_None;
	adcConfig.dataAlign = ADC_DATA_ALIGN_RIGHT;
	adcConfig.nbrOfChannel = 1;
	ADC_Config(ADC1, &adcConfig);

	ADC_ConfigRegularChannel(ADC1, g_tAdcConfig[0].ADCChannel,1,ADC_SAMPLETIME_13CYCLES5);

	ADC_Enable(ADC1);
	ADC_EnableTempSensorVrefint(ADC1);
	ADC_ResetCalibration(ADC1);
	while(ADC_ReadResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_ReadCalibrationStartFlag(ADC1));
	ADC_EnableSoftwareStartConv(ADC1);
}


void bsp_adc_filter(uint8_t Channel,uint16_t data,uint16_t *buff)
{
	uint8_t i = 0;
	uint32_t sum = 0;
    static bsp_adc_filter_str g_tAdc_Filter = { 0 };

	if (Channel >= MAX_MUM_AD)
	{
		return;
	}	
	
	g_tAdc_Filter.Buffer[Channel][g_tAdc_Filter.Cnt[Channel]] = data;
	g_tAdc_Filter.Cnt[Channel]++;
	if(g_tAdc_Filter.Cnt[Channel] >= ADC_BUFFER_NUM)
	{
		g_tAdc_Filter.Cnt[Channel] = 0;
		g_tAdc_Filter.flag[Channel] = 1;
	}
	
	if(g_tAdc_Filter.flag[Channel] == 1)
	{
		sum = 0;
		for (i = 0; i < ADC_BUFFER_NUM; i++)
		{
			sum += g_tAdc_Filter.Buffer[Channel][i];
		}
		buff[Channel] = (uint16_t)(sum >> 6);		
	}
	else
	{
		sum = 0;
		for (i = 0; i < g_tAdc_Filter.Cnt[Channel]; i++)
		{
			sum += g_tAdc_Filter.Buffer[Channel][i];
		}
		buff[Channel] = (uint16_t)(sum / g_tAdc_Filter.Cnt[Channel]);
	}
}

uint16_t bsp_adc_volt_get(ADC_LIST_E index)
{
	uint16_t temp;
	if(index >= MAX_MUM_AD)
	{
		return 0;
	}
	temp = ADC_Filter_Data[index];
	if(temp > 4095)
	{
		temp = 4095;
	}

	return (temp*3300)/4095;
}

void ADC_Handle(void)
{
	uint16_t adcData;
	static uint8_t ADCUserChannelIndex = 0;

	if (ADC_ReadStatusFlag(ADC1,ADC_FLAG_EOC) == SET)
	{	
		ADC_ClearStatusFlag(ADC1,ADC_FLAG_EOC);
		
		adcData = ADC_ReadConversionValue(ADC1);
		Instant_Data[ADCUserChannelIndex] = adcData;
		bsp_adc_filter(ADCUserChannelIndex,adcData,ADC_Filter_Data);

		ADCUserChannelIndex++;
		ADCUserChannelIndex = ADCUserChannelIndex % MAX_MUM_AD;
				
		ADC_ConfigRegularChannel(ADC1,g_tAdcConfig[ADCUserChannelIndex].ADCChannel, 1, ADC_SAMPLETIME_13CYCLES5);
		ADC_EnableSoftwareStartConv(ADC1);
	}
}

uint16_t bsp_get_volt_24V(void)
{
	return bsp_adc_volt_get(power_24) * 11 + 500;
}

