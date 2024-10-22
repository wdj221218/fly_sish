/*
 * bsp_gpio.c
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */
#include "bsp_gpio.h"

const bsp_gpio_str g_tGpioInputConfig[MAX_MUM_INPUT] =
{
	ptc_err_chk,		GPIOD,		GPIO_PIN_10,	1,		0,
	pump_err_chk,		GPIOD,		GPIO_PIN_11,	1,		0,
	force_chk,		    GPIOD,		GPIO_PIN_14,	0,		0,
	tank_chk,		    GPIOD,		GPIO_PIN_15,	0,		0,
	jcq_chk,			GPIOD,		GPIO_PIN_3,		1,		0,
	fan1_fb,		    GPIOE,		GPIO_PIN_2,		0,		0,
	fan2_fb,		    GPIOE,		GPIO_PIN_6,		0,		0,
	fan3_fb,		    GPIOE,		GPIO_PIN_14,	0,		0,
	valve_on_check,		GPIOC,		GPIO_PIN_13,	0,		0,
	valve_open_check,	GPIOC,		GPIO_PIN_14,	0,		0,
	valve_close_check,	GPIOC,		GPIO_PIN_15,	0,		0,
};

const bsp_gpio_str g_tGpioOutputConfig[MAX_MUM_OUTPUT] =
{
	ptc1_ctl,			GPIOB,		GPIO_PIN_9,		1,		0,//PTC控制引脚
	ptc2_ctl,			GPIOB,		GPIO_PIN_8,		1,		0,//加热带控制引脚
	fan_pwr_ctl,		GPIOE,		GPIO_PIN_3,	    1,		0,//风机供电控制
	pump_pwr_ctl,		GPIOB,		GPIO_PIN_14,	1,		0,//水泵供电控制
	pres_pwr_ctl,		GPIOE,		GPIO_PIN_15,	1,		1,//压力供电控制
	temp_pwr_ctl,		GPIOB,		GPIO_PIN_12,	1,		1,//温度供电控制
	dcf_ctl,			GPIOA,		GPIO_PIN_11,	1,		0,//三通阀控制
	jcq_ctl,			GPIOD,		GPIO_PIN_2,		1,		0,//接触器控制
	kg_ctl,				GPIOD,		GPIO_PIN_4,		1,		0,//常开常闭控制
	ee_wc,				GPIOE,		GPIO_PIN_7,		1,		0,//EE写控制引脚
	pcs_pump_pwr_ctl,	GPIOA,		GPIO_PIN_8,		1,		0,//PCS水泵控制
	led1,				GPIOE,		GPIO_PIN_0,		1,		0,//led1
	led2,				GPIOE,		GPIO_PIN_1,	    1,		0,//led2
	led3,				GPIOE,		GPIO_PIN_4,		1,		0,//led3
};

void bsp_gpio_init(void)
{	
	uint8_t i = 0; 
	GPIO_Config_T gpioConfig;
	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB |
							   RCM_APB2_PERIPH_GPIOC | RCM_APB2_PERIPH_GPIOD | RCM_APB2_PERIPH_GPIOE);
	//GPIO_ConfigPinRemap(GPIO_REMAP_SWJ_JTAGDISABLE);
	for(i = 0; i < MAX_MUM_INPUT; i++)
	{
		gpioConfig.mode = GPIO_MODE_IN_PU;
		gpioConfig.pin = g_tGpioInputConfig[i].Pin;
		GPIO_Config(g_tGpioInputConfig[i].Port,&gpioConfig);
	}

	for(i = 0; i  < MAX_MUM_OUTPUT;i++)
	{
		gpioConfig.mode = GPIO_MODE_OUT_PP;
		gpioConfig.pin = g_tGpioOutputConfig[i].Pin;
		gpioConfig.speed = GPIO_SPEED_50MHz;
		GPIO_Config(g_tGpioOutputConfig[i].Port,&gpioConfig);
		if(g_tGpioOutputConfig[i].DefaultLevel)
		{
			GPIO_SetBit(g_tGpioOutputConfig[i].Port,g_tGpioOutputConfig[i].Pin);
		}
		else
		{
			GPIO_ResetBit(g_tGpioOutputConfig[i].Port,g_tGpioOutputConfig[i].Pin);
		}
	}
}

void bsp_set_gpio(GPIO_OUTPUT_LIST Index,uint8_t level)
{
	if(Index > MAX_MUM_OUTPUT)
	{
		return;
	}

	if(level)
	{
		GPIO_SetBit(g_tGpioOutputConfig[Index].Port,g_tGpioOutputConfig[Index].Pin);
	}
	else
	{
		GPIO_ResetBit(g_tGpioOutputConfig[Index].Port,g_tGpioOutputConfig[Index].Pin);
	}


}

void bsp_set_gpio_act(GPIO_OUTPUT_LIST Index)
{
	if(Index > MAX_MUM_OUTPUT)
	{
		return;
	}

	bsp_set_gpio(Index,g_tGpioOutputConfig[Index].ActLevel);
}

void bsp_set_gpio_unact(GPIO_OUTPUT_LIST Index)
{
	if(Index > MAX_MUM_OUTPUT)
	{
		return;
	}
	bsp_set_gpio(Index,!g_tGpioOutputConfig[Index].ActLevel);
}

void bsp_set_gpio_default(GPIO_OUTPUT_LIST Index)
{
	if(Index > MAX_MUM_OUTPUT)
	{
		return;
	}
	bsp_set_gpio(Index,g_tGpioOutputConfig[Index].DefaultLevel);
}

uint8_t bsp_get_output(GPIO_OUTPUT_LIST Index)
{
	if(Index > MAX_MUM_OUTPUT)
	{
		return 3;
	}
	return GPIO_ReadOutputBit(g_tGpioOutputConfig[Index].Port, g_tGpioOutputConfig[Index].Pin);
}

uint8_t bsp_get_input(GPIO_INPUT_LIST Index)
{
	if(Index > MAX_MUM_INPUT)
	{
		return 3;
	}
	return GPIO_ReadInputBit(g_tGpioInputConfig[Index].Port,g_tGpioInputConfig[Index].Pin);
}

