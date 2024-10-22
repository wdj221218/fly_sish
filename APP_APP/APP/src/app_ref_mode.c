/*
 * app_ref_mode.c
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */
#include "app_ref_mode.h"
extern  fault_in_value_s g_tfault_in_value[fault_diag_max];
extern fault_out_s g_tfault_out;
extern uint8_t ref_err;
extern uint8_t low_ref_err;
extern uint8_t expan_init;
extern single_info_s low_pres_single;
extern unit_run_state_s g_tunit_run_state;

u8 ref_mode_set = 0;

void ref_part_start_time(uint32_t motor_end_time,uint32_t *part_start_time)
{
    if((0 == motor_end_time) || (get_elapsed_tick_count(motor_end_time) > 180000))//3min
    {
        *part_start_time = get_tick_count();
    }
    else
    {
        *part_start_time = motor_end_time + 180000;

    }
}

void ref_end_delay_time(uint32_t motor_start_time,uint32_t *motor_end_time)
{

    if((get_elapsed_tick_count(motor_start_time) > 300000) || (0 == motor_start_time))//5min
    {
        *motor_end_time = get_tick_count();
    }
    else
    {
        *motor_end_time = motor_start_time + 300000;
    }
}

void ref_end_immed_time(uint32_t *motor_err_time)
{
    *motor_err_time = get_tick_count();
}

uint8_t motor_run_temp_judge(void)
{
    static uint32_t timer1 = 0;
    static uint32_t timer2 = 0;

    if(DataBase_Get(sys_environment_temp) > DataBase_Get(sys_ref_max_temp))
    {
        if(0 == timer1)
        {
            timer1 = get_tick_count();
        }
        if(get_elapsed_tick_count(timer1) > 3000)
        {
            return 1;
            timer1 = 0;
        }
        timer2 = 0;
    }
    else if (DataBase_Get(sys_environment_temp) <= DataBase_Get(sys_ref_min_temp))
    {
        if(0 == timer2)
        {
            timer2 = get_tick_count();
        }
        if(get_elapsed_tick_count(timer2) > 3000)
        {
            return 1;
            timer2 = 0;
        }
        timer1 = 0;
    }

    return 0;
}

void in_pres_low_check_speed_0(void)
{
    single_st_e temp = SINGLE_IDLE;
    if(1 == DataBase_Get(sys_fault_shield))
    {
        return;
    }
    
    if((DataBase_Get(sys_in_pres_trans_temp) < DataBase_Get(sys_low_pres_protect_temp)) && (0 == DataBase_Get(sys_motor_speed_now)))
    {
    	temp = single_input(&low_pres_single,SINGLE_ST1);
    }
    else
    {
    	temp = single_input(&low_pres_single,SINGLE_ST2);
    }

    if(SINGLE_ST1 == temp)
    {
    	if(0 == g_tfault_in_value[in_motor_pres_low].value)
    	{
            g_tfault_in_value[in_motor_pres_low].value = 1;
            g_tfault_out.in_motor_pres_low = 1;
            DataBase_Push(sys_in_motor_pres_low_err_num,DataBase_Get(sys_in_motor_pres_low_err_num) + 1);
    		key_data_put(fault_con,fault_in_motor_pres_low);
    		app_fault_check_en_set(idx_in_motor_pres_low_check,0);
            app_fault_code_put();
    	}
    }
}

