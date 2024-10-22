/*
 * app_temp_hold.c
 *
 *  Created on: 2024年3月25日
 *      Author: Dell
 */
#include "app_temp_hold.h"
#if 0

uint8_t Hold_Temp_Init(void)
{
    Compressor_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_in_water_temp),Stop_Status_Compre);
    if(0 == DataBase_Get(sys_motor_speed_now))
    {
        Fan_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_conden_temp),Stop_Status_Fan);
        Expansion_Run(DataBase_Get(sys_over_heat),Stop_Status_Expan);
    }
    PTC_Control(PTC_OFF);
    Pump_Run_HoldTemp(get_abs_dec(DataBase_Get(sys_in_water_temp),DataBase_Get(sys_out_water_temp)));

    if((0 == DataBase_Get(sys_motor_speed_now)) &&
       (0 == DataBase_Get(sys_fan_level))       &&
       (0 == DataBase_Get(sys_expan_open))      &&
       (0 == DataBase_Get(sys_ptc_state)))
    {
        return HOLD_READY;
    }
    else
    {
        return HOLD_UNREADY;
    }
}

uint8_t Hold_Temp_Heat(uint16_t target_temp,uint16_t now_temp)
{
    if(now_temp > target_temp)
    {
    	Pump_Run_HoldTemp(get_abs_dec(DataBase_Get(sys_in_water_temp),DataBase_Get(sys_out_water_temp)));
        PTC_Control(PTC_OFF);
        if(0 == DataBase_Get(sys_ptc_state))
        {
        	return HOLD_READY;
        }
        else
        {
        	return HOLD_UNREADY;
        }
    }
    else
    {
    	Pump_Run_HoldTemp(get_abs_dec(DataBase_Get(sys_in_water_temp),DataBase_Get(sys_out_water_temp)));
    	PTC_Control(PTC_ON);
        return HOLD_UNREADY;
    }
}

uint8_t Hold_Temp_Ref(uint16_t target_temp,uint16_t now_temp)
{
    static uint32_t timer = 0;
    if(now_temp < target_temp)
    {
    	Pump_Run_HoldTemp(get_abs_dec(DataBase_Get(sys_in_water_temp),DataBase_Get(sys_out_water_temp)));
        if(0 == timer)
        {
            timer = get_tick_count();
        }

        Compressor_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_out_water_temp),Stop_Status_Compre);
        if(get_elapsed_tick_count(timer) > 3000 && 0 == DataBase_Get(sys_motor_speed_now))
        {
            Fan_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_conden_temp),Stop_Status_Fan);
            Expansion_Run(DataBase_Get(sys_over_heat),Stop_Status_Expan);
        }

        if((0 == DataBase_Get(sys_expan_open)) &&
           (0 == DataBase_Get(sys_fan_level)) &&
           (0 == DataBase_Get(sys_motor_speed_now)))
        {
            timer = 0;
            return HOLD_READY;
        }
        else
        {
        	return HOLD_UNREADY;
        }
    }
    else
    {
    	Pump_Run_HoldTemp(get_abs_dec(DataBase_Get(sys_in_water_temp),DataBase_Get(sys_out_water_temp)));
        if((0 == DataBase_Get(sys_motor_speed_now)) &&
           (0 == Get_PTC()) &&
		   (DataBase_Get(sys_fan_level) > 0) &&
		   (DataBase_Get(sys_expan_open) >= DataBase_Get(sys_expan_open_init)))
        {
            Compressor_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_in_water_temp),Init_State_Compre);
        }
        else if((0 == Get_PTC()) && (0 != DataBase_Get(sys_motor_speed_now)))
        {
            Compressor_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_in_water_temp),Run_State_Compre);
        }

        if(0 == DataBase_Get(sys_fan_level))
        {
            Fan_Run(DataBase_Get(sys_environment_temp),1000,Init_State_Fan);
        }
        else
        {
            Fan_Run(DataBase_Get(sys_environment_temp),1000,Run_State_Fan);
        }

        if(0 == Expansion_Open)
        {
            Expansion_Run(60,Init_State_Expan);
        }
        else
        {
            Expansion_Run(60,Run_State_Expan);
        }
        timer = 0;
        return HOLD_UNREADY;
    }
}

uint8_t Hold_Temp_Stop(void)
{
    Compressor_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_in_water_temp),Stop_Status_Compre);
    if(0 == DataBase_Get(sys_motor_speed_now))
    {
        Fan_Run(DataBase_Get(sys_environment_temp),DataBase_Get(sys_conden_temp),Stop_Status_Fan);
        Expansion_Run(DataBase_Get(sys_over_heat),Stop_Status_Expan);
    }

    PTC_Control(PTC_OFF);
    Pump_Run_HoldTemp(get_abs_dec(DataBase_Get(sys_in_water_temp),DataBase_Get(sys_out_water_temp)));

	return HOLD_READY;
}

void Hold_Temp_State_Machine(uint16_t target_temp,uint16_t now_temp)
{
    static Hold_Temp_List state = HoldTemp_Init;
    if(0xAA == DataBase_Get(sys_temp_hold_set))
    {
        switch (state)
        {
            case HoldTemp_Init:
                if(HOLD_READY == Hold_Temp_Init())
                {
                    if(now_temp < target_temp - DataBase_Get(sys_hold_heat_diff))
                    {
                        state = HoldTemp_Heat;
                        DataBase_Push(sys_run_mode,Heat_Mode);
                    }
                    else if(now_temp > target_temp + DataBase_Get(sys_hold_ref_diff))
                    {
                        state = HoldTemp_Ref;
                        DataBase_Push(sys_run_mode,Refrigerate_Mode);
                    }
                }
            break;

            case HoldTemp_Heat:
                if(HOLD_READY == Hold_Temp_Heat(target_temp,now_temp))
                {
                    state = HoldTemp_Init;
                    DataBase_Push(sys_run_mode,WaterCycle_Mode);
                }
            break;

            case HoldTemp_Ref:
                if(HOLD_READY == Hold_Temp_Ref(target_temp,now_temp))
                {
                    state = HoldTemp_Init;
                    DataBase_Push(sys_run_mode,WaterCycle_Mode);
                }
            break;

            default:

            break;
        }
    }
    else if(0x55 == DataBase_Get(sys_temp_hold_set))
    {

        Hold_Temp_Stop();
    }
}

#endif
