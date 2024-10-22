/*
 * bsp_gpio.h
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_GPIO_H_
#define COMMON_BSP_INC_BSP_GPIO_H_

#include <stdint.h>
#include "apm32f10x_gpio.h"
#include "apm32f10x_rcm.h"

typedef enum
{
	ptc1_ctl,
	ptc2_ctl,
	fan_pwr_ctl,
	pump_pwr_ctl,
	pres_pwr_ctl,
	temp_pwr_ctl,
	dcf_ctl,
	jcq_ctl,
	kg_ctl,
	ee_wc,
	pcs_pump_pwr_ctl,
	led1,
	led2,
	led3,
	MAX_MUM_OUTPUT
} GPIO_OUTPUT_LIST;

typedef enum
{
	ptc_err_chk,
	pump_err_chk,
	force_chk,
	tank_chk,
	jcq_chk,
	fan1_fb,
	fan2_fb,
	fan3_fb,
	valve_on_check,
	valve_open_check,
	valve_close_check,
	MAX_MUM_INPUT
} GPIO_INPUT_LIST;


typedef struct 
{
	uint8_t 		Index;
	GPIO_T* 		Port;
	GPIO_PIN_T   	Pin;
	uint8_t 		ActLevel;
	uint8_t			DefaultLevel;
} bsp_gpio_str;

void bsp_gpio_init(void);
void bsp_set_gpio_act(GPIO_OUTPUT_LIST Index);
void bsp_set_gpio_unact(GPIO_OUTPUT_LIST Index);
void bsp_set_gpio_default(GPIO_OUTPUT_LIST Index);
uint8_t bsp_get_output(GPIO_OUTPUT_LIST Index);
uint8_t bsp_get_input(GPIO_INPUT_LIST Index);

#endif /* COMMON_BSP_INC_BSP_GPIO_H_ */
