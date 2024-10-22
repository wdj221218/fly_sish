/*
 * app_valve_set.c
 *
 *  Created on: 2024年8月30日
 *      Author: Dell
 */

#include "app_valve_control.h"

void app_valve_set(valve_state_e state)
{
	if(0 == DataBase_Get(sys_unit_select))
	{
		return;
	}

	if(1 == state)
	{
		if(1 == DataBase_Get(sys_valve_state))
		{
			return;
		}
		bsp_set_gpio_act(dcf_ctl);
		DataBase_Push(sys_valve_state,state);
	}
	else if(0 == state)
	{
		if(0 == DataBase_Get(sys_valve_state))
		{
			return;
		}
		bsp_set_gpio_unact(dcf_ctl);
		DataBase_Push(sys_valve_state,state);
	}
}