mode_run_state_e ref_mode_run_single(mode_run_state_e state_next)
{
    static mode_run_state_e state = mode_idle;
    static uint32_t part_start_time = 0;
    static uint32_t motor_start_time = 0;
    static uint32_t motor_end_time = 0;
    static uint32_t start_time = 0;
    static uint32_t end_time = 0;
    uint16_t temp_now = 0;

    g_tunit_run_state.unit_mode_state = unit_mode_ref;
	if(in_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_now = DataBase_Get(sys_in_water_temp);
	}
	else if(out_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_now = DataBase_Get(sys_out_water_temp);
	}
	else if(bms_avr_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_now = DataBase_Get(sys_bms_temp_avr);
	}

    in_pres_low_check_speed_0();

    if((0 == DataBase_Get(sys_motor_speed_now)) && (DataBase_Get(sys_out_motor_temp) > 1350))
    {
    	if(mode_err != state)
    	{
            state = mode_err;
            ref_end_immed_time(&motor_end_time);
    	}
    }

    if(((1 == ref_err) || (1 == motor_run_temp_judge())))
    {
    	if((mode_err != state) && (mode_stop != state))
    	{
            state = mode_err;
            ref_end_immed_time(&motor_end_time);
    	}
    }
    else if(mode_idle == state)
    {
        state = mode_init;
        ref_part_start_time(DataBase_Get(sys_motor_end_time),&part_start_time);
    }

    switch (state)
    {
        case mode_idle:

        break;

        case mode_init:
            if(get_tick_count() > part_start_time)
            {
            	if(0 == start_time)
            	{
            		start_time = get_tick_count();
            	}

                app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Init_State_Fan);
                app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_run);
                app_pump_run_ref_heat();
            }


            if((get_elapsed_tick_count(start_time) > 15000) &&
               (0 != start_time) &&
               (DataBase_Get(sys_pump_level) > 0) &&
			   (DataBase_Get(sys_expan_open) > 10) &&
			   (DataBase_Get(sys_fan_level) > 0))
            {
                app_compressor_run(DataBase_Get(sys_environment_temp),temp_now,compre_run);
            }

            if((0 != DataBase_Get(sys_fan_level)) && ((0 != DataBase_Get(sys_expan_open)) && (1 == expan_init)) && (0 != DataBase_Get(sys_motor_speed_now)) && (3 == DataBase_Get(sys_pump_level)))
            {
                state = mode_run;
                start_time = 0;
            }

            if(mode_stop == state_next)
            {
            	ref_end_delay_time(DataBase_Get(sys_motor_start_time),&motor_end_time);
                state = mode_run_to_stop;
                start_time = 0;
            }
            else if(mode_power_off == state_next)
            {
            	ref_end_immed_time(&motor_end_time);
                state = mode_run_to_stop;
                start_time = 0;
            }
            else if(mode_end == state_next)
            {
            	ref_end_immed_time(&motor_end_time);
                state = mode_run_to_stop;
                start_time = 0;
            }

            g_tunit_run_state.unit_run_state = unit_run_wait_start;
        break;

        case mode_run:

            if(mode_stop == state_next)
            {
            	ref_end_delay_time(DataBase_Get(sys_motor_start_time),&motor_end_time);
                state = mode_run_to_stop;
            }
            else if(mode_power_off == state_next)
            {
            	ref_end_immed_time(&motor_end_time);
                state = mode_run_to_stop;
            }
            else if(mode_end == state_next)
            {
            	ref_end_immed_time(&motor_end_time);
                state = mode_run_to_stop;
                start_time = 0;
            }
            else
            {
                 app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Run_State_Fan);
                 app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_run);
                 app_pump_run_ref_heat();
                 app_compressor_run(DataBase_Get(sys_environment_temp), temp_now,compre_run);
            }
            g_tunit_run_state.unit_run_state = unit_run_run;
        break;

        case mode_run_to_stop:

            if(get_tick_count() > motor_end_time)
            {
                app_compressor_run(DataBase_Get(sys_environment_temp),temp_now,compre_stop);
            }

            if((0 == DataBase_Get(sys_motor_speed_now)) && (0 == end_time))
            {
            	end_time = get_tick_count();
            }

            if((get_elapsed_tick_count(end_time) > 10000) && (0 != end_time))
            {
            	 app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Stop_Status_Fan);
            	 app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_stop);
            	 app_pump_run_ref_heat();
            }

            if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
            {
                state = mode_stop;
                end_time = 0;
            }
            g_tunit_run_state.unit_run_state = unit_run_wait_stop;
        break;

        case mode_stop:
            if(mode_stop != state_next)
            {
                state = mode_idle;
            }
        break;

        case mode_err:
            if(0 == ref_err)
            {
                if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
                {
                    state = mode_idle;
                }
            }

            if((mode_stop == state_next) || (mode_power_off == state_next))
            {
                if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
                {
                    state = mode_stop;
                }
            }

            if(get_tick_count() > motor_end_time)
            {
                app_compressor_run(DataBase_Get(sys_environment_temp),temp_now,compre_err);
            }

            if((0 == DataBase_Get(sys_motor_speed_now)) && (0 == end_time))
            {
            	end_time = get_tick_count();
            }

            if((get_elapsed_tick_count(end_time) > 10000) && (0 != end_time))
            {
    			app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Err_State_Fan);
    			app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_err);
                app_pump_run_ref_heat();
            }

            if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
            {
                end_time = 0;
            }
            g_tunit_run_state.unit_run_state = unit_run_fault;
        break;

        default:
        break;
    }
    return state;
}

