/*
 * app_compre_control.c
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#include "app_compre_control.h"
#define LOG_HEAD LOG_ID_APP
temp_data_s g_ttemp_data;
int16_t cal_table[7][8] =
{
	{-8, -5, -3, -2, -1,  0,  1,  2},
	{-6, -4, -2, -1,  0,  0,  2,  3},
	{-4, -3, -2, -1,  0,  1,  2,  3},
	{-3, -2, -1,  0,  1,  2,  3,  4},
	{-2, -1,  0,  1,  2,  4,  6,  7},
	{ 0,  1,  2,  3,  4,  5,  7,  8},
	{ 1,  2,  3,  4,  5,  6,  8,  9}
};
start_in_statelist start_in = normal;
extern uint8_t ref_err;
void compressor_off_call_back(void)
{
	DataBase_Push(sys_motor_speed_temp,0);
	start_in = speed_change;
}

void compressor_start_call_back(void)
{
	app_fault_check_en_set(idx_in_motor_pres_low_check,1);
}

void get_index(float rate,float err,uint8_t *index_x,uint8_t *index_y)
{
	if(rate <= -1.5)
	{
		*index_x = 0;
	}
	else if((rate > -1.5) && (rate <= -1))
	{
		*index_x = 1;
	}
	else if((rate > -1) && (rate <= -0.5))
	{
		*index_x = 2;
	}
	else if((rate > -0.5) && (rate <= 0))
	{
		*index_x = 3;
	}
	else if((rate > 0) && (rate <= 0.5))
	{
		*index_x = 4;
	}
	else if((rate > 0.5) && (rate <= 1))
	{
		*index_x = 5;
	}
	else if((rate > 1) && (rate <= 1.5))
	{
		*index_x = 6;
	}
	else if(rate > 1.5) 
	{
		*index_x = 7;
	}

	if(err <= -1)
	{
		*index_y = 0;
	}
	else if((err > -1) && (err <= -0.5))
	{
		*index_y = 1;
	}
	else if((err > -0.5) && (err <= 0))
	{
		*index_y = 2;
	}
	else if((err > 0) && (err <= 0.5))
	{
		*index_y = 3;
	}
	else if((err > 0.5) && (err <= 1))
	{
		*index_y = 4;
	}
	else if((err > 1) && (err <= 2))
	{
		*index_y = 5;
	}
	else if(err > 2) 
	{
		*index_y = 6;
	}
}
uint8_t limit_seclet = 0;
uint16_t speed_max_init(uint16_t temp_envir)
{
	if(temp_envir < FC1_MAX)
	{
		limit_seclet = 1;
		return DataBase_Get(sys_motor_fre_limit1);
	}
	else if((temp_envir >= FC2_MIN) && (temp_envir < FC2_MAX))
	{
		limit_seclet = 2;
		return DataBase_Get(sys_motor_fre_limit2);
	}
	else if((temp_envir >= FC3_MIN) && (temp_envir < FC3_MAX))
	{
		limit_seclet = 3;
		return DataBase_Get(sys_motor_fre_limit3);
	}
	else if((temp_envir >= FC4_MIN) && (temp_envir < FC4_MAX))
	{
		limit_seclet = 4;
		return DataBase_Get(sys_motor_fre_limit4);
	}
	else if((temp_envir >= FC5_MIN) && (temp_envir < FC5_MAX))
	{
		limit_seclet = 5;
		return DataBase_Get(sys_motor_fre_limit5);
	}
	else if((temp_envir >= FC6_MIN))
	{
		limit_seclet = 6;
		return  DataBase_Get(sys_motor_fre_limit6);
	}	
	return 0;
}

void speed_max_cal(uint16_t temp_envir,uint16_t *pspeed_max)
{
	if(1 == limit_seclet)
	{
		if(temp_envir >= FC1_MAX + 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit2);
			limit_seclet = 2;
		}
		else
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit1);
		}
	}
	else if(2 == limit_seclet)
	{
		if(temp_envir < FC2_MIN - 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit1);
			limit_seclet = 1;
		}
		else if(temp_envir >= FC2_MAX + 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit3);
			limit_seclet = 3;
		}
		else
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit2);
		}
	}
	else if(3 == limit_seclet)
	{
		if(temp_envir < FC3_MIN - 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit2);
			limit_seclet = 2;
		}
		else if(temp_envir >= FC3_MAX + 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit4);
			limit_seclet = 4;
		}
		else
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit3);
		}
	}
	else if(4 == limit_seclet)
	{
		if(temp_envir < FC4_MIN - 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit3);
			limit_seclet = 3;
		}
		else if(temp_envir >= FC4_MAX + 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit5);
			limit_seclet = 5;
		}
		else
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit4);
		}
	}
	else if(5 == limit_seclet)
	{
		if(temp_envir < FC5_MIN - 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit4);
			limit_seclet = 4;
		}
		else if(temp_envir >= FC5_MAX + 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit6);
			limit_seclet = 6;
		}
		else
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit5);
		}
	}
	else if(6 == limit_seclet)
	{
		if(temp_envir < FC6_MIN - 10)
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit5);
			limit_seclet = 5;
		}
		else
		{
			*pspeed_max = DataBase_Get(sys_motor_fre_limit6);
		}
	}
}

void app_compressor_run(uint16_t temp_envir,uint16_t temp_now,Compre_Mach_StateList state_mach)
{
	static uint16_t speed_max = 0;
	uint16_t speed_min = 0;
	uint16_t temp_target = 0;
	static uint16_t speed_temp = 0;
	static uint32_t timer = 0;
	static uint32_t init_timer = 0;
	static Compressor_StateList state = Init_State_Compre;
	static uint32_t motor_change_timer = 0;
	static float temp_err_last = 0;
	float temp_err = 0;
	float temp_err_rate = 0;
	float init_fre = 0;
	uint8_t index_x = 0;
	uint8_t index_y = 0;

	if(0 == speed_max)
	{
		speed_max = speed_max_init(temp_envir);
	}
	else
	{
		speed_max_cal(temp_envir,&speed_max);
	}
		
	temp_target = DataBase_Get(sys_ref_temp);
	speed_min = DataBase_Get(sys_motor_fre_min);

	temp_err = temp_trans(temp_now) - temp_trans(temp_target);
	temp_err_rate = temp_err - temp_err_last;

	g_ttemp_data.temp_err_now = temp_err * 10;
	g_ttemp_data.temp_err_rate = temp_err_rate * 10;

	speed_temp = DataBase_Get(sys_motor_speed_temp);

	if((motor_change_timer > 30000) && (get_tick_count() > motor_change_timer - 30000))
	{
		temp_err_last = temp_err;
		g_ttemp_data.temp_err_last = temp_err_last * 10;
		motor_change_timer = 0;
	}

	if(compre_run == state_mach)
	{
		switch (state)
		{
			case Init_State_Compre:
				if(0 == DataBase_Get(sys_motor_speed_temp))
				{
					init_fre = ((10 + 30 * temp_err) * DataBase_Get(sys_motor_fre_max) / 127) + 0.9 * (temp_trans(temp_envir) - 35);

					if(init_fre < 0)
					{
						init_fre = 0;
					}

					speed_temp = round_user(init_fre);
					if(speed_temp < 30)
					{
						speed_temp = 30;
					}

					if(speed_temp > DataBase_Get(sys_compre_start_max))
					{
						speed_temp = DataBase_Get(sys_compre_start_max);
					}
					AssignMent(speed_max,speed_min,(uint32_t*)&speed_temp,speed_temp);
				}

				if((get_elapsed_tick_count(init_timer) > TIME_INIT_COMPRESS) && (0 != init_timer))
				{
					init_timer = 0;
					state = Run_State_Compre;
				}

				if(1 == DataBase_Get(sys_fre_red))
				{
					if(speed_temp >= 38)
					{
						speed_temp -= 8;
					}
					else
					{
						speed_temp = 30;
					}
				}
			break;

			case Run_State_Compre:
				if(0 == DataBase_Get(sys_fre_red))
				{
					if(0 == DataBase_Get(sys_fre_limit))
					{
						get_index(temp_err_rate,temp_err,&index_x,&index_y);
						AssignMent(speed_max,speed_min,(uint32_t*)&speed_temp,speed_temp + cal_table[index_y][index_x]);
					}
					else
					{
						get_index(temp_err_rate,temp_err,&index_x,&index_y);
						AssignMent(speed_temp,speed_min,(uint32_t*)&speed_temp,speed_temp + cal_table[index_y][index_x]);
					}
				}
				else if(1 == DataBase_Get(sys_fre_red))
				{
					if(speed_temp >= 38)
					{
						speed_temp -= 8;
					}
					else
					{
						speed_temp = 30;
					}
				}
			break;

			default:

			break;
		}
	}
	else if(compre_stop == state_mach)
	{
		init_timer = 0;
		if(DataBase_Get(sys_motor_speed_now) > 30)
		{
			speed_temp = 30;
		}
		else if(DataBase_Get(sys_motor_speed_now) <= 30)
		{
			speed_temp = 0;
		}
	}
	else if(compre_err == state_mach)
	{
		init_timer = 0;
		speed_temp = 0;
	}

	if((get_elapsed_tick_count(timer) < TIME_INTERVAL_COMPRESS) && (0 != timer) &&
	   (1 != DataBase_Get(sys_fre_red)) &&
	   (compre_run == state_mach))
	{
		return;
	}

	if(speed_temp == DataBase_Get(sys_motor_speed_temp))
	{
		return;
	}

	if(0 == speed_temp)
	{
		state = Init_State_Compre;
		motor_change_timer = 0;
	}
	else
	{
		motor_change_timer = get_tick_count() + TIME_INTERVAL_COMPRESS;
	}

	DataBase_Push(sys_motor_speed_temp,speed_temp);
	DataBase_Push(sys_compre_fre_point,speed_temp);
	WDBG(LOG_ID_SUB_APP_COMPRE,"compress speed change %d",speed_temp);
	timer = get_tick_count();
	start_in = speed_change;
	DataBase_Push(sys_fre_red,0);
	if(0 == speed_temp)
	{
		start_in = end;
		DataBase_Push(sys_motor_end_time,get_tick_count());
	}

	if((0 != DataBase_Get(sys_motor_speed_temp)) && (0 == DataBase_Get(sys_motor_speed_now)))
	{
		start_in = start;
		DataBase_Push(sys_motor_start_time,get_tick_count());
		DataBase_Push(sys_motor_run_cnt,DataBase_Get(sys_motor_run_cnt) + 1);
		if(0 == DataBase_Get(sys_fault_shield))
		{
			app_fault_check_en_set(idx_in_motor_pres_low_check,0);
			sys_timer_start(SYS_TIMER_ID_COMPRE_START,SYS_TIMR_ID_ONE,120000,compressor_start_call_back);
		}
		init_timer = get_tick_count();
		motor_change_timer = get_tick_count() + TIME_INIT_COMPRESS;
	}
}


void app_compress_run_manual(uint16_t speed_temp)
{
	static uint32_t start_timer = 0;
	static uint32_t end_timer = 0;
	static start_in_statelist start_in_temp = 0;
	static uint32_t timer = 0;

	if((0 != speed_temp) && (0 == DataBase_Get(sys_motor_speed_temp)))
	{
		start_in_temp = start;
	}
	else if((0 == speed_temp) && (0 != DataBase_Get(sys_motor_speed_temp)))
	{
		start_in_temp = end;
	}
	else if(speed_temp != DataBase_Get(sys_motor_speed_temp))
	{
		start_in_temp = speed_change;
	}
	else
	{
		start_in_temp = normal;
	}

	if(1 == ref_err)
	{
		speed_temp = 0;
		start_in_temp = end;
	}

	if(DataBase_Get(sys_motor_speed_temp) == DataBase_Get(sys_motor_speed_now))
	{
		start_in = start_in_temp;
		DataBase_Push(sys_motor_speed_temp,speed_temp);
		WDBG(LOG_ID_SUB_APP_COMPRE,"compress speed change %d",speed_temp);
		if((start == start_in) && (0 == DataBase_Get(sys_motor_speed_now)) && (0 != DataBase_Get(sys_motor_speed_temp)))
		{
			DataBase_Push(sys_motor_run_cnt,DataBase_Get(sys_motor_run_cnt) + 1);
		}
	}
}

void app_motor_time_cnt_clean(void)
{
	DataBase_Push(sys_motor_run_time_temp,0);
	DataBase_Push(sys_motor_run_time_totaol,0);
	DataBase_Push(sys_motor_run_cnt,0);
}

void app_motor_run_time(void)
{
	if(DataBase_Get(sys_motor_speed_now) > 0)
    {
		DataBase_Push(sys_motor_run_time_temp,DataBase_Get(sys_motor_run_time_temp) + 1);
		if(0 == DataBase_Get(sys_motor_run_time_temp) % 3600)
		{
			DataBase_Push(sys_motor_run_time_totaol,DataBase_Get(sys_motor_run_time_totaol) + 1);
			DataBase_Push(sys_motor_run_time_temp,0);
		}
        DataBase_Push(sys_motor_run_time,DataBase_Get(sys_motor_run_time) + 1);
    }
    else
    {
        DataBase_Push(sys_motor_run_time,0);
    }
}


