/*
 * app_fan_control.c
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#include "app_fan_control.h"
fan_speeds g_tfan1_speed;
fan_speeds g_tfan3_speed;
#define LOG_HEAD LOG_ID_APP
void fan_off_callback(void)
{
	bsp_set_gpio_unact(fan_pwr_ctl);
}

void app_set_fan_pwm(uint8_t duty)
{
	bsp_set_fan_duty(duty);
}

uint8_t app_get_fan_duty(void)
{
	switch (DataBase_Get(sys_fan_level))
	{
		case 0:
			return 0;
		break;

		case 1:
			return DataBase_Get(sys_fan_low_duty);
		break;

		case 2:
			return DataBase_Get(sys_fan_middle_duty);
		break;

		case 3:
			return DataBase_Get(sys_fan_high_duty);
		break;

		default:
			return 0;
		break;
	}
}

void app_fan_fb_init(void)
{
    GPIO_Config_T 		GPIO_ConfigStruct;

    RCM_EnableAPB2PeriphClock(FAN1_FB_RCM);
	GPIO_ConfigStructInit(&GPIO_ConfigStruct);
	GPIO_ConfigStruct.pin = FAN1_FB_PIN;
	GPIO_ConfigStruct.mode = GPIO_MODE_IN_FLOATING;
	GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
	GPIO_Config(FAN1_FB_PORT, &GPIO_ConfigStruct);

    RCM_EnableAPB2PeriphClock(FAN2_FB_RCM);
	GPIO_ConfigStructInit(&GPIO_ConfigStruct);
	GPIO_ConfigStruct.pin = FAN2_FB_PIN;
	GPIO_ConfigStruct.mode = GPIO_MODE_IN_FLOATING;
	GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
	GPIO_Config(FAN2_FB_PORT, &GPIO_ConfigStruct);

    RCM_EnableAPB2PeriphClock(FAN3_FB_RCM);
	GPIO_ConfigStructInit(&GPIO_ConfigStruct);
	GPIO_ConfigStruct.pin = FAN3_FB_PIN;
	GPIO_ConfigStruct.mode = GPIO_MODE_IN_FLOATING;
	GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
	GPIO_Config(FAN3_FB_PORT, &GPIO_ConfigStruct);
}

void app_get_fan_fb(void)
{
	g_tfan1_speed.io_now = (FAN1_FB_PORT->IDATA & FAN1_FB_PIN) ?  BIT_SET : BIT_RESET;
	if((1 == g_tfan1_speed.io_now) && (0 == g_tfan1_speed.io_last))
	{
		g_tfan1_speed.plus_num++;
	}
	g_tfan1_speed.io_last = g_tfan1_speed.io_now;

	g_tfan3_speed.io_now = (FAN3_FB_PORT->IDATA & FAN3_FB_PIN) ?  BIT_SET : BIT_RESET;
	if((1 == g_tfan3_speed.io_now) && (0 == g_tfan3_speed.io_last))
	{
		g_tfan3_speed.plus_num++;
	}
	g_tfan3_speed.io_last = g_tfan3_speed.io_now;

}

void app_get_fan_speed(void)
{
	g_tfan1_speed.rpm = g_tfan1_speed.plus_num * 60;
	g_tfan1_speed.plus_num = 0;
	DataBase_Push(sys_fan1_fb,g_tfan1_speed.rpm);

	g_tfan3_speed.rpm = g_tfan3_speed.plus_num * 60;
	g_tfan3_speed.plus_num = 0;
	DataBase_Push(sys_fan3_fb,g_tfan3_speed.rpm);
}

void app_set_fan_level(uint8_t level)
{
	DataBase_Push(sys_fan_duty,app_get_fan_duty());
	uint8_t temp;
	switch(level)
	{
		case 0:
			app_set_fan_pwm(0);
		break;

		case 1:
			app_set_fan_pwm(DataBase_Get(sys_fan_low_duty));
		break;

		case 2:
			app_set_fan_pwm(DataBase_Get(sys_fan_middle_duty));
		break;

		case 3:
			app_set_fan_pwm(DataBase_Get(sys_fan_high_duty));
		break;

		default:
		break;
	}
	WDBG(LOG_ID_SUB_APP_FAN,"fan level set %d",level);
	if(0 == level )
	{
		if(SYS_TIMER_RUN == sys_timer_get_st(SYS_TIMER_ID_FAN_OFF))
		{
			return;
		}

		if(0 == bsp_get_output(fan_pwr_ctl))
		{
			return;
		}

		sys_timer_start(SYS_TIMER_ID_FAN_OFF,SYS_TIMR_ID_ONE,5000,fan_off_callback);
	}
	else
	{
		bsp_set_gpio_act(fan_pwr_ctl);
	}
}

void app_fan_run(uint16_t temp_envir,uint16_t out_pres_trans_temp,Fan_StateList state)
{
	static uint8_t level = 0;
	static uint32_t timer = 0;
	static uint32_t timer1 = 0;
	static uint32_t timer2 = 0;
	static uint32_t timer3 = 0;
	level = DataBase_Get(sys_fan_level);
	switch (state)
	{
		case Init_State_Fan:
			if(temp_envir >= 820)
			{
				level = 3;
			}
			else if(temp_envir >= 700 && temp_envir < 820)
			{
				level = 2;
			}
			else if(temp_envir < 700)
			{
				level = 1;
			}
		break;

		case Run_State_Fan:
			if(out_pres_trans_temp > DataBase_Get(sys_fan_inc_temp))
			{
				if(0 == timer1)
				{
					timer1 = get_tick_count();
				}

				if(timer1 > 3000)
				{
					AssignMent(3,1,(uint32_t*)(&level),level + 1);
					timer1 = 0;
				}
				timer2 = 0;
				timer3 = 0;
			}
			else if(out_pres_trans_temp > DataBase_Get(sys_fan_dec_temp) && out_pres_trans_temp <= DataBase_Get(sys_fan_inc_temp))
			{
				if(0 == timer2)
				{
					timer2 = get_tick_count();
				}

				if(timer2 > 3000)
				{
					AssignMent(3,1,(uint32_t*)(&level),level);
					timer2 = 0;
				}
				timer1 = 0;
				timer3 = 0;				
			}
			else if(out_pres_trans_temp <= DataBase_Get(sys_fan_dec_temp))
			{
				if(0 == timer3)
				{
					timer3 = get_tick_count();
				}

				if(timer3 > 3000)
				{
					AssignMent(3,1,(uint32_t*)(&level),level - 1);
					timer3 = 0;
				}
				timer1 = 0;
				timer2 = 0;	
				
			}
		break;

		case Stop_Status_Fan:
			level = 0;
			timer1 = 0;
			timer2 = 0;
			timer3 = 0;
		break;

		case Err_State_Fan:
			level = 0;
			timer1 = 0;
			timer2 = 0;
			timer3 = 0;
		break;

		default:
		break;
	}

	if(((get_elapsed_tick_count(timer) < TIME_INTERVAL_FAN) && (0 != timer)) &&
	   (Err_State_Fan != state) &&
	   (Stop_Status_Fan != state))
//	   (power_off_mode != DataBase_Get(sys_unit_mode_next)))
	{
		return;
	}

	if(DataBase_Get(sys_fan_level) == level)
	{
		app_set_fan_level(level);
		return;
	}

	DataBase_Push(sys_fan_level,level);
	app_set_fan_level(level);
	timer = get_tick_count();
}

void app_fan_run_manual(uint8_t level)
{

	DataBase_Push(sys_fan_level,level);
	app_set_fan_level(level);
}

void app_fan_run_low_power(uint16_t temp_now,Fan_StateList state)
{
	uint8_t level = 0;
	uint16_t temp_target ;
	static uint32_t timer = 0;
	int temp_err = 0;
	temp_target = DataBase_Get(sys_ref_temp);
	level = DataBase_Get(sys_fan_level);
	temp_err = temp_now - temp_target;

	switch (state)
	{
		case Init_State_Fan:
			level = 3;
		break;

		case Run_State_Fan:
			if(temp_err > (int16_t)(DataBase_Get(sys_ref_backlash)))
			{
				level = 3;
			}
			else if(temp_err < -(int16_t)(DataBase_Get(sys_ref_backlash)))
			{
				level = 0;
			}
			else
			{
				level = 2;
			}

		break;

		case Stop_Status_Fan:
			level = 0;
		break;

		case Err_State_Fan:
			level = 0;
		break;

		default:
		break;
	}

	if((get_elapsed_tick_count(timer) < TIME_INTERVAL_FAN) && (0 != timer) && (Run_State_Fan == state))
	{
		return;
	}

	if(DataBase_Get(sys_fan_level) == level)
	{
		return;
	}

	DataBase_Push(sys_fan_level,level);
	app_set_fan_level(level);
	timer = get_tick_count();
}
