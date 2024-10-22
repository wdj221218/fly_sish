/*
 * app_ptc_control.c
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#include "app_ptc_control.h"
#define LOG_HEAD LOG_ID_APP
extern fault_in_value_s g_tfault_in_value[fault_diag_max];
uint8_t app_get_ptc(void)
{
    if(1 == bsp_get_output(ptc1_ctl))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

uint8_t app_get_belt(void)
{
    if(1 == bsp_get_output(ptc2_ctl))
    {
        return 1;
    }
    else
    {
        return 0;
    }    
}

void app_ptc_control(PTC_Status_List status)
{
    static uint32_t timer = 0;
    switch (status)
    {
        case PTC_ON:
            if(0 != app_get_ptc())
            {
                return;
            }
            bsp_set_gpio_act(ptc1_ctl);
            DataBase_Push(sys_ptc_state,1);
            DataBase_Push(sys_ptc_run_cnt,DataBase_Get(sys_ptc_run_cnt) + 1);
            timer = get_tick_count();
            WDBG(LOG_ID_SUB_APP_PTC,"PTC ON");
        break;

        case PTC_OFF:
            if(1 != app_get_ptc())
            {
                return;
            }
            bsp_set_gpio_unact(ptc1_ctl);
            DataBase_Push(sys_ptc_state,0);
            timer = get_tick_count();
            WDBG(LOG_ID_SUB_APP_PTC,"PTC OFF");
        break;

        case PTC_ERR:
            bsp_set_gpio_unact(ptc1_ctl);
            DataBase_Push(sys_ptc_state,0);
            timer = get_tick_count();
            WDBG(LOG_ID_SUB_APP_PTC,"PTC ERR");
        break;

        default:
        break;
    }
}

void belt_control(PTC_Status_List status)
{
    switch (status)
    {
        case PTC_ON:
            if(0 != app_get_belt())
            {
                return;
            }
            bsp_set_gpio_act(ptc2_ctl);
            DataBase_Push(sys_heat_belt_state,1);
            WDBG(LOG_ID_SUB_APP_PTC,"BELT ON");
        break;

        case PTC_OFF:
            if(1 != app_get_belt())
            {
                return;
            }
            bsp_set_gpio_unact(ptc2_ctl);
            DataBase_Push(sys_heat_belt_state,0);
            WDBG(LOG_ID_SUB_APP_PTC,"BELT OFF");
        break;

        default:
        break;
    }
}

void app_belt_control(void)
{
	if(power_off_mode == DataBase_Get(sys_unit_mode))
	{
		belt_control(PTC_OFF);
	}

	if((manual_mode != DataBase_Get(sys_unit_mode)) && (power_off_mode != DataBase_Get(sys_unit_mode)))
	{
		if(1 == g_tfault_in_value[environment_temp_sensor].value)
		{
	        if(DataBase_Get(sys_motor_speed_now) > 0)
	        {
	            belt_control(PTC_OFF);
	        }
	        else
	        {
	            belt_control(PTC_ON);
	        }
		}
		else
		{
	        if((DataBase_Get(sys_motor_run_time) < 180) && DataBase_Get(sys_environment_temp) <= 500)
	        {
	            belt_control(PTC_ON);
	        }

	        if((DataBase_Get(sys_environment_temp) >= 520) || (DataBase_Get(sys_motor_run_time) >= 180))
	        {
	            belt_control(PTC_OFF);
	        }
		}
	}
}

void app_ptc_control_manual(uint8_t status)
{
	if(1 == status)
	{
        if(0 != app_get_ptc())
        {
            return;
        }
		bsp_set_gpio_act(ptc1_ctl);
		DataBase_Push(sys_ptc_run_cnt,DataBase_Get(sys_ptc_run_cnt) + 1);
        DataBase_Push(sys_ptc_state,1);
	}
	else if(0 == status)
	{
		bsp_set_gpio_unact(ptc1_ctl);
        DataBase_Push(sys_ptc_state,0);
	}

}

void app_belt_control_manual(uint8_t status)
{
	if(1 == status)
	{
		bsp_set_gpio_act(ptc2_ctl);
        DataBase_Push(sys_heat_belt_state,1);
	}
	else if(0 == status)
	{
		bsp_set_gpio_unact(ptc2_ctl);
        DataBase_Push(sys_heat_belt_state,0);
	}

}

void app_ptc_time_cnt_clean(void)
{
	DataBase_Push(sys_ptc_run_time_temp,0);
	DataBase_Push(sys_ptc_run_time_total,0);
	DataBase_Push(sys_ptc_run_cnt,0);
}

void app_ptc_run_time(void)
{
    if(1 == DataBase_Get(sys_ptc_state))
    {
    	DataBase_Push(sys_ptc_run_time_temp,DataBase_Get(sys_ptc_run_time_temp) + 1);

    	if(0 == DataBase_Get(sys_ptc_run_time_temp) % 3600)
    	{
    		DataBase_Push(sys_ptc_run_time_total,DataBase_Get(sys_ptc_run_time_total) + 1);
    		DataBase_Push(sys_ptc_run_time_temp,0);
    	}

        DataBase_Push(sys_ptc_run_time,DataBase_Get(sys_ptc_run_time) + 1);
    }
    else
    {
        DataBase_Push(sys_ptc_run_time,0);
    }
}

