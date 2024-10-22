/*
 * app_auto_mode.c
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */
#include "app_auto_mode.h"

void auto_mode_run(void)
{
	static uint32_t timer = 0;

	switch (DataBase_Get(sys_auto_run_mode))
	{
		case auto_self_cir:
			self_cir_mode_run();
			if(auto_self_cir != DataBase_Get(sys_auto_run_mode_next))
			{
				DataBase_Push(sys_auto_run_mode,DataBase_Get(sys_auto_run_mode_next));
			}
		break;

		case auto_heat:
			if(auto_heat == DataBase_Get(sys_auto_run_mode_next))
			{
				heat_mode_run(mode_run);
			}
			else
			{
				if(mode_stop == heat_mode_run(mode_stop))
				{
					DataBase_Push(sys_auto_run_mode,DataBase_Get(sys_auto_run_mode_next));
				}
			}
		break;

		case auto_ref:
			if(auto_ref == DataBase_Get(sys_auto_run_mode_next))
			{
				ref_mode_run(mode_run);
			}
			else
			{
				if(mode_stop == ref_mode_run(mode_end))
				{
					DataBase_Push(sys_auto_run_mode,DataBase_Get(sys_auto_run_mode_next));
				}
			}
		break;

		default:

		break;
	}
}

uint8_t auto_mode_exit(void)
{
//	if (power_off_mode != DataBase_Get(sys_unit_mode_next))
//	{
//		return 0;
//	}

	switch (DataBase_Get(sys_auto_run_mode))
	{
		case auto_self_cir:
			DataBase_Push(sys_auto_run_mode,auto_self_cir);
			return 1;
		break;

		case auto_heat:
			if((mode_stop == heat_mode_run(mode_stop)) || (mode_err == heat_mode_run(mode_stop)))
			{
				DataBase_Push(sys_auto_run_mode,auto_self_cir);
				return 1;
			}
			return 0;
		break;

		case auto_ref:

			if(mode_stop == ref_mode_run(mode_power_off))
			{
				DataBase_Push(sys_auto_run_mode,auto_self_cir);
				return 1;
			}
			return 0;
		break;

		default:
			return 0;
		break;
	}
}
