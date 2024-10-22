/*
 * app_expan_control.c
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */
#include "app_expan_control.h"
over_heat_data_s g_tover_heat_data;
void Expansion_IO_Init(void)
{
	GPIO_Config_T gpioConfig;

	RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA | RCM_APB2_PERIPH_GPIOB | RCM_APB2_PERIPH_AFIO);
	GPIO_ConfigPinRemap(GPIO_REMAP_SWJ_JTAGDISABLE);

	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.pin = GPIO_PIN_15;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(GPIOA,&gpioConfig);
	GPIO_ResetBit(GPIOA,GPIO_PIN_15);

	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.pin = GPIO_PIN_3 | GPIO_PIN_10 | GPIO_PIN_11;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(GPIOB,&gpioConfig);
	GPIO_ResetBit(GPIOB,GPIO_PIN_3 | GPIO_PIN_10 | GPIO_PIN_11);
}

uint8_t expan_init = 0;

void Expan_Open_Init(void)
{
	static Expan_Init_StateList state = Start_Open;
	switch(state)
	{
		case Start_Open:
			DataBase_Push(sys_expan_open,0);
			DataBase_Push(sys_expan_open_temp,240);
			state = Open_240;
		break;

		case Open_240:
			if(240 == DataBase_Get(sys_expan_open))
			{
				DataBase_Push(sys_expan_open,540);
				DataBase_Push(sys_expan_open_temp,0);
				state = Close_540;
			}
		break;

		case Close_540:
			if(0 == DataBase_Get(sys_expan_open))
			{
				state = End_Open;
			}
		break;

		case End_Open:
			expan_init = 1;
		break;

		default:
		break;
	}

}

void Power_Off(void)
{
	A_PHASE_OFF;
	B_PHASE_OFF;
	C_PHASE_OFF;
	D_PHASE_OFF;
}

uint32_t Set_Expansion_Beat(uint8_t state,Dir_List dir)
{
	Power_Off();

	if(Forward == dir)
	{
		if(7 == state)
		{
			state = 0;
		}
		else
		{
			state++;
		}
	}
	else if(Reverses == dir)
	{
		if(0 == state)
		{
			state = 7;
		}
		else
		{
			state--;
		}
	}

	switch (state)
	{
		case 0:
			A_PHASE_ON;
			B_PHASE_OFF;
			C_PHASE_OFF;
			D_PHASE_OFF;
		break;

		case 1:
			A_PHASE_ON;
			B_PHASE_ON;
			C_PHASE_OFF;
			D_PHASE_OFF;
		break;

		case 2:
			A_PHASE_OFF;
			B_PHASE_ON;
			C_PHASE_OFF;
			D_PHASE_OFF;
		break;

		case 3:
			A_PHASE_OFF;
			B_PHASE_ON;
			C_PHASE_ON;
			D_PHASE_OFF;
		break;

		case 4:
			A_PHASE_OFF;
			B_PHASE_OFF;
			C_PHASE_ON;
			D_PHASE_OFF;
		break;

		case 5:
			A_PHASE_OFF;
			B_PHASE_OFF;
			C_PHASE_ON;
			D_PHASE_ON;
		break;

		case 6:
			A_PHASE_OFF;
			B_PHASE_OFF;
			C_PHASE_OFF;
			D_PHASE_ON;
		break;

		case 7:
			A_PHASE_ON;
			B_PHASE_OFF;
			C_PHASE_OFF;
			D_PHASE_ON;
		break;

		default:
		break;
	}

	return get_tick_count();
}

