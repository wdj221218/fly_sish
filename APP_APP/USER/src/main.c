/*
 * main.c
 *
 *  Created on: 2024年3月18日
 *      Author: Dell
 */
#include "bsp.h"
#include "mid.h"
#include "app.h"

uint16_t cmd = 0;

uint16_t app_soft = 0;
uint16_t sbl_soft = 0;
__attribute__((section(".target_info")))
const volatile uint8_t  a_ver[4] = {1,0,0,2};
void soft_version_get(void)
{
	app_soft = a_ver[0] * 1000 + a_ver[1] * 100 + a_ver[2] * 10 + a_ver[3];
	sbl_soft = *(uint8_t*)(SBL_VERSION) * 1000 + *(uint8_t*)(SBL_VERSION + 1) * 100 + *(uint8_t*)(SBL_VERSION + 2) * 10 + *(uint8_t*)(SBL_VERSION + 3);
	DataBase_Push(sys_app_version,app_soft);
	DataBase_Push(sys_sbl_version,sbl_soft);
}

#define LOG_HEAD LOG_ID_SYS

int main(void)
{
	__set_PRIMASK(0);
	bsp_systick_init();
	Param_Init();
	DataBase_Init();
	bsp_adc_init();
	bsp_gpio_init();
	app_fan_fb_init();

	bsp_pwm_init();
	bsp_uart_init();
	key_data_init();
	bl24c02_init();
	Expansion_IO_Init();
	TaskListInit();
	soft_version_get();
	Time_init();
 	app_wdt_init();

	if(1 == DataBase_Get(sys_start_mode))
	{
		sys_timer_start(SYS_TIMER_ID_START,SYS_TIMR_ID_ONE,30000,start_mode_cb);
	}

	app_ref_mode_set(DataBase_Get(sys_unit_select));

	sys_timer_start(SYS_TIMER_ID_MODS_M,SYS_TIMR_ID_ONE,30000,modbus_master_cb);
	sys_timer_start(SYS_TIMER_ID_BMS,SYS_TIMR_ID_ONE,30000,bms_community_cb);
	WDBG(LOG_ID_SUB_SYS_INIT,"system init....");

	while(1)
	{
		sys_timer_server();
		app_uart_handle();
		Task_Schedule();
	}
}

