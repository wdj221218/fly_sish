/*
 * app_mode_judge.c
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#include "app_mode_judge.h"

event_s g_tsys_event;
single_info_s ref_single = {{3000,3000,3000,3000,0},SINGLE_IDLE,SINGLE_IDLE,2,0,0};
single_info_s heat_single = {{3000,3000,3000,3000,0},SINGLE_IDLE,SINGLE_IDLE,2,0,0};

void start_mode_cb(void)
{
	DataBase_Push(sys_power_set,1);
}

void ref_heat_judge(void)
{
	int16_t temp_ref;
	int16_t temp_heat;
	single_st_e ref_single_temp = SINGLE_IDLE;
	single_st_e heat_single_temp = SINGLE_IDLE;
	uint16_t ref_backlash_temp;
	if(in_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_ref = DataBase_Get(sys_in_water_temp) - DataBase_Get(sys_in_water_ctl_ref_temp);
		temp_heat = DataBase_Get(sys_in_water_ctl_heat_temp) -  DataBase_Get(sys_in_water_temp);
		DataBase_Push(sys_ref_temp,DataBase_Get(sys_in_water_ctl_ref_temp));
		DataBase_Push(sys_heat_temp,DataBase_Get(sys_in_water_ctl_heat_temp));
	}
	else if(out_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_ref = DataBase_Get(sys_out_water_temp) - DataBase_Get(sys_out_water_ctl_ref_temp);
		temp_heat = DataBase_Get(sys_out_water_ctl_heat_temp) -  DataBase_Get(sys_out_water_temp);
		DataBase_Push(sys_ref_temp,DataBase_Get(sys_out_water_ctl_ref_temp));
		DataBase_Push(sys_heat_temp,DataBase_Get(sys_out_water_ctl_heat_temp));
	}
	else if(bms_avr_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_ref = DataBase_Get(sys_bms_temp_avr) - DataBase_Get(sys_bms_temp_ctl_ref_temp);
		temp_heat = DataBase_Get(sys_bms_temp_ctl_heat_temp) -  DataBase_Get(sys_bms_temp_avr);
		DataBase_Push(sys_ref_temp,DataBase_Get(sys_bms_temp_ctl_ref_temp));
		DataBase_Push(sys_heat_temp,DataBase_Get(sys_bms_temp_ctl_heat_temp));
	}

	if(temp_ref > (int16_t)(DataBase_Get(sys_ref_backlash)))
	{
		ref_single_temp = single_input(&ref_single,ENTER);
	}
	else if((temp_ref < -(int16_t)(DataBase_Get(sys_ref_backlash))) && (DataBase_Get(sys_environment_temp) > 100))
	{
		ref_single_temp = single_input(&ref_single,EXIT);
	}
	else
	{
		ref_single_temp = single_input(&ref_single,NORMAL);
	}

	if(ref_single_temp != SINGLE_IDLE)
	{
		if(ENTER == single_get_st(&ref_single))
		{
			g_tsys_event.enter_refrig = 1;
			g_tsys_event.exit_refrig = 0;
		}
		else if(EXIT == single_get_st(&ref_single))
		{
			g_tsys_event.enter_refrig = 0;
			g_tsys_event.exit_refrig = 1;
		}
		else if(NORMAL == single_get_st(&ref_single))
		{
			g_tsys_event.enter_refrig = 0;
			g_tsys_event.exit_refrig = 0;
		}
	}

	if(temp_heat > (int16_t)(DataBase_Get(sys_heat_backlash)))
	{
		heat_single_temp = single_input(&heat_single,ENTER);
	}
	else if(temp_heat <= 0)
	{
		heat_single_temp = single_input(&heat_single,EXIT);
	}
	else
	{
		heat_single_temp = single_input(&heat_single,NORMAL);
	}

	if(heat_single_temp != SINGLE_IDLE)
	{
		if(ENTER == single_get_st(&heat_single))
		{
			g_tsys_event.enter_heat = 1;
			g_tsys_event.exit_heat = 0;
		}
		else if(EXIT == single_get_st(&heat_single))
		{
			g_tsys_event.enter_heat = 0;
			g_tsys_event.exit_heat = 1;
		}
		else if(NORMAL == single_get_st(&heat_single))
		{
			g_tsys_event.enter_heat = 0;
			g_tsys_event.exit_heat = 0;
		}
	}
}

void run_mode_judge(void)
{
	if(auto_mode == DataBase_Get(sys_unit_mode_set))
	{
		g_tsys_event.auto_mode = 1;
	}
	else
	{
		g_tsys_event.auto_mode = 0;
	}

    if (self_cir_mode == DataBase_Get(sys_unit_mode_set))
	{
		g_tsys_event.self_cir_mode = 1;
	}
	else
	{
		g_tsys_event.self_cir_mode = 0;
	}

    if (heat_mode == DataBase_Get(sys_unit_mode_set) && (DataBase_Get(sys_environment_temp) > 100))
	{
		g_tsys_event.heat_mode = 1;
	}
	else
	{
		g_tsys_event.heat_mode = 0;
	}

	if (ref_mode == DataBase_Get(sys_unit_mode_set))
	{
		g_tsys_event.refrig_mode = 1;
	}
	else
	{
		g_tsys_event.refrig_mode = 0;
	}
}

void unit_mode_judge(void)
{
    if (1 == DataBase_Get(sys_power_set))
	{
		g_tsys_event.power_on_mode = 1;
		g_tsys_event.power_off_mode = 0;
	}
	else
	{
		g_tsys_event.power_on_mode = 0;
		g_tsys_event.power_off_mode = 1;
	}

    if (1 == DataBase_Get(sys_test_set))
	{
		g_tsys_event.test_mode = 1;
	}
	else
	{
		g_tsys_event.test_mode = 0;
	}

    if (1 == DataBase_Get(sys_manual_set))
	{
		g_tsys_event.manual_mode = 1;
	}
	else
	{
		g_tsys_event.manual_mode = 0;
	}

    if (1 == DataBase_Get(sys_vacuum_set))
	{
		g_tsys_event.vacuum_mode = 1;
	}
	else
	{
		g_tsys_event.vacuum_mode = 0;
	}

}

void sys_event_judge(void)
{
	ref_heat_judge();
	unit_mode_judge();
}

void unit_mode_manage(void)
{
	static uint32_t timer = 0;
	switch (DataBase_Get(sys_unit_mode))
	{
		case power_off_mode:
			if(1 == g_tsys_event.power_on_mode)
			{
				DataBase_Push(sys_unit_mode_next,power_on_mode);
			}
			else if(1 == g_tsys_event.test_mode)
			{
				DataBase_Push(sys_unit_mode_next,test_mode);
			}
			else if(1 == g_tsys_event.manual_mode)
			{
				DataBase_Push(sys_unit_mode_next,manual_mode);
			}
			else if(1 == g_tsys_event.vacuum_mode)
			{
				DataBase_Push(sys_unit_mode_next,vacuum_mode);
			}
		break;

		case power_on_mode:
			if(1 == g_tsys_event.power_off_mode)
			{
				DataBase_Push(sys_unit_mode_next,power_off_mode);
			}
		break;

		case test_mode:
			if((0 == timer) && (1 == g_tsys_event.test_mode))
			{
				timer = get_tick_count();
			}

			if(get_elapsed_tick_count(timer) > DataBase_Get(sys_test_hold_time) * 60000)
			{
				DataBase_Push(sys_test_set,0);
				timer = 0;
			}

			if(0 == g_tsys_event.test_mode)
			{
				timer = 0;
				DataBase_Push(sys_unit_mode_next,power_off_mode);
			}
		break;

		case manual_mode:
			if(0 == g_tsys_event.manual_mode)
			{
				DataBase_Push(sys_unit_mode_next,power_off_mode);
			}			
		break;

		case vacuum_mode:
			if(0 == g_tsys_event.vacuum_mode)
			{
				DataBase_Push(sys_unit_mode_next,power_off_mode);
			}	
		break;

		default:
		break;
}
}

void run_mode_manage(void)
{
	if(DataBase_Get(sys_unit_mode) != power_on_mode)
	{
		return;
	}

	if(DataBase_Get(sys_run_mode) != auto_mode)
	{
		return;
	}

	switch (DataBase_Get(sys_auto_run_mode))
	{
		case auto_self_cir:
			if(1 == g_tsys_event.enter_refrig)
			{
				DataBase_Push(sys_auto_run_mode_next,auto_ref);
			}

			if(1 == g_tsys_event.enter_heat)
			{
				DataBase_Push(sys_auto_run_mode_next,auto_heat);
			}

		break;

		case auto_heat:
			if(1 == g_tsys_event.exit_heat)
			{
				DataBase_Push(sys_auto_run_mode_next,auto_self_cir);
			}
		break;

		case auto_ref:
			if(1 == g_tsys_event.exit_refrig)
			{
				DataBase_Push(sys_auto_run_mode_next,auto_self_cir);
			}
		break;
		
		default:
		break;
	}
}

void app_mode_judge_handle(void)
{
	sys_event_judge();
	unit_mode_manage();
	run_mode_manage();
}