mode_run_state_e ref_mode_run_double(mode_run_state_e state_next)
{
    static mode_run_state_e run_state = mode_idle;
    static ref_run_mode_state ref_state = ref_idle;
    static uint32_t part_start_time = 0;
    static uint32_t motor_start_time = 0;
    static uint32_t motor_end_time = 0;
    static uint32_t start_time = 0;
    static uint32_t end_time = 0;
    static uint32_t time = 0;
    uint16_t temp_now = 0;
    int16_t temp_err = 0;

    uint16_t temp_target = 0;

    temp_target = DataBase_Get(sys_ref_temp);

    g_tunit_run_state.unit_mode_state = unit_mode_ref;

	if(in_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_now = DataBase_Get(sys_in_water_temp);
	}
	else if(out_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_now = DataBase_Get(sys_out_water_temp);
	}
	else if(bms_avr_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_now = DataBase_Get(sys_bms_temp_avr);
	}
	temp_err = temp_now - temp_target;
    switch (run_state)
    {
        case mode_idle:
        	run_state = mode_init;
            if(DataBase_Get(sys_environment_temp) < DataBase_Get(sys_valve_start_temp))
            {
            	ref_state = ref_low_power_init;
            }
            else
            {
            	ref_state = ref_normal_init;
            	ref_part_start_time(DataBase_Get(sys_motor_end_time),&part_start_time);
            }
        break;

        case mode_init:
        	if(ref_low_power_init == ref_state)
        	{
            	app_fan_run_low_power(temp_now,Init_State_Fan);
            	app_pump_run_ref_heat();
            	app_valve_set(valve_open);

            	if((3 == DataBase_Get(sys_fan_level)) && (3 == DataBase_Get(sys_pump_level)))
                {
            		run_state = mode_run;
            		ref_state = ref_low_power_run;
                }
            	g_tunit_run_state.unit_run_state = unit_run_wait_start;
        	}
        	else if(ref_normal_init == ref_state)
        	{
                in_pres_low_check_speed_0();

        	    if(((1 == ref_err) || (1 == motor_run_temp_judge())))
        	    {
					run_state = mode_err;
					ref_state = ref_normal_err;
					ref_end_immed_time(&motor_end_time);
        	    }

        	    if((0 == DataBase_Get(sys_motor_speed_now)) && (DataBase_Get(sys_out_motor_temp) > 1350))
        	    {
        	    	run_state = mode_err;
        	    	ref_state = ref_low_power_err;
					ref_end_immed_time(&motor_end_time);
        	    }

                if(get_tick_count() > part_start_time)
                {
                	if(0 == start_time)
                	{
                		start_time = get_tick_count();
                	}

                    app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Init_State_Fan);
                    app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_run);
                    app_pump_run_ref_heat();
                }

                if((get_elapsed_tick_count(start_time) > 15000) &&
                   (0 != start_time) &&
                   (DataBase_Get(sys_pump_level) > 0) &&
    			   (DataBase_Get(sys_expan_open) > 10) &&
    			   (DataBase_Get(sys_fan_level) > 0))
                {
                    app_compressor_run(DataBase_Get(sys_environment_temp),temp_now,compre_run);
                }

                if((0 != DataBase_Get(sys_fan_level)) && ((0 != DataBase_Get(sys_expan_open)) && (1 == expan_init)) && (0 != DataBase_Get(sys_motor_speed_now)) && (3 == DataBase_Get(sys_pump_level)))
                {
                	run_state = mode_run;
                	ref_state = ref_normal_run;
                    start_time = 0;
                }

                if(mode_stop == state_next)
                {
                	ref_end_delay_time(DataBase_Get(sys_motor_start_time),&motor_end_time);
                	run_state = mode_run_to_stop;
                	ref_state = ref_normal_run_stop;
                    start_time = 0;
                }
                else if(mode_power_off == state_next)
                {
                	ref_end_immed_time(&motor_end_time);
                	run_state = mode_run_to_stop;
                	ref_state = ref_normal_run_stop;
                    start_time = 0;
                }
                else if(mode_end == state_next)
                {
                    ref_end_immed_time(&motor_end_time);
                	run_state = mode_run_to_stop;
                	ref_state = ref_normal_run_stop;
                    start_time = 0;
                }

                g_tunit_run_state.unit_run_state = unit_run_wait_start;
        	}
        break;

        case mode_run:
        	if(ref_low_power_run == ref_state)
        	{
                app_fan_run_low_power(temp_now,Run_State_Fan);
                app_pump_run_ref_heat();

                if(DataBase_Get(sys_environment_temp) > DataBase_Get(sys_valve_start_temp))
                {
                	ref_state = ref_normal_init;
                	run_state = mode_init;
                	app_valve_set(valve_close);
                	ref_part_start_time(DataBase_Get(sys_motor_end_time),&part_start_time);
                    time = 0;
                }

                if(mode_stop == state_next)
                {
                	ref_state = ref_low_power_run_stop;
                	run_state = mode_run_to_stop;
                    time = 0;
                }
                else if(mode_power_off == state_next)
                {
                	ref_state = ref_low_power_run_stop;
                	run_state = mode_run_to_stop;
                }
                else if(mode_end == state_next)
                {
                    ref_state = ref_low_power_run_stop;
                	run_state = mode_run_to_stop;
                }

                if(1 == low_ref_err)
                {
                	ref_state = ref_low_power_err;
                	run_state = mode_err;
                    time = 0;
                }

                if(temp_err >= 30)
                {
                    if(0 == time)
                    {
                        time = get_tick_count();
                    }
                    else if(get_elapsed_tick_count(time) > 300000)
                    {
                    	ref_state = ref_normal_init;
                    	run_state = mode_init;
                        app_valve_set(valve_close);
                        ref_part_start_time(DataBase_Get(sys_motor_end_time),&part_start_time);
                        time = 0;
                    }
                }
                else
                {
                    time = 0;
                }
                g_tunit_run_state.unit_run_state = unit_run_run;
        	}
        	else if(ref_normal_run == ref_state)
        	{
        	    if(1 == ref_err)
        	    {
        	    	ref_end_immed_time(&motor_end_time);
					run_state = mode_err;
					ref_state = ref_normal_err;
        	    }

                if(mode_stop == state_next)
                {
                	ref_end_delay_time(DataBase_Get(sys_motor_start_time),&motor_end_time);
                	ref_state = ref_normal_run_stop;
                	run_state = mode_run_to_stop;
                }
                else if(mode_power_off == state_next)
                {
                	ref_end_immed_time(&motor_end_time);
                	ref_state = ref_normal_run_stop;
                	run_state = mode_run_to_stop;
                }
                else if(mode_end == state_next)
                {
                    ref_end_immed_time(&motor_end_time);
                	ref_state = ref_normal_run_stop;
                	run_state = mode_run_to_stop;
                }
                else
                {
                     app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Run_State_Fan);
                     app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_run);
                     app_pump_run_ref_heat();
                     app_compressor_run(DataBase_Get(sys_environment_temp), temp_now,compre_run);
                }
                g_tunit_run_state.unit_run_state = unit_run_run;
        	}
        break;

        case mode_run_to_stop:
        	if(ref_low_power_run_stop == ref_state)
        	{
                if(1 == low_ref_err)
                {
                	ref_state = ref_low_power_err;
                	run_state = mode_err;
                    time = 0;
                }

                app_fan_run_low_power(temp_now,Stop_Status_Fan);

                if(0 == DataBase_Get(sys_fan_level))
                {
                    if(0 == time)
                    {
                        time = get_tick_count();
                    }
                    else if(get_elapsed_tick_count(time) > 5000)
                    {
                        app_valve_set(valve_close);
                        run_state = mode_stop;
                        time = 0;
                    }
                }
                else
                {
                    time = 0;
                }
                g_tunit_run_state.unit_run_state = unit_run_wait_stop;
        	}
        	else if(ref_normal_run_stop == ref_state)
        	{
                if(1 == ref_err)
                {
                	ref_state = ref_normal_err;
                	run_state = mode_err;
                    time = 0;
                }

                if(get_tick_count() > motor_end_time)
                {
                    app_compressor_run(DataBase_Get(sys_environment_temp),temp_now,compre_stop);
                }

                if((0 == DataBase_Get(sys_motor_speed_now)) && (0 == end_time))
                {
                	end_time = get_tick_count();
                }

                if((get_elapsed_tick_count(end_time) > 10000) && (0 != end_time))
                {
                	 app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Stop_Status_Fan);
                	 app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_stop);
                	 app_pump_run_ref_heat();
                }

                if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
                {
                	run_state = mode_stop;
                    end_time = 0;
                }
                g_tunit_run_state.unit_run_state = unit_run_wait_stop;
        	}
        break;

        case mode_stop:
            if(mode_stop != state_next)
            {
            	run_state = mode_idle;
            }
        break;

        case mode_err:
        	if(ref_low_power_err == ref_state)
        	{
                if(0 == ref_err)
                {
                    if(0 == DataBase_Get(sys_fan_level))
                    {
                    	run_state = mode_idle;
                    }
                }

                if(mode_stop == state_next)
                {
                    if(0 == DataBase_Get(sys_fan_level))
                    {
                    	run_state = mode_stop;
                    }
                }

                app_fan_run_low_power(temp_now,Err_State_Fan);

                if(0 == DataBase_Get(sys_fan_level))
                {
                    if(0 == time)
                    {
                        time = get_tick_count();
                    }
                    else if(get_elapsed_tick_count(time) > 5000)
                    {
                        app_valve_set(valve_close);
                        time = 0;
                    }
                }
                else
                {
                    time = 0;
                }
        	}
        	else if(ref_normal_err == ref_state)
        	{
                if(0 == ref_err)
                {
                    if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
                    {
                    	run_state = mode_idle;
                    }
                }

                if(mode_stop == state_next)
                {
                    if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
                    {
                    	run_state = mode_stop;
                    }
                }

                if(get_tick_count() > motor_end_time)
                {
                    app_compressor_run(DataBase_Get(sys_environment_temp),temp_now,compre_err);
                }

                if((0 == DataBase_Get(sys_motor_speed_now)) && (0 == end_time))
                {
                	end_time = get_tick_count();
                }

                if((get_elapsed_tick_count(end_time) > 10000) && (0 != end_time))
                {
        			app_fan_run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_pres_trans_temp),Err_State_Fan);
        			app_expansion_run(DataBase_Get(sys_over_heat),DataBase_Get(sys_environment_temp),expan_err);
                    app_pump_run_ref_heat();
                }

                if((0 == DataBase_Get(sys_fan_level)) && (0 == DataBase_Get(sys_expan_open)) && (0 == DataBase_Get(sys_motor_speed_now)))
                {
                    end_time = 0;
                }
                g_tunit_run_state.unit_run_state = unit_run_fault;
        	}

        break;

        default:
        break;
    }
    return run_state;
}

mode_run_state_e ref_mode_run(mode_run_state_e state_next)
{
	if(SINGLE_WATER == ref_mode_set)
	{
		return ref_mode_run_single(state_next);
	}
	else if(DOUBLE_WATER == ref_mode_set)
	{
		return ref_mode_run_double(state_next);
	}
}

uint8_t ref_mode_exit(void)
{
	if(mode_stop == ref_mode_run(mode_power_off))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void app_ref_mode_set(u8 mode)
{
	ref_mode_set = mode;
}