void app_expansion_run(uint16_t over_heat,uint16_t envir_temp,Expan_Mach_StateList state)
{
	static uint32_t expan_change_timer = 0;
	static uint32_t timer = 0;
	static uint32_t init_timer = 0;
	static uint16_t open_temp = 0;
	static uint16_t over_heat_last = 0;
	static Expansion_StateList run_state = Init_State_Expan;
	uint16_t open_init;
	uint16_t point_over_heat;
	float open_change = 0;
	float over_heat_err = 0;
	float over_heat_rate = 0;
	float change_temp = 0;
	float expan_min_open_temp = 0;

	if(0 == expan_init)
	{
		return;
	}

	if(envir_temp < 540)
	{
		open_init = DataBase_Get(sys_expan_init_open_1);
		point_over_heat = DataBase_Get(sys_over_heat_point1);
	}
	else if((envir_temp >= 540) && (envir_temp < 700))
	{
		open_init = DataBase_Get(sys_expan_init_open_2);
		point_over_heat = DataBase_Get(sys_over_heat_point2);
	}
	else if((envir_temp >= 700) && (envir_temp < 780))
	{
		open_init = DataBase_Get(sys_expan_init_open_3);
		point_over_heat = DataBase_Get(sys_over_heat_point3);
	}
	else if((envir_temp >= 780) && (envir_temp < 860))
	{
		open_init = DataBase_Get(sys_expan_init_open_4);
		point_over_heat = DataBase_Get(sys_over_heat_point4);
	}
	else if(envir_temp >= 860)
	{
		open_init = DataBase_Get(sys_expan_init_open_5);
		point_over_heat = DataBase_Get(sys_over_heat_point5);
	}

	DataBase_Push(sys_over_heat_point,point_over_heat);
	open_temp = DataBase_Get(sys_expan_open_temp); 

	expan_min_open_temp = DataBase_Get(sys_expan_min_open);
	expan_min_open_temp /= 10;

	if((expan_change_timer > 30000) && (get_tick_count() > expan_change_timer - 30000))
	{
		over_heat_last = over_heat;
		DataBase_Push(sys_over_heat_last,over_heat_last);
		expan_change_timer = 0;
	}

	if(expan_run == state)
	{
		switch (run_state)
		{
			case Init_State_Expan:
					open_temp = open_init;
				if((get_elapsed_tick_count(init_timer) > DataBase_Get(sys_expan_init_hold_time) * 1000) && (0 != init_timer))
				{
					init_timer = 0;
					run_state = Run_State_Expan;
				}
			break;

			case Run_State_Expan:

				over_heat_err = temp_trans(over_heat) - temp_trans(point_over_heat);
				over_heat_rate = temp_trans(over_heat) - temp_trans(over_heat_last);

				g_tover_heat_data.over_heat_rate = over_heat_rate * 10;
				DataBase_Push(sys_over_heat_rate,g_tover_heat_data.over_heat_rate);
				change_temp = over_heat_err + over_heat_rate/2;
				open_change = round_user(change_temp * expan_min_open_temp);

				if(open_change > 0 && open_change <= 1)//不进行膨胀阀调节
				{
					return;
				}

				if(open_change >= 0)
				{
					if(open_change > 20)
					{
						open_change = 20;
					}
				}
				else
				{
					if(open_change < -20)
					{
						open_change = -20;
					}
				}

				if((1 == DataBase_Get(sys_motor_out_temp_low_ctl)) && (open_change > 0))
				{
					return;
				}
				else if((1 == DataBase_Get(sys_motor_out_temp_high_ctl)) && (open_change < 0))
				{
					return;
				}
				AssignMent(480,50,(uint32_t*)&open_temp,open_temp += open_change);
			break;

			default:
			break;
		}
	}
	else if(expan_stop == state || expan_err == state)
	{
		open_temp = 0;
	}

	if(((get_elapsed_tick_count(timer) < DataBase_Get(sys_expan_ctl_interval) * 1000) && (0 != timer)) &&
	   (expan_err != state) &&
	   (expan_stop != state))
//	   (power_off_mode != DataBase_Get(sys_unit_mode_next)))
	{
		return;
	}

	if(DataBase_Get(sys_expan_open_temp) == open_temp)
	{
		return;
	}

	if((0 == DataBase_Get(sys_expan_open_temp)) && (open_temp > 0))
	{
		expan_change_timer = get_tick_count() + DataBase_Get(sys_expan_init_hold_time) * 1000;
		init_timer = get_tick_count();
	}
	else
	{
		expan_change_timer = get_tick_count() + DataBase_Get(sys_expan_ctl_interval) * 1000;
	}

	if(0 == open_temp)
	{
		run_state = Init_State_Expan;
		expan_change_timer = 0;
		init_timer = 0;
	}

	DataBase_Push(sys_expan_open_temp,open_temp);
	timer = get_tick_count();

}

