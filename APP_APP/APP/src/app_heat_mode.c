/*
 * app_heat_mode.c
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */
#include "app_heat_mode.h"
extern  fault_in_value_s g_tfault_in_value[fault_diag_max];
extern uint8_t heat_err;
extern unit_run_state_s g_tunit_run_state;

void heat_start_time_get(uint32_t *ptc_start_time,uint32_t *pump_start_time)
{
    *ptc_start_time = get_tick_count() + 15000;
    *pump_start_time = get_tick_count();
}

void heat_end_time_get(uint32_t *ptc_end_time,uint32_t *pump_end_time)
{
    *ptc_end_time = get_tick_count();
    *pump_end_time = get_tick_count() + 5000;
}


mode_run_state_e heat_mode_run(mode_run_state_e state_next)
{
    static mode_run_state_e state = mode_init;
    uint32_t ptc_start_time = 0;
    uint32_t pump_start_time = 0;
    uint32_t ptc_end_time = 0;
    uint32_t pump_end_time = 0;
    g_tunit_run_state.unit_mode_state = unit_mode_heat;
    if((1 == heat_err) || (DataBase_Get(sys_environment_temp) < 100))
    {
        state = mode_err;
    }

    switch (state)
    {
        case mode_init:
            app_ptc_control(PTC_ON);
            app_pump_run_ref_heat();
            if((1 == app_get_ptc()) && (3 == DataBase_Get(sys_pump_level)))
            {
                state = mode_run;
            }
            g_tunit_run_state.unit_run_state = unit_run_run;
        break;

        case mode_run:
            if(mode_stop == state_next)
            {
                app_ptc_control(PTC_OFF);
               
                if(get_tick_count() > pump_end_time)//待定
                {
                    app_pump_run_ref_heat();
                }
            }

            if(0 == app_get_ptc())
            {
                state = mode_stop;
            }
            g_tunit_run_state.unit_run_state = unit_run_run;
        break;

        case mode_stop:
            if((mode_stop != state_next) && (mode_err != state_next))
            {
                state = mode_init;
            }
        break;

        case mode_err:
            if((0 == heat_err) && (DataBase_Get(sys_environment_temp) >= 100))
            {
                state = mode_init;
            }
            app_ptc_control(PTC_ERR);
            g_tunit_run_state.unit_run_state = unit_run_fault;
        break;

        default:
        break;
    }
    return state;
}
uint8_t heat_mode_exit(void)
{
	if((mode_stop == heat_mode_run(mode_stop)) || (mode_err == heat_mode_run(mode_stop)))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
