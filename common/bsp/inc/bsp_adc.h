/*
 * bsp_adc.h
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_ADC_H_
#define COMMON_BSP_INC_BSP_ADC_H_

#include <stdint.h>
#include "apm32f10x.h"
#include "apm32f10x_adc.h"
#include "apm32f10x_gpio.h"
#include "apm32f10x_rcm.h"

#define ADC_BUFFER_NUM      64

typedef enum
{
	power_15 = 0,
	pres2,
	pres1,
	pres4,
	pres3,
	adc_ref,
	temp6,
	temp2,
	temp3,
	temp4,
	temp5,
	temp1,
	power_12,
	power_24,
	VREFINT_AD,//内部基准源
	MAX_MUM_AD,
}ADC_LIST_E;

typedef struct 
{
	uint16_t Buffer[MAX_MUM_AD][ADC_BUFFER_NUM];
	uint8_t  Cnt[MAX_MUM_AD];
	uint8_t  flag[MAX_MUM_AD];	
}bsp_adc_filter_str;

typedef struct 
{
	uint8_t 		index;
	GPIO_T* 		ADCPort;
	uint16_t 		ADCPin;
	ADC_CHANNEL_T  	ADCChannel;
}bsp_adc_str;

void bsp_adc_init(void);
void ADC_Handle(void);
uint16_t bsp_adc_volt_get(ADC_LIST_E index);
uint16_t bsp_get_volt_24V(void);
#endif /* COMMON_BSP_INC_BSP_ADC_H_ */