uint8_t Set_Expansion_Start(void)
{
	static uint32_t timer = 0;
	uint8_t beat_temp;

	beat_temp = DataBase_Get(sys_expan_open) % 8;
	Set_Expansion_Beat(beat_temp + 1,Reverses);
	if(0 == timer)
	{
		timer = get_tick_count();
	}
	else if(get_elapsed_tick_count(timer) > 600)
	{
		timer = 0;
		return EXPAN_READY;
	}
	return EXPAN_UNREADY;
}

uint8_t Set_Expansion_Stop(void)
{
	static uint32_t timer = 0;
	if(0 == timer)
	{
		timer = get_tick_count();
	}
	else if(get_elapsed_tick_count(timer) > 600)
	{
		Power_Off();
		timer = 0;
		return EXPAN_READY;
	}
	return EXPAN_UNREADY;
}

uint8_t Set_Expansion_Opne(uint16_t open)
{
	uint8_t beat_temp;

	beat_temp = DataBase_Get(sys_expan_open) % 8;

	if(DataBase_Get(sys_expan_open) > open)
	{
	    Set_Expansion_Beat(beat_temp,Reverses);
	    DataBase_Push(sys_expan_open,DataBase_Get(sys_expan_open) - 1);
	}
	else if(DataBase_Get(sys_expan_open) < open)
	{
		Set_Expansion_Beat(beat_temp,Forward);
		DataBase_Push(sys_expan_open,DataBase_Get(sys_expan_open) + 1);
	}
	else if(DataBase_Get(sys_expan_open) == open)
	{
		return EXPAN_READY;
	}
	return EXPAN_UNREADY;
}


uint8_t Set_Expansion_Sleep(uint16_t open)
{
	if(DataBase_Get(sys_expan_open) != open)
	{
		return EXPAN_READY;
	}
	return EXPAN_UNREADY;
}


void Expansion_State_Machine(uint16_t open)
{
	static Expan_State_List run_state = Sleep;

	switch (run_state)
	{
	case Sleep:
		if(EXPAN_READY == Set_Expansion_Sleep(open))
		{
			run_state = Start;
		}
	break;

	case Start:
		if(EXPAN_READY == Set_Expansion_Start())
		{
			run_state = Run;
		}
	break;

	case Run:
		if(EXPAN_READY == Set_Expansion_Opne(open))
		{
			run_state = Stop;
		}
	break;

	case Stop:
		if(EXPAN_READY == Set_Expansion_Stop())
		{
			run_state = Sleep;
		}
	break;

	default:
	break;
	}
}

void set_expan_write(void)
{
	bl24c02_writeonebyte(EXPAN_WRITE_FLAG,1);
}

void clr_expan_write(void)
{
	bl24c02_writeonebyte(EXPAN_WRITE_FLAG,0);
}

uint16_t expan_open_get(void)
{
	uint16_t temp = 0;
	temp = (uint16_t)(bl24c02_readonebyte(EXPAN_OPEN_H)) << 8 | bl24c02_readonebyte(EXPAN_OPEN_L);
	return temp;
}

void expan_open_set(uint16_t data)
{
	bl24c02_writeonebyte(EXPAN_OPEN_H,(uint8_t)(DataBase_Get(sys_expan_open) >> 8));
	bl24c02_writeonebyte(EXPAN_OPEN_L,(uint8_t)(DataBase_Get(sys_expan_open)));
	set_expan_write();
}

void app_expan_run_manual(uint16_t open_temp)
{
	if(open_temp == DataBase_Get(sys_expan_open_temp))
	{
		return;
	}
	DataBase_Push(sys_expan_open_temp,open_temp);
}

void app_expan_run_vacuum(uint8_t state)
{
	if(1 == state)
	{
		if(VACUUM_EXPAN == DataBase_Get(sys_expan_open_temp))
		{
			return;
		}
		DataBase_Push(sys_expan_open_temp,VACUUM_EXPAN);
	}
	else
	{
		if(0 == DataBase_Get(sys_expan_open_temp))
		{
			return;
		}
		DataBase_Push(sys_expan_open_temp,0);
	}

}


