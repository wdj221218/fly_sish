/*
 * main.c
 *
 *  Created on: 2024年3月18日
 *      Author: Dell
 */

#include "bsp.h"
#include "mid.h"
#include "app.h"
__attribute__((section(".target_info")))
const volatile uint8_t  b_ver[4] = {1,0,0,0};
void main()
{
	__set_PRIMASK(0);
	bsp_systick_init();
	Param_Init();
	DataBase_Init();
	bsp_adc_init();
	bsp_gpio_init();
	bsp_uart_init();
	TaskListInit();

	uint32_t timer = get_tick_count();
	while(get_elapsed_tick_count(timer) < 1000)
	{
		Uart_Handle();
	}

	BOOT_Check();
	while(1)
	{
		if(bsp_get_volt_24V() > 7000)
		{
			Uart_Handle();
			Progress_Check();
		}

		Task2msRun();
		Task10msRun();
		Task20msRun();
		Task50msRun();
		Task200msRun();
		Task1000msRun();
	}
}

