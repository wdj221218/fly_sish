/*
 * app_unit_run.c
 *
 *  Created on: 2024年3月25日
 *      Author: Dell
 */

#include "app_unit_run.h"
unit_run_state_s g_tunit_run_state;
relay_out_state_s g_trelay_out_state;
relay_out_state_pc_s g_trelay_out_state_pc;
uint8_t power_off_set(void)
{
	switch (DataBase_Get(sys_run_mode))
	{
		case auto_mode:
			if(1 == auto_mode_exit())
			{
				app_pump_off();
				return 1;					
			}
			return 0;
		break;

		case self_cir_mode:
			if(1 == self_cir_mode_exit())
			{
				app_pump_off();
				return 1;
			}
			return 0;
		break;

		case heat_mode:
			if(1 == heat_mode_exit())
			{
				app_pump_off();
				return 1;
			}
			return 0;
		break;

		case ref_mode:
			if(1 == ref_mode_exit())
			{
				app_pump_off();
				return 1;
			}
			return 0;
		break;

		default:
			return 0;
		break;
	}
}

void power_off_handle(void)
{
	DataBase_Push(sys_unit_mode,DataBase_Get(sys_unit_mode_next));
	g_tunit_run_state.unit_mode_state = unit_mode_power_off;
	g_tunit_run_state.unit_run_state = unit_run_idle;
}

void power_on_handle(void)
{
	if(power_off_mode == DataBase_Get(sys_unit_mode_next))
	{
		if(1 == power_off_set())
		{
			DataBase_Push(sys_unit_mode,DataBase_Get(sys_unit_mode_next));
		}
		return;
	}
	
	switch (DataBase_Get(sys_run_mode))
	{
		case auto_mode:
			if(auto_mode == DataBase_Get(sys_run_mode_next))
			{
				auto_mode_run();
			}
			else
			{
				if(1 == auto_mode_exit())
				{
					DataBase_Push(sys_run_mode,DataBase_Get(sys_run_mode_next));
				}
			}
		break;

		case self_cir_mode:
			if(self_cir_mode == DataBase_Get(sys_run_mode_next))
			{
				self_cir_mode_run();
			}
			else
			{
				if(1 == self_cir_mode_exit())
				{
					DataBase_Push(sys_run_mode,DataBase_Get(sys_run_mode_next));
				}
			}
		break;

		case heat_mode:
			if(heat_mode == DataBase_Get(sys_run_mode_next))
			{
				heat_mode_run(mode_run);
			}
			else
			{
				if((mode_stop == heat_mode_run(mode_stop)) || (mode_err == heat_mode_run(mode_stop)))
				{
					DataBase_Push(sys_run_mode,DataBase_Get(sys_run_mode_next));
				}
			}
		break;

		case ref_mode:
			if(ref_mode == DataBase_Get(sys_run_mode_next))
			{
				ref_mode_run(mode_run);
			}
			else
			{
				if((mode_stop == ref_mode_run(mode_stop)) || (mode_err == ref_mode_run(mode_stop)))
				{
					DataBase_Push(sys_run_mode,DataBase_Get(sys_run_mode_next));
				}
			}
		break;
	
		default:

		break;
	}
}

void test_handle(void)
{
	if(power_off_mode == DataBase_Get(sys_unit_mode_next))
	{
		if(1 == test_mode_exit())
		{
			DataBase_Push(sys_unit_mode,DataBase_Get(sys_unit_mode_next));
		}
		return;
	}
	test_mode_run();
}

void manual_handle(void)
{
	if(power_off_mode == DataBase_Get(sys_unit_mode_next))
	{
		if(1 == manual_mode_exit())
		{
			DataBase_Push(sys_unit_mode,DataBase_Get(sys_unit_mode_next));
		}
		return;
	}
	manual_mode_run();
}

void vacuum_handle(void)
{
	if(power_off_mode == DataBase_Get(sys_unit_mode_next))
	{
		if(1 == vacuum_mode_exit())
		{
			DataBase_Push(sys_unit_mode,DataBase_Get(sys_unit_mode_next));
		}
		return;
	}
	vacuum_mode_run();
}

