/*
 * app_test_mode.c
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */
#include "app_test_mode.h"

void test_mode_run()
{
    switch (DataBase_Get(sys_test_run_mode))
    {
        case test_self_cir:
            self_cir_mode_run();
			if(test_self_cir != DataBase_Get(sys_test_run_mode_next))
			{
				DataBase_Push(sys_test_run_mode,DataBase_Get(sys_test_run_mode_next));
			}       
        break;

        case test_heat:
 			if(test_heat == DataBase_Get(sys_test_run_mode_next))
			{
				heat_mode_run(mode_run);
			}
			else
			{
				if(mode_stop == heat_mode_run(mode_stop))
				{
					DataBase_Push(sys_test_run_mode,DataBase_Get(sys_test_run_mode_next));
				}
			}       
        break;

        case test_ref:
         	if(test_ref == DataBase_Get(sys_test_run_mode_next))
			{
				ref_mode_run(mode_run);
			}
			else
			{
				if(mode_stop == ref_mode_run(mode_stop))
				{
					DataBase_Push(sys_test_run_mode,DataBase_Get(sys_test_run_mode_next));
				}
			}   
        break;
        
        default:
        
        break;
    }
}

uint8_t test_mode_exit(void)
{
	switch (DataBase_Get(sys_test_run_mode))
	{
		case test_self_cir:
			app_pump_off();
			DataBase_Push(sys_test_run_mode,test_self_cir);
			return 1;
		break;

		case test_heat:
			if(mode_stop == heat_mode_run(mode_stop))
			{
				app_pump_off();
				DataBase_Push(sys_test_run_mode,test_self_cir);
				return 1;
			}
			return 0;
		break;

		case test_ref:
			if(mode_stop == ref_mode_run(mode_power_off))
			{
				app_pump_off();
				DataBase_Push(sys_test_run_mode,test_self_cir);
				return 1;
			}
			return 0;
		break;

		default:
			return 0;
		break;
	}
}
