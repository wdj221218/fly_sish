/*
 * app_manual_mode.c
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */
#include "app_manual_mode.h"

void manual_mode_run(void)
{
	if(0 == DataBase_Get(sys_manual_pump_level))
	{
		if((0 == DataBase_Get(sys_motor_speed_now)) && (0 == DataBase_Get(sys_manual_motor_speed)))
		{
			app_pump_run_manual(DataBase_Get(sys_manual_pump_level));
		}
	}
	else
	{
		app_pump_run_manual(DataBase_Get(sys_manual_pump_level));
	}

	if(0 == DataBase_Get(sys_manual_fan_level))
	{
		if((0 == DataBase_Get(sys_motor_speed_now)) && (0 == DataBase_Get(sys_manual_motor_speed)))
		{
			app_fan_run_manual(DataBase_Get(sys_manual_fan_level));
		}
	}
	else
	{
		app_fan_run_manual(DataBase_Get(sys_manual_fan_level));
	}

	if(0 == DataBase_Get(sys_manual_expan_open))
	{
		if((0 == DataBase_Get(sys_motor_speed_now)) && (0 == DataBase_Get(sys_manual_motor_speed)))
		{
			app_expan_run_manual(DataBase_Get(sys_manual_expan_open));
		}
	}
	else
	{
		app_expan_run_manual(DataBase_Get(sys_manual_expan_open));
	}

    if(DataBase_Get(sys_manual_motor_speed) > 0)
    {
    	if((DataBase_Get(sys_expan_open) > 0) && (DataBase_Get(sys_fan_level) > 0))
    	{
    		if(DataBase_Get(sys_manual_motor_speed) < 30)
    		{
    			app_compress_run_manual(30);
    		}
    		else if(DataBase_Get(sys_manual_motor_speed) > 110)
    		{
    			app_compress_run_manual(110);
    		}
    		else
    		{
    			app_compress_run_manual(DataBase_Get(sys_manual_motor_speed));
    		}
    	}
    	else
    	{
    		app_compress_run_manual(0);
    	}
    }
    else
    {
    	app_compress_run_manual(DataBase_Get(sys_manual_motor_speed));
    }

    app_ptc_control_manual(DataBase_Get(sys_manual_ptc_state));
    app_belt_control_manual(DataBase_Get(sys_manual_heat_belt_state));
    app_valve_set(DataBase_Get(sys_manual_valve_state));
}

uint8_t manual_mode_exit(void)
{
    app_pump_run_manual(0);
    app_fan_run_manual(0);
    app_ptc_control_manual(0);
    app_belt_control_manual(0);
    app_expan_run_manual(0);
    app_compress_run_manual(0);
	if(0 == DataBase_Get(sys_motor_speed_now))
	{
		app_expan_run_manual(0);
	}

    if((0 == DataBase_Get(sys_pump_level)) && (0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_ptc_state)) && \
       (0 == DataBase_Get(sys_heat_belt_state)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
    {
        return 1;
    }
    else
    {
        return 0;
    }

}

