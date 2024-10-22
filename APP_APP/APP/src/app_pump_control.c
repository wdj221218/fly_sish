/*
 * app_pump_control.c
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */
#include"app_pump_control.h"
#define LOG_HEAD LOG_ID_APP
extern  fault_in_value_s g_tfault_in_value[fault_diag_max];
extern uint8_t pump_err;
extern unit_run_state_s g_tunit_run_state;
void pump_off_callback(void)
{
	bsp_set_gpio_unact(pump_pwr_ctl);
}

void set_pump_pwm(uint8_t duty)
{
	if(duty > 100)
	{
		duty = 100;
	}

    bsp_set_pump_duty(duty);
}

uint8_t app_get_pump_duty(void)
{
    switch (DataBase_Get(sys_pump_level))
	{
		case 0:
			return 0;
		break;

		case 1:
			return DataBase_Get(sys_pump_low_duty);
		break;

		case 2:
			return DataBase_Get(sys_pump_middle_duty);
		break;

		case 3:
			return DataBase_Get(sys_pump_high_duty);
		break;

		default:
			return 0;
		break;
	}
}

void app_set_pump_level(uint8_t level)
{
	DataBase_Push(sys_pump_duty,app_get_pump_duty());
    switch (level)
    {
		case 0:
			set_pump_pwm(5);
			break;

		case 1:

			set_pump_pwm(DataBase_Get(sys_pump_low_duty));
			break;

		case 2:
			set_pump_pwm(DataBase_Get(sys_pump_middle_duty));
			break;

		case 3:
			set_pump_pwm(DataBase_Get(sys_pump_high_duty));
			break;

		default:
		break;
    }
    WDBG(LOG_ID_SUB_APP_PUMP,"pump level set %d",level);
	if(0 == level)
	{
		if(SYS_TIMER_RUN == sys_timer_get_st(SYS_TIMER_ID_PUMP_OFF))
		{
			return;
		}

		if(0 == bsp_get_output(pump_pwr_ctl))
		{
			return;
		}

		sys_timer_start(SYS_TIMER_ID_PUMP_OFF,SYS_TIMR_ID_ONE,5000,pump_off_callback);;
	}
	else
	{
		bsp_set_gpio_act(pump_pwr_ctl);
	}
}

uint8_t GetPump_Run(void)
{
	return DataBase_Get(sys_pump_level);
}

void app_pump_run_cir(uint16_t bms_min,uint16_t bms_max)
{
    static uint32_t timer = 0;
    static uint8_t level = 0;
    uint16_t temp_dif;
    uint16_t temp_ref;
    uint16_t temp_heat;

    temp_dif = bms_max - bms_min;

	if(in_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_ref = DataBase_Get(sys_in_water_ctl_ref_temp);
		temp_heat = DataBase_Get(sys_in_water_ctl_heat_temp);
	}
	else if(out_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_ref = DataBase_Get(sys_out_water_ctl_ref_temp);
		temp_heat = DataBase_Get(sys_out_water_ctl_heat_temp);
	}
	else if(bms_avr_temp == DataBase_Get(sys_ctl_temp_set))
	{
		temp_ref = DataBase_Get(sys_bms_temp_ctl_ref_temp);
		temp_heat = DataBase_Get(sys_bms_temp_ctl_heat_temp);
	}

    if(temp_dif >= DataBase_Get(sys_self_cir_start_dev))
    {
        level = 1;
    }

    if(bms_max >= temp_ref || bms_min <= temp_heat)
    {
        level = 2;
    }

    if((temp_dif <= sys_self_cir_stop_dev) && (bms_max < temp_ref ) && (bms_min > temp_heat))
    {
        level = 0;
        g_tunit_run_state.unit_run_state = unit_run_idle;
    }

    if(1 == g_tfault_in_value[communication_bms].value)
    {
        level = 1;
    }

    if(1 == pump_err)
    {
    	level = 0;
    	g_tunit_run_state.unit_run_state = unit_run_fault;
    }
    else
    {
    	g_tunit_run_state.unit_run_state = unit_run_run;
    }

    if((get_elapsed_tick_count(timer) < TIME_INTERVAL_PUMP) && (0 != timer) && (0 == pump_err))
    {
        return;
    }

    if(DataBase_Get(sys_pump_level) == level)
    {
        return;
    }

    DataBase_Push(sys_pump_level,level);
    app_set_pump_level(level);
    timer = get_tick_count();
}

void app_pump_run_ref_heat(void)
{

    if(1 == pump_err)
    {
        DataBase_Push(sys_pump_level,0);
        app_set_pump_level(0);
        return;
    }

    if(3 == DataBase_Get(sys_pump_level))
    {
        return;
    }

    DataBase_Push(sys_pump_level,3);
    app_set_pump_level(3);
}

void app_pump_run_manual(uint8_t level)
{
    if(1 == pump_err)
    {
        DataBase_Push(sys_pump_level,0);
        app_set_pump_level(0);
        return;
    }

/*	if(level == DataBase_Get(sys_pump_level))
	{
		app_set_pump_level(level);
		return;
	}*/
	DataBase_Push(sys_pump_level,level);
    app_set_pump_level(level);
}

void app_pump_run_auto_init(void)
{
    if(1 == pump_err)
    {
        DataBase_Push(sys_pump_level,0);
        app_set_pump_level(0);
        return;
    }

    if(3 == DataBase_Get(sys_pump_level))
    {
        return;
    }

    DataBase_Push(sys_pump_level,3);
    app_set_pump_level(3);    
}

void app_pump_off(void)
{
    if(DataBase_Get(sys_pump_level) == 0)
    {
        return;
    }

    DataBase_Push(sys_pump_level,0);
    app_set_pump_level(0);	
}