void app_unit_mode_put(void)
{
	switch (DataBase_Get(sys_unit_mode))
	{
		case power_off_mode:
			DataBase_Push(sys_power_state,0);
			DataBase_Push(sys_mode_run,sys_other);
			DataBase_Push(sys_mode,sys_power_off);
		break;

		case power_on_mode:
			DataBase_Push(sys_power_state,1);
			switch (DataBase_Get(sys_run_mode))
			{
				case auto_mode:
					DataBase_Push(sys_mode,sys_auto_mode);
					switch (DataBase_Get(sys_auto_run_mode))
					{
						case auto_self_cir:
							DataBase_Push(sys_mode_run,sys_self_cir);
						break;

						case auto_heat:
							DataBase_Push(sys_mode_run,sys_heat);
						break;

						case auto_ref:
							DataBase_Push(sys_mode_run,sys_ref);
						break;

						default:

						break;
					}
				break;

				case self_cir_mode:
					DataBase_Push(sys_mode,sys_self_cir_mode);
					DataBase_Push(sys_mode_run,sys_self_cir);
				break;

				case heat_mode:
					DataBase_Push(sys_mode,sys_heat_mode);
					DataBase_Push(sys_mode_run,sys_heat);
				break;

				case ref_mode:
					DataBase_Push(sys_mode,sys_ref_mode);
					DataBase_Push(sys_mode_run,sys_ref);
				break;
			
				default:

				break;
			}
		break;

		case test_mode:
			DataBase_Push(sys_mode,sys_test_mode);
			switch (DataBase_Get(sys_test_run_mode))
			{
				case test_self_cir:
					DataBase_Push(sys_mode_run,sys_self_cir);
				break;

				case test_heat:
					DataBase_Push(sys_mode_run,sys_heat);
				break;

				case test_ref:
					DataBase_Push(sys_mode_run,sys_ref);
				break;
				
				default:
				
				break;
			}
		break;

		case manual_mode:
			DataBase_Push(sys_mode,sys_manual_mode);
			DataBase_Push(sys_mode_run,sys_other);
		break;

		case vacuum_mode:
			DataBase_Push(sys_mode,sys_vacumm_mode);
			DataBase_Push(sys_mode_run,sys_other);
		break;
		
		default:
		break;
	}
}

void relay_out_state_get(void)
{
	if(1 == bsp_get_output(pump_pwr_ctl))
	{
		g_trelay_out_state.relay_pump = 1;
		g_trelay_out_state_pc.relay_pump = 1;
	}
	else
	{
		g_trelay_out_state.relay_pump = 0;
		g_trelay_out_state_pc.relay_pump = 0;
	}

	if(1 == bsp_get_output(fan_pwr_ctl))
	{
		g_trelay_out_state.relay_fan = 1;
		g_trelay_out_state_pc.relay_fan = 1;
	}
	else
	{
		g_trelay_out_state.relay_fan = 0;
		g_trelay_out_state_pc.relay_fan = 0;
	}

	if(1 == bsp_get_output(ptc1_ctl))
	{
		g_trelay_out_state.relay_ptc = 1;
		g_trelay_out_state_pc.relay_ptc = 1;
	}
	else
	{
		g_trelay_out_state.relay_ptc = 0;
		g_trelay_out_state_pc.relay_ptc = 0;
	}

	if(1 == bsp_get_output(ptc2_ctl))
	{
		g_trelay_out_state.relay_belt = 1;
		g_trelay_out_state_pc.relay_belt = 1;
	}
	else
	{
		g_trelay_out_state.relay_belt = 0;
		g_trelay_out_state_pc.relay_belt = 0;
	}
	DataBase_Push(sys_relay_out,*(uint16_t*)(&g_trelay_out_state));
	DataBase_Push(sys_relay_out_pc,*(uint16_t*)(&g_trelay_out_state_pc));
}

void app_unit_run(void)
{
	switch (DataBase_Get(sys_unit_mode))
	{
		case power_off_mode:
			power_off_handle();
		break;

		case power_on_mode:
			power_on_handle();
		break;

		case test_mode:
			test_handle();
		break;

		case manual_mode:
			manual_handle();
		break;

		case vacuum_mode:
			vacuum_handle();
		break;
		
		default:
		break;
	}
	app_unit_mode_put();
	relay_out_state_get();
	DataBase_Push(sys_mods_state,*(uint16_t*)(&g_tunit_run_state));
}

