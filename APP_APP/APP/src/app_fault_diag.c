/*
 * app_fault_diag.c
 *
 *  Created on: 2024年3月22日
 *      Author: Dell
 */

#include "app_fault_diag.h"

fault_out_s g_tfault_out;
fault_sum_s g_tfault_sum;
fault_in_value_s g_tfault_in_value[fault_diag_max] = {0};
single_info_s g_tfault_single[fault_diag_max] =
{
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//power_control_board
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//power_expan
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//adcref_control_board
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//communication_motor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//in_watar_temp_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_water_temp_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_motor_temp_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//in_motor_temp_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//environment_temp_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//in_water_pres_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_water_pres_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_motor_pres_sensor
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//in_motor_pres_sensor
	{{0,3000,3000,3000,   	0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_motor_pres_high
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//in_motor_pres_low
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_motor_temp_high
	{{0,3000,3000,0,   	    0},SINGLE_ST1,SINGLE_ST1,2,0,0},//in_water_pres_low
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_water_pres_high
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_water_temp_low
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//out_water_temp_high
	{{0,20000,20000,0,   	0},SINGLE_ST1,SINGLE_ST1,2,0,0},//fan1
	{{0,20000,20000,0,   	0},SINGLE_ST1,SINGLE_ST1,2,0,0},//fan2
	{{0,20000,20000,0,   	0},SINGLE_ST1,SINGLE_ST1,2,0,0},//fan3
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//compre_drive
	{{0,5000,5000,0,   	    0},SINGLE_ST1,SINGLE_ST1,2,0,0},//pump
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//ptc
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//communication_bms
	{{0,3000,3000,0,   		0},SINGLE_ST1,SINGLE_ST1,2,0,0},//envit_temp_over
};

single_info_s low_pres_single = {{0,3000,0,0,0},SINGLE_IDLE,SINGLE_IDLE,2,0,0};

fault_check_s g_tfault_in_check[faule_check_list_max] = 
{
	//处理函数                         
	{power_control_board_check_in,		100,	    100,		0,		1,		1},
	{power_expan_check_in,				100,		100,		0,		1,		1},
	{adc_ref_check_in,					100,		100,		0,		1,		1},
	{temp_sensor_check_in,				100,		100,		0,		1,		1},
	{pres_sensor_check_in,				100,		100,		0,		1,		1},
	{in_motor_pres_low_check_in,		100,		100,		0,		0,		0},
	{out_motor_pres_high_check_in,		100,		100,		0,		0,		0},
	{out_motor_temp_high_check_in,		100,		100,		0,		0,		0},
	{fan_check_in,						100,		100,		0,		1,		1},
	{compre_fre_check_in,				100,		100,		0,		1,		1},
	{in_water_pres_low_check_in,		100,		100,		0,		1,		1},//暂时
	{out_water_pres_high_check_in,		100,		100,		0,		1,		1},
	{out_water_temp_low_check_in,		100,		100,		0,		0,		0},
	{out_water_temp_high_check_in,		100,		100,		0,		0,		0},
	{pump_check_in,						100,		100,		0,		1,		1},
	{ptc_check_in,						100,		100,		0,		1,		1},
	{motor_community_check_in,			100,		100,		0,		1,		1},
	{host_community_check_in,			100,		100,		0,		1,		1},
	{envit_temp_over_check_in,			100,		100,		0,		1,		1},
};

fault_check_s g_tfault_out_check[faule_check_list_max] = 
{
	{power_control_board_check_out,		100,		100,		0,		1,		1},
	{power_expan_check_out,				100,		100,		0,		1,		1},
	{adc_ref_check_out,					100,		100,		0,		1,		1},
	{temp_sensor_check_out,				100,		100,		0,		1,		1},
	{pres_sensor_check_out,				100,		100,		0,		1,		1},
	{in_motor_pres_low_check_out,		100,		100,		0,		1,		1},
	{out_motor_pres_high_check_out,		100,		100,		0,		1,		1},
	{out_motor_temp_high_check_out,		100,		100,		0,		1,		1},
	{fan_check_out,						100,		100,		0,		1,		1},
	{compre_fre_check_out,				100,		100,		0,		1,		1},
	{in_water_pres_low_check_out,		100,		100,		0,		1,		1},//暂时
	{out_water_pres_high_check_out,		100,		100,		0,		1,		1},
	{out_water_temp_low_check_out,		100,		100,		0,		1,		1},
	{out_water_temp_high_check_out,		100,		100,		0,		1,		1},
	{pump_check_out,					100,		100,		0,		1,		1},
	{ptc_check_out,						100,		100,		0,		1,		1},
	{motor_community_check_out,			100,		100,		0,		1,		1},
	{host_community_check_out,			100,		100,		0,		1,		1},
	{envit_temp_over_check_out,			100,		100,		0,		1,		1},
};

uint8_t g_tlevel1_fault[] =
{
	in_water_pres_low,
	out_water_pres_high,
	pump,
	power_control_board,
	adcref_control_board,
	power_expan,
	envit_temp_over,
};

uint8_t g_tlevel2_fault[] =
{
	ptc,
	out_water_temp_low,
	out_water_temp_high,
	in_motor_pres_low,
	in_motor_pres_sensor,
	in_motor_temp_sensor,
	out_motor_pres_sensor,
	out_motor_temp_sensor,
	out_motor_pres_high,
	out_motor_temp_high,
	compre_drive,
	communication_motor,
	fan1,
	fan2,
	fan3,
};

uint8_t g_tlevel3_fault[] =
{
	communication_bms,
	out_water_pres_sensor,
	out_water_temp_sensor,
	environment_temp_sensor,
	in_water_pres_sensor,
	in_watar_temp_sensor,
};

uint8_t g_thardware_fault[] =
{
	power_control_board,
	power_expan,
	adcref_control_board,
};

uint8_t g_twater_fault[] =
{
	in_water_pres_low,
	out_water_pres_high,
	pump,
};

uint8_t g_tref_fault[] =
{
	fan1,
	fan2,
	fan3,
	compre_drive,
	out_water_temp_low,
	out_motor_pres_high,
	in_motor_pres_low,
	out_motor_temp_high,
	envit_temp_over,
};

uint8_t g_theat_fault[] =
{
	ptc,
	out_water_temp_high,
};

uint8_t g_tcommunity_fault[] =
{
	communication_bms,
	communication_motor,
};


uint8_t g_tsensor_fault[] =
{
	in_watar_temp_sensor,
	out_water_temp_sensor,
	out_motor_temp_sensor,
	in_motor_temp_sensor,
	environment_temp_sensor,
	in_water_pres_sensor,
	out_water_pres_sensor,
	out_motor_pres_sensor,
	in_motor_pres_sensor,
};

fault_check_s *g_tfault_check[CHECK_NUM] =
{
	g_tfault_in_check,
	g_tfault_out_check,
};

void app_fault_tick_inc(fault_check_s *g_tfault_check)
{
	for(uint8_t i = 0;i < faule_check_list_max;i++)
	{
		if(1 == g_tfault_check[i].fault_en)
		{
			g_tfault_check[i].sys_tick++;
		}
		else
		{
			g_tfault_check[i].sys_tick = 0;
		}
	}
}

void app_fault_tick_inc_handle(void)
{
	for(uint8_t i = 0;i < CHECK_NUM;i++)
	{
		app_fault_tick_inc(g_tfault_check[i]);
	}
}

void data_updata(uint32_t *pbuff)
{
	uint32_t temp1,temp2;
	temp1 = pbuff[1];
	temp2 = pbuff[2];
	pbuff[0] = temp1;
	pbuff[1] = temp2;

}

void app_fault_check_en_set(fault_check_list index,uint8_t fault_en)
{
	g_tfault_in_check[index].fault_en = fault_en;
}

void app_fault_check_period_set(fault_check_list index,uint32_t time)
{
	g_tfault_in_check[index].period = time;
}

uint32_t app_fault_check_period_get(fault_check_list index)
{
	return g_tfault_in_check[index].period;
}

uint8_t app_get_fault_in_value(fault_diag_list index)
{
	return g_tfault_in_value[index].value;
}

uint16_t app_get_fault_in_num(fault_diag_list index)
{
	return g_tfault_in_value[index].num;
}

void diag_value(fault_diag_list index,uint32_t diag_value,uint16_t max_value,uint16_t min_value,fault_in_value_s *pfault)
{
	single_st_e temp = SINGLE_IDLE;

	if((diag_value > max_value) || (diag_value < min_value))
	{
		temp = single_input(&g_tfault_single[index],ERROR_STATUS);
	}
	else
	{
		temp = single_input(&g_tfault_single[index],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[index]))
		{
			pfault->value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[index]))
		{
			pfault->value = 1;
			pfault->num++;
		}
	}
}


void power_control_board_check_in(void)
{
	diag_value(power_control_board,DataBase_Get(sys_power_volt),32000,16000,&g_tfault_in_value[power_control_board]);
}

void power_control_board_check_out(void)
{
	if(g_tfault_out.power_control_board == g_tfault_in_value[power_control_board].value)
	{
		return;
	}

	if (1 == g_tfault_in_value[power_control_board].value)
	{
		g_tfault_out.power_control_board = 1;
		DataBase_Push(sys_power_control_board_err_num,DataBase_Get(sys_power_control_board_err_num) + 1);
		key_data_put(fault_con,fault_power_control_board);
	}
	else
	{
		g_tfault_out.power_control_board = 0;
		key_data_put(fault_rem,fault_power_control_board);
	}
}

/*void power_ipm_check_in(void)
{
	diag_value(power_ipm,DataBase_Get(sys_ipm_volt),17000,13000,&g_tfault_in_value[power_ipm]);
}

void power_ipm_check_out(void)
{
	g_tfault_out.power_ipm = g_tfault_in_value[power_ipm];
}*/

void power_expan_check_in(void)
{
	diag_value(power_expan,DataBase_Get(sys_expan_volt),16000,8000,&g_tfault_in_value[power_expan]);
}

void power_expan_check_out(void)
{
	if(g_tfault_out.power_expan == g_tfault_in_value[power_expan].value)
	{
		return;
	}

	if (1 == g_tfault_in_value[power_expan].value)
	{
		g_tfault_out.power_expan = 1;
		DataBase_Push(sys_power_expan_err_num,DataBase_Get(sys_power_expan_err_num) + 1);
		key_data_put(fault_con,fault_power_expan);
	}
	else
	{
		g_tfault_out.power_expan = 0;
		key_data_put(fault_rem,fault_power_expan);
	}
}

void adc_ref_check_in(void)
{
	diag_value(adcref_control_board,DataBase_Get(sys_adc_ref),2700,2300,&g_tfault_in_value[adcref_control_board]);
}

void adc_ref_check_out(void)
{
	if(g_tfault_out.adcref_control_board == g_tfault_in_value[adcref_control_board].value)
	{
		return;
	}

	if (1 == g_tfault_in_value[adcref_control_board].value)
	{
		g_tfault_out.adcref_control_board = 1;
		DataBase_Push(sys_adcref_control_board_err_num,DataBase_Get(sys_adcref_control_board_err_num) + 1);
		key_data_put(fault_con,fault_adcref_control_board);
	}
	else
	{
		g_tfault_out.adcref_control_board = 0;
		key_data_put(fault_rem,fault_adcref_control_board);
	}
}

void temp_sensor_check_in(void)
{
	diag_value(in_watar_temp_sensor,bsp_adc_volt_get(TEMP_INPUT_WATER),3196,108,&g_tfault_in_value[in_watar_temp_sensor]);
	diag_value(out_water_temp_sensor,bsp_adc_volt_get(TEMP_OUTPUT_WATER),3196,108,&g_tfault_in_value[out_water_temp_sensor]);
	diag_value(in_motor_temp_sensor,bsp_adc_volt_get(TEMP_MOTOR_IN),3196,108,&g_tfault_in_value[in_motor_temp_sensor]);
	diag_value(out_motor_temp_sensor,bsp_adc_volt_get(TEMP_MOTOR_OUT),3196,108,&g_tfault_in_value[out_motor_temp_sensor]);
	diag_value(environment_temp_sensor,bsp_adc_volt_get(TEMP_ENVIRONMENT),3196,108,&g_tfault_in_value[environment_temp_sensor]);
}

void temp_sensor_check_out(void)
{
	if(g_tfault_out.in_watar_temp_sensor != g_tfault_in_value[in_watar_temp_sensor].value)
	{
		if (1 == g_tfault_in_value[in_watar_temp_sensor].value)
		{
			g_tfault_out.in_watar_temp_sensor = 1;
			DataBase_Push(sys_in_watar_temp_sensor_err_num,DataBase_Get(sys_in_watar_temp_sensor_err_num) + 1);
			key_data_put(fault_con,fault_in_watar_temp_sensor);
		}
		else
		{
			g_tfault_out.in_watar_temp_sensor = 0;
			key_data_put(fault_rem,fault_in_watar_temp_sensor);
		}
	}

	if(g_tfault_out.out_water_temp_sensor != g_tfault_in_value[out_water_temp_sensor].value)
	{
		if (1 == g_tfault_in_value[out_water_temp_sensor].value)
		{
			g_tfault_out.out_water_temp_sensor = 1;
			DataBase_Push(sys_out_water_temp_sensor_err_num,DataBase_Get(sys_out_water_temp_sensor_err_num) + 1);
			key_data_put(fault_con,fault_out_water_temp_sensor);
		}
		else
		{
			g_tfault_out.out_water_temp_sensor = 0;
			key_data_put(fault_rem,fault_out_water_temp_sensor);
		}
	}

	if(g_tfault_out.in_motor_temp_sensor != g_tfault_in_value[in_motor_temp_sensor].value)
	{
		if (1 == g_tfault_in_value[in_motor_temp_sensor].value)
		{
			g_tfault_out.in_motor_temp_sensor = 1;
			DataBase_Push(sys_in_motor_temp_sensor_err_num,DataBase_Get(sys_in_motor_temp_sensor_err_num) + 1);
			key_data_put(fault_con,fault_in_motor_temp_sensor);
		}
		else
		{
			g_tfault_out.in_motor_temp_sensor = 0;
			key_data_put(fault_rem,fault_in_motor_temp_sensor);
		}
	}

	if(g_tfault_out.out_motor_temp_sensor != g_tfault_in_value[out_motor_temp_sensor].value)
	{
		if (1 == g_tfault_in_value[out_motor_temp_sensor].value)
		{
			g_tfault_out.out_motor_temp_sensor = 1;
			DataBase_Push(sys_out_motor_temp_sensor_err_num,DataBase_Get(sys_out_motor_temp_sensor_err_num) + 1);
			key_data_put(fault_con,fault_out_motor_temp_sensor);
		}
		else
		{
			g_tfault_out.out_motor_temp_sensor = 0;
			key_data_put(fault_rem,fault_out_motor_temp_sensor);
		}
	}

	if(g_tfault_out.environment_temp_sensor != g_tfault_in_value[environment_temp_sensor].value)
	{
		if (1 == g_tfault_in_value[environment_temp_sensor].value)
		{
			g_tfault_out.environment_temp_sensor = 1;
			DataBase_Push(sys_environment_temp_sensor_err_num,DataBase_Get(sys_environment_temp_sensor_err_num) + 1);
			key_data_put(fault_con,fault_environment_temp_sensor);
		}
		else
		{
			g_tfault_out.environment_temp_sensor = 0;
			key_data_put(fault_rem,fault_environment_temp_sensor);
		}
	}

}

void pres_sensor_check_in(void)
{
	diag_value(in_water_pres_sensor,bsp_adc_volt_get(PRESS_INPUT_WATER) * 15,  45500,4000,&g_tfault_in_value[in_water_pres_sensor]);
	diag_value(out_water_pres_sensor,bsp_adc_volt_get(PRESS_OUTPUT_WATER) * 15,45500,4000,&g_tfault_in_value[out_water_pres_sensor]);
	diag_value(in_motor_pres_sensor,bsp_adc_volt_get(PRESS_IN_MOTOR) * 15,     45500,4000,&g_tfault_in_value[in_motor_pres_sensor]);
	diag_value(out_motor_pres_sensor,bsp_adc_volt_get(PRESS_OUT_MOTOR) * 15,   45500,4000,&g_tfault_in_value[out_motor_pres_sensor]);
}

void pres_sensor_check_out(void)
{
	if(g_tfault_out.in_water_pres_sensor != g_tfault_in_value[in_water_pres_sensor].value)
	{
		if (1 == g_tfault_in_value[in_water_pres_sensor].value)
		{
			g_tfault_out.in_water_pres_sensor = 1;
			DataBase_Push(sys_in_water_pres_sensor_err_num,DataBase_Get(sys_in_water_pres_sensor_err_num) + 1);
			key_data_put(fault_con,fault_in_water_pres_sensor);
		}
		else
		{
			g_tfault_out.in_water_pres_sensor = 0;
			key_data_put(fault_rem,fault_in_water_pres_sensor);
		}
	}

	if(g_tfault_out.out_water_pres_sensor != g_tfault_in_value[out_water_pres_sensor].value)
	{
		if (1 == g_tfault_in_value[out_water_pres_sensor].value)
		{
			g_tfault_out.out_water_pres_sensor = 1;
			DataBase_Push(sys_out_water_pres_sensor_err_num,DataBase_Get(sys_out_water_pres_sensor_err_num) + 1);
			key_data_put(fault_con,fault_out_water_pres_sensor);
		}
		else
		{
			g_tfault_out.out_water_pres_sensor = 0;
			key_data_put(fault_rem,fault_out_water_pres_sensor);
		}
	}

	if(g_tfault_out.in_motor_pres_sensor != g_tfault_in_value[in_motor_pres_sensor].value)
	{
		if (1 == g_tfault_in_value[in_motor_pres_sensor].value)
		{
			g_tfault_out.in_motor_pres_sensor = 1;
			DataBase_Push(sys_in_motor_pres_sensor_err_num,DataBase_Get(sys_in_motor_pres_sensor_err_num) + 1);
			key_data_put(fault_con,fault_in_motor_pres_sensor);
		}
		else
		{
			g_tfault_out.in_motor_pres_sensor = 0;
			key_data_put(fault_rem,fault_in_motor_pres_sensor);
		}
	}

	if(g_tfault_out.out_motor_pres_sensor != g_tfault_in_value[out_motor_pres_sensor].value)
	{
		if (1 == g_tfault_in_value[out_motor_pres_sensor].value)
		{
			g_tfault_out.out_motor_pres_sensor = 1;
			DataBase_Push(sys_out_motor_pres_sensor_err_num,DataBase_Get(sys_out_motor_pres_sensor_err_num) + 1);
			key_data_put(fault_con,fault_out_motor_pres_sensor);
		}
		else
		{
			g_tfault_out.out_motor_pres_sensor = 0;
			key_data_put(fault_rem,fault_out_motor_pres_sensor);
		}
	}
}

void in_motor_pres_low_check_in(void)//系统低压故障
{
	single_st_e temp = SINGLE_IDLE;

	if(DataBase_Get(sys_in_pres_trans_temp) < DataBase_Get(sys_low_pres_protect_temp))
	{
		temp = single_input(&g_tfault_single[in_motor_pres_low],ERROR_STATUS);
	}
	else if(DataBase_Get(sys_in_pres_trans_temp) >= DataBase_Get(sys_low_pres_protect_temp) + 100)
	{
		temp = single_input(&g_tfault_single[in_motor_pres_low],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[in_motor_pres_low]))
		{
			g_tfault_in_value[in_motor_pres_low].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[in_motor_pres_low]))
		{
			g_tfault_in_value[in_motor_pres_low].value = 1;
			g_tfault_in_value[in_motor_pres_low].timer[g_tfault_in_value[in_motor_pres_low].num % 3] = get_tick_count();
			g_tfault_in_value[in_motor_pres_low].num++;
		}
	}
}

void in_motor_pres_low_check_out(void)
{
	static uint32_t timer = 0;

	if(1 == g_tfault_out.in_motor_pres_low)
	{
		return;
	}

	if((1 == g_tfault_in_value[in_motor_pres_low].value) && (0 == timer))
	{
		timer = get_tick_count();
	}
	else if(0 == g_tfault_in_value[in_motor_pres_low].value)
	{
		timer = 0;
	}

	if((get_elapsed_tick_count(timer) > 600000) && (0 != timer))
	{
		g_tfault_out.in_motor_pres_low = 1;
		DataBase_Push(sys_in_motor_pres_low_err_num,DataBase_Get(sys_in_motor_pres_low_err_num) + 1);
		key_data_put(fault_con,fault_in_motor_pres_low);
		app_fault_check_en_set(idx_in_motor_pres_low_check,0);
		timer = 0;
	}

	if((get_elapsed_tick_count(g_tfault_in_value[in_motor_pres_low].timer[0]) > 2400000) && \
	   (0 != g_tfault_in_value[in_motor_pres_low].timer[0]))//40min
	{
		if(g_tfault_in_value[in_motor_pres_low].num < 3)
		{
			if(g_tfault_in_value[in_motor_pres_low].num > 0)
			{
				g_tfault_in_value[in_motor_pres_low].num--;
			}
			data_updata(g_tfault_in_value[in_motor_pres_low].timer);
		}
		else
		{
			g_tfault_out.in_motor_pres_low = 1;
			DataBase_Push(sys_in_motor_pres_low_err_num,DataBase_Get(sys_in_motor_pres_low_err_num) + 1);
			key_data_put(fault_con,fault_in_motor_pres_low);
			app_fault_check_en_set(idx_in_motor_pres_low_check,0);
			timer = 0;
		}
	}
	else
	{
		if(g_tfault_in_value[in_motor_pres_low].num < 3)
		{
			return;
		}
			
		g_tfault_out.in_motor_pres_low = 1;
		DataBase_Push(sys_in_motor_pres_low_err_num,DataBase_Get(sys_in_motor_pres_low_err_num) + 1);
		key_data_put(fault_con,fault_in_motor_pres_low);
		app_fault_check_en_set(idx_in_motor_pres_low_check,0);
		timer = 0;
	}
}

void out_motor_pres_high_check_in(void)//系统高压故障
{
	single_st_e temp = SINGLE_IDLE;

	if(40000 == app_fault_check_period_get(idx_out_motor_pres_high_check))
	{
		app_fault_check_period_set(idx_out_motor_pres_high_check,10);
		g_tfault_single[out_motor_pres_high].status = SINGLE_ST1;
		g_tfault_single[out_motor_pres_high].temp_status = SINGLE_ST1;
		g_tfault_in_value[out_motor_pres_high].value = 0;
	}

	if(DataBase_Get(sys_out_pres_trans_temp) > DataBase_Get(sys_high_pres_protect_temp))//停机
	{
		temp = single_input(&g_tfault_single[out_motor_pres_high],SINGLE_ST4);
	}
	else if(DataBase_Get(sys_out_pres_trans_temp) >= DataBase_Get(sys_high_pres_protect_temp) - 20)//降频
	{
		temp = single_input(&g_tfault_single[out_motor_pres_high],SINGLE_ST3);
	}
	else if(DataBase_Get(sys_out_pres_trans_temp) >= DataBase_Get(sys_high_pres_protect_temp) - 40)//限频
	{
		temp = single_input(&g_tfault_single[out_motor_pres_high],SINGLE_ST2);
	}
	else if(DataBase_Get(sys_out_pres_trans_temp) <= DataBase_Get(sys_high_pres_protect_temp) - 60)
	{
		temp = single_input(&g_tfault_single[out_motor_pres_high],SINGLE_ST1);
	}

	if(temp != SINGLE_IDLE)
	{
		if(SINGLE_ST1 == single_get_st(&g_tfault_single[out_motor_pres_high]))
		{
			g_tfault_in_value[out_motor_pres_high].value = 0;
			DataBase_Push(sys_fre_limit,0);
		}
		else if(SINGLE_ST2 == single_get_st(&g_tfault_single[out_motor_pres_high]))
		{
			g_tfault_in_value[out_motor_pres_high].value = fault_level_1;//限频
			DataBase_Push(sys_fre_limit,1);
		}
		else if(SINGLE_ST3 == single_get_st(&g_tfault_single[out_motor_pres_high]))
		{
			g_tfault_in_value[out_motor_pres_high].value = fault_level_2;//降频
			DataBase_Push(sys_fre_red,1);
		}
		else if(SINGLE_ST4 == single_get_st(&g_tfault_single[out_motor_pres_high]))
		{
			g_tfault_in_value[out_motor_pres_high].value = fault_level_3;//停机
			g_tfault_in_value[out_motor_pres_high].timer[g_tfault_in_value[out_motor_pres_high].num % 3] = get_tick_count();
			g_tfault_in_value[out_motor_pres_high].num++;
		}
	}
}

void out_motor_pres_high_check_out(void)
{
	static uint32_t timer = 0;

	if(1 == g_tfault_out.out_motor_pres_high)
	{
		return;
	}

	if(fault_level_2 == g_tfault_in_value[out_motor_pres_high].value)
	{
		app_fault_check_period_set(idx_out_motor_pres_high_check,40000);//40s
	}

	if((get_elapsed_tick_count(g_tfault_in_value[out_motor_pres_high].timer[0]) > 2400000) && (0 != g_tfault_in_value[out_motor_pres_high].timer[0]))//40min
	{
		if(g_tfault_in_value[out_motor_pres_high].num < 3)
		{
			if(g_tfault_in_value[out_motor_pres_high].num > 1)
			{
				g_tfault_in_value[out_motor_pres_high].num--;
			}
			data_updata(g_tfault_in_value[out_motor_pres_high].timer);
		}
		else
		{
			g_tfault_out.out_motor_pres_high = 1;
			DataBase_Push(sys_out_motor_pres_high_err_num,DataBase_Get(sys_out_motor_pres_high_err_num) + 1);
			key_data_put(fault_con,fault_out_motor_pres_high);
			app_fault_check_en_set(idx_out_motor_pres_high_check,0);		
			timer = 0;		
		}
	}
	else
	{
		if(g_tfault_in_value[out_motor_pres_high].num < 3)
		{
			return;
		}
		g_tfault_out.out_motor_pres_high = 1;
		key_data_put(fault_con,fault_out_motor_pres_high);
		DataBase_Push(sys_out_motor_pres_high_err_num,DataBase_Get(sys_out_motor_pres_high_err_num) + 1);
		app_fault_check_en_set(idx_out_motor_pres_high_check,0);
	}
}

void out_motor_temp_high_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if(DataBase_Get(sys_out_motor_temp) > DataBase_Get(sys_out_motor_high_temp_protect))
	{
		temp = single_input(&g_tfault_single[out_motor_temp_high],ERROR_STATUS);
	}
	else if(DataBase_Get(sys_out_motor_temp) <= DataBase_Get(sys_out_motor_high_temp_protect) - 200)
	{
		temp = single_input(&g_tfault_single[out_motor_temp_high],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[out_motor_temp_high]))
		{
			g_tfault_in_value[out_motor_temp_high].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[out_motor_temp_high]))
		{
			g_tfault_in_value[out_motor_temp_high].value = 1;
			g_tfault_in_value[out_motor_temp_high].timer[g_tfault_in_value[out_motor_temp_high].num % 3] = get_tick_count();
			g_tfault_in_value[out_motor_temp_high].num++;
		}
	}
}

void out_motor_temp_high_check_out(void)
{
	if(1 == g_tfault_out.out_motor_temp_high)
	{
		return;
	}

	if((get_elapsed_tick_count(g_tfault_in_value[out_motor_temp_high].timer[0]) > 2400000) && (0 != g_tfault_in_value[out_motor_temp_high].timer[0]))//40min
	{
		if(g_tfault_in_value[out_motor_temp_high].num < 3)
		{
			if(g_tfault_in_value[out_motor_temp_high].num > 1)
			{
				g_tfault_in_value[out_motor_temp_high].num--;
			}
			data_updata(g_tfault_in_value[out_motor_temp_high].timer);
		}
		else
		{
			g_tfault_out.out_motor_temp_high = 1;
			DataBase_Push(sys_out_motor_temp_high_err_num,DataBase_Get(sys_out_motor_temp_high_err_num) + 1);
			key_data_put(fault_con,fault_out_motor_temp_high);
			app_fault_check_en_set(idx_out_motor_temp_high_check,0);			
		}
	}
	else
	{
		if(g_tfault_in_value[out_motor_temp_high].num < 3)
		{
			return;
		}
		g_tfault_out.out_motor_temp_high = 1;
		DataBase_Push(sys_out_motor_temp_high_err_num,DataBase_Get(sys_out_motor_temp_high_err_num) + 1);
		key_data_put(fault_con,fault_out_motor_temp_high);
		app_fault_check_en_set(idx_out_motor_temp_high_check,0);							
	}
}

void fan_check_in(void)
{
	uint8_t level_temp = 0;
	uint8_t duty_temp = 0;
	uint32_t fan_speed_max;
	uint32_t fan_speed_min;
	level_temp = DataBase_Get(sys_fan_level);
	switch (level_temp)
	{
		case 0:
			duty_temp = 0;
		break;

		case 1:
			duty_temp = DataBase_Get(sys_fan_low_duty);
		break;

		case 2:
			duty_temp = DataBase_Get(sys_fan_middle_duty);
		break;

		case 3:
			duty_temp = DataBase_Get(sys_fan_high_duty);
		break;

		default:
		break;
	}
	if(duty_temp != 0)
	{
		fan_speed_max = 38 * duty_temp;//  2800 * duty_temp / 100 * 1.5
		fan_speed_min = 14 * duty_temp;//  2800 * duty_temp / 100 * 0.5
	}
	else
	{
		fan_speed_max =	100;
		fan_speed_min = 0;		
	}

//	diag_value(fan1,DataBase_Get(sys_fan1_fb),fan_speed_max,fan_speed_min,&g_tfault_in_value[fan1]);
// 	diag_value(fan2,DataBase_Get(sys_fan2_fb),fan_speed_max,fan_speed_min,&g_tfault_in_value[fan2]);
//	diag_value(fan3,DataBase_Get(sys_fan3_fb),fan_speed_max,fan_speed_min,&g_tfault_in_value[fan3]);
}

void fan_check_out(void)
{
	if(g_tfault_out.fan1 != g_tfault_in_value[fan1].value)
	{
		if (1 == g_tfault_in_value[fan1].value)
		{
			g_tfault_out.fan1 = 1;
			DataBase_Push(sys_fan1_err_num,DataBase_Get(sys_fan1_err_num) + 1);
			key_data_put(fault_con,fault_fan1);
		}
		else
		{
			g_tfault_out.fan1 = 0;
			key_data_put(fault_rem,fault_fan1);
		}
	}

	if(g_tfault_out.fan2 != g_tfault_in_value[fan2].value)
	{
		if (1 == g_tfault_in_value[fan2].value)
		{
			g_tfault_out.fan2 = 1;
			DataBase_Push(sys_fan2_err_num,DataBase_Get(sys_fan2_err_num) + 1);
			key_data_put(fault_con,fault_fan2);
		}
		else
		{
			g_tfault_out.fan2 = 0;
			key_data_put(fault_rem,fault_fan2);
		}
	}

	if(g_tfault_out.fan3 != g_tfault_in_value[fan3].value)
	{
		if (1 == g_tfault_in_value[fan3].value)
		{
			g_tfault_out.fan3 = 1;
			DataBase_Push(sys_fan3_err_num,DataBase_Get(sys_fan3_err_num) + 1);
			key_data_put(fault_con,fault_fan3);
		}
		else
		{
			g_tfault_out.fan3 = 0;
			key_data_put(fault_rem,fault_fan3);
		}
	}
}

void compre_fre_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if((DataBase_Get(sys_motor_fault_code1) != 0) || (DataBase_Get(sys_motor_fault_code2) != 0))
	{
		temp = single_input(&g_tfault_single[compre_drive],ERROR_STATUS);
	}
	else
	{
		temp = single_input(&g_tfault_single[compre_drive],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[compre_drive]))
		{
			g_tfault_in_value[compre_drive].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[compre_drive]))
		{
			g_tfault_in_value[compre_drive].value = 1;
			g_tfault_in_value[compre_drive].num++;
		}
	}
}

void compre_fre_check_out(void)
{
	if(g_tfault_out.compre_drive != g_tfault_in_value[compre_drive].value)
	{
		if (1 == g_tfault_in_value[compre_drive].value)
		{
			g_tfault_out.compre_drive = 1;
			DataBase_Push(sys_compre_fre_err_num,DataBase_Get(sys_compre_fre_err_num) + 1);
			key_data_put(fault_con,fault_compre_drive);
		}
		else
		{
			g_tfault_out.compre_drive = 0;
			key_data_put(fault_rem,fault_compre_drive);
		}
	}
}

void in_water_pres_low_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if(DataBase_Get(sys_in_water_press) < DataBase_Get(sys_in_water_low_pres_protect))
	{
		temp = single_input(&g_tfault_single[in_water_pres_low],ERROR_STATUS);
	}
	else if(DataBase_Get(sys_in_water_press) >= DataBase_Get(sys_in_water_low_pres_protect) + 3)
	{
		temp = single_input(&g_tfault_single[in_water_pres_low],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[in_water_pres_low]))
		{
			g_tfault_in_value[in_water_pres_low].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[in_water_pres_low]))
		{
			g_tfault_in_value[in_water_pres_low].value = 1;
			g_tfault_in_value[in_water_pres_low].timer[g_tfault_in_value[in_water_pres_low].num % 3] = get_tick_count();
			g_tfault_in_value[in_water_pres_low].num++;
		}
	}
}

void in_water_pres_low_check_out(void)
{
	if(0 == g_tfault_in_check[idx_in_water_pres_low_check].fault_en)
	{
		return;
	}

	g_tfault_out.in_water_pres_low = g_tfault_in_value[in_water_pres_low].value;
	if((get_elapsed_tick_count(g_tfault_in_value[in_water_pres_low].timer[0]) > 2400000) && (0 != g_tfault_in_value[in_water_pres_low].timer[0]))//40min
	{
		if(g_tfault_in_value[in_water_pres_low].num < 3)
		{
			if(g_tfault_in_value[in_water_pres_low].num > 1)
			{
				g_tfault_in_value[in_water_pres_low].num--;
			}
			data_updata(g_tfault_in_value[in_water_pres_low].timer);
		}
		else
		{
			g_tfault_out.in_water_pres_low = 1;
			DataBase_Push(sys_in_water_pres_low_err_num,DataBase_Get(sys_in_water_pres_low_err_num) + 1);
			key_data_put(fault_con,fault_in_water_pres_low);
			app_fault_check_en_set(idx_in_water_pres_low_check,0);			
		}
	}
	else
	{
		if(g_tfault_in_value[in_water_pres_low].num < 3)
		{
			return;
		}
		g_tfault_out.in_water_pres_low = 1;
		DataBase_Push(sys_in_water_pres_low_err_num,DataBase_Get(sys_in_water_pres_low_err_num) + 1);
		key_data_put(fault_con,fault_in_water_pres_low);
		app_fault_check_en_set(idx_in_water_pres_low_check,0);							
	}
}

void out_water_pres_high_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if(DataBase_Get(sys_out_water_press) >= DataBase_Get(sys_out_water_high_pres_protect))
	{
		temp = single_input(&g_tfault_single[out_water_pres_high],ERROR_STATUS);
	}
	else if(DataBase_Get(sys_out_water_press) <= DataBase_Get(sys_out_water_high_pres_protect) - 3)
	{
		temp = single_input(&g_tfault_single[out_water_pres_high],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[out_water_pres_high]))
		{
			g_tfault_in_value[out_water_pres_high].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[out_water_pres_high]))
		{
			g_tfault_in_value[out_water_pres_high].value = 1;
			g_tfault_in_value[out_water_pres_high].timer[g_tfault_in_value[out_water_pres_high].num % 3] = get_tick_count();
			g_tfault_in_value[out_water_pres_high].num++;
		}
	}
}

void out_water_pres_high_check_out(void)
{
	if(0 == g_tfault_in_check[idx_out_water_pres_high_check].fault_en)
	{
		return;
	}

	g_tfault_out.out_water_pres_high = g_tfault_in_value[out_water_pres_high].value;

	if((get_elapsed_tick_count(g_tfault_in_value[out_water_pres_high].timer[0]) > 2400000) && (0 != g_tfault_in_value[out_water_pres_high].timer[0]))//40min
	{
		if(g_tfault_in_value[out_water_pres_high].num < 3)
		{
			if(g_tfault_in_value[out_water_pres_high].num > 1)
			{
				g_tfault_in_value[out_water_pres_high].num--;
			}
			data_updata(g_tfault_in_value[out_water_pres_high].timer);
		}
		else
		{
			g_tfault_out.out_water_pres_high = 1;
			DataBase_Push(sys_out_water_pres_high_err_num,DataBase_Get(sys_out_water_pres_high_err_num) + 1);
			key_data_put(fault_con,fault_out_water_pres_high);
			app_fault_check_en_set(idx_out_water_pres_high_check,0);			
		}
	}
	else
	{
		if(g_tfault_in_value[out_water_pres_high].num < 3)
		{
			return;
		}
		g_tfault_out.out_water_pres_high = 1;
		DataBase_Push(sys_out_water_pres_high_err_num,DataBase_Get(sys_out_water_pres_high_err_num) + 1);
		key_data_put(fault_con,fault_out_water_pres_high);
		app_fault_check_en_set(idx_out_water_pres_high_check,0);							
	}
}

void out_water_temp_low_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;
	if(DataBase_Get(sys_out_water_temp) <= 350)//-5℃
	{
		temp = single_input(&g_tfault_single[out_water_temp_low],ERROR_STATUS);
	}
	else if(DataBase_Get(sys_out_water_temp) > 400)
	{
		temp = single_input(&g_tfault_single[out_water_temp_low],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[out_water_temp_low]))
		{
			g_tfault_in_value[out_water_temp_low].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[out_water_temp_low]))
		{
			g_tfault_in_value[out_water_temp_low].value = 1;
			g_tfault_in_value[out_water_temp_low].num++;
		}
	}
}

void out_water_temp_low_check_out(void)
{
	if(g_tfault_out.out_water_temp_low != g_tfault_in_value[out_water_temp_low].value)
	{
		if (1 == g_tfault_in_value[out_water_temp_low].value)
		{
			g_tfault_out.out_water_temp_low = 1;
			DataBase_Push(sys_out_water_temp_low_err_num,DataBase_Get(sys_out_water_temp_low_err_num) + 1);
			key_data_put(fault_con,fault_out_water_temp_low);
		}
		else
		{
			g_tfault_out.out_water_temp_low = 0;
			key_data_put(fault_rem,fault_out_water_temp_low);
		}
	}
}

void out_water_temp_high_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;
	if(DataBase_Get(sys_out_water_temp) >= 950)
	{
		temp = single_input(&g_tfault_single[out_water_temp_high],ERROR_STATUS);
	}
	else if(DataBase_Get(sys_out_water_temp) < 900)
	{
		temp = single_input(&g_tfault_single[out_water_temp_high],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[out_water_temp_high]))
		{
			g_tfault_in_value[out_water_temp_high].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[out_water_temp_high]))
		{
			g_tfault_in_value[out_water_temp_high].value = 1;
			g_tfault_in_value[out_water_temp_high].num++;
		}
	}
}

void out_water_temp_high_check_out(void)
{
	if(g_tfault_out.out_water_temp_high != g_tfault_in_value[out_water_temp_high].value)
	{
		if (1 == g_tfault_in_value[out_water_temp_high].value)
		{
			g_tfault_out.out_water_temp_high = 1;
			DataBase_Push(sys_out_water_temp_high_err_num,DataBase_Get(sys_out_water_temp_high_err_num) + 1);
			key_data_put(fault_con,fault_out_water_temp_high);
			
		}
		else
		{
			g_tfault_out.out_water_temp_high = 0;
			key_data_put(fault_rem,fault_out_water_temp_high);
		}
	}
}
pump_fault_out_s g_tpump_fault_out;
void pump_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if(0 == bsp_get_input(pump_err_chk))
	{
		temp = single_input(&g_tfault_single[pump],ERROR_STATUS);
	}
	else
	{
		temp = single_input(&g_tfault_single[pump],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[pump]))
		{
			g_tfault_in_value[pump].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[pump]))
		{
			g_tfault_in_value[pump].value = 1;
			g_tfault_in_value[pump].num++;
		}
	}
}

void pump_check_out(void)
{
	if(g_tfault_out.pump != g_tfault_in_value[pump].value)
	{
		if (1 == g_tfault_in_value[pump].value)
		{
			g_tfault_out.pump = 1;
			DataBase_Push(sys_pump_err_num,DataBase_Get(sys_pump_err_num) + 1);
			key_data_put(fault_con,fault_pump);
		}
		else
		{
			g_tfault_out.pump = 0;
			key_data_put(fault_rem,fault_pump);
		}
	}
}

void ptc_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if(1 == bsp_get_input(ptc_err_chk))
	{
		temp = single_input(&g_tfault_single[ptc],ERROR_STATUS);
	}
	else
	{
		temp = single_input(&g_tfault_single[ptc],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[ptc]))
		{
			g_tfault_in_value[ptc].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[ptc]))
		{
			g_tfault_in_value[ptc].value = 1;
			g_tfault_in_value[ptc].num++;
		}
	}
}

void ptc_check_out(void)
{
	if(g_tfault_out.ptc != g_tfault_in_value[ptc].value)
	{
		if (1 == g_tfault_in_value[ptc].value)
		{
			g_tfault_out.ptc = 1;
			DataBase_Push(sys_ptc_err_num,DataBase_Get(sys_ptc_err_num) + 1);
			key_data_put(fault_con,fault_ptc);
		}
		else
		{
			g_tfault_out.ptc = 0;
			key_data_put(fault_rem,fault_ptc);
		}
	}
}

void motor_community_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if(1 == DataBase_Get(sys_motor_community_err))
	{
		temp = single_input(&g_tfault_single[communication_motor],ERROR_STATUS);
	}
	else
	{
		temp = single_input(&g_tfault_single[communication_motor],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[communication_motor]))
		{
			g_tfault_in_value[communication_motor].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[communication_motor]))
		{
			g_tfault_in_value[communication_motor].value = 1;
			g_tfault_in_value[communication_motor].num++;
		}
	}
}

void motor_community_check_out(void)
{
	if(g_tfault_out.communication_motor != g_tfault_in_value[communication_motor].value)
	{
		if (1 == g_tfault_in_value[communication_motor].value)
		{
			g_tfault_out.communication_motor = 1;
			DataBase_Push(sys_communication_motor_err_num,DataBase_Get(sys_communication_motor_err_num) + 1);
			key_data_put(fault_con,fault_communication_motor);
		}
		else
		{
			g_tfault_out.communication_motor = 0;
			key_data_put(fault_rem,fault_communication_motor);
		}
	}
}

void host_community_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if(1 == DataBase_Get(sys_bms_community_err))
	{
		temp = single_input(&g_tfault_single[communication_bms],ERROR_STATUS);
	}
	else
	{
		temp = single_input(&g_tfault_single[communication_bms],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[communication_bms]))
		{
			g_tfault_in_value[communication_bms].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[communication_bms]))
		{
			g_tfault_in_value[communication_bms].value = 1;
			g_tfault_in_value[communication_bms].num++;
		}
	}
}

void host_community_check_out(void)
{
	if(g_tfault_out.communication_bms != g_tfault_in_value[communication_bms].value)
	{
		if (1 == g_tfault_in_value[communication_bms].value)
		{
			g_tfault_out.communication_bms = 1;
			DataBase_Push(sys_communication_host_err_num,DataBase_Get(sys_communication_host_err_num) + 1);
			key_data_put(fault_con,fault_communication_bms);
		}
		else
		{
			g_tfault_out.communication_bms = 0;
			key_data_put(fault_rem,fault_communication_bms);
		}
	}
}

void envit_temp_over_check_in(void)
{
	single_st_e temp = SINGLE_IDLE;

	if((DataBase_Get(sys_environment_temp) > DataBase_Get(sys_ref_max_temp)) || (DataBase_Get(sys_environment_temp) <= DataBase_Get(sys_ref_min_temp)))
	{
		temp = single_input(&g_tfault_single[envit_temp_over],ERROR_STATUS);
	}
	else
	{
		temp = single_input(&g_tfault_single[envit_temp_over],NORMAL_STATUS);
	}

	if(temp != SINGLE_IDLE)
	{
		if(NORMAL_STATUS == single_get_st(&g_tfault_single[envit_temp_over]))
		{
			g_tfault_in_value[envit_temp_over].value = 0;
		}
		else if(ERROR_STATUS == single_get_st(&g_tfault_single[envit_temp_over]))
		{
			g_tfault_in_value[envit_temp_over].value = 1;
			g_tfault_in_value[envit_temp_over].num++;
		}
	}
}
void envit_temp_over_check_out(void)
{
	if(g_tfault_out.envit_temp_over != g_tfault_in_value[envit_temp_over].value)
	{
		if (1 == g_tfault_in_value[envit_temp_over].value)
		{
			g_tfault_out.envit_temp_over = 1;
			DataBase_Push(sys_envir_temp_over_err_num,DataBase_Get(sys_envir_temp_over_err_num) + 1);
			key_data_put(fault_con,fault_envit_temp_over);
		}
		else
		{
			g_tfault_out.envit_temp_over = 0;
			key_data_put(fault_rem,fault_envit_temp_over);
		}
	}
}

void envir_low_check(void)
{
	if(DataBase_Get(sys_environment_temp) < 50)
	{
		DataBase_Push(sys_envir_low,1);
	}
	else
	{
		DataBase_Push(sys_envir_low,0);
	}
}

void motor_out_temp_low_check()
{
	static uint32_t timer = 0;
	if(get_elapsed_tick_count(DataBase_Get(sys_motor_run_time)) > 150000)
	{
		if(0 == DataBase_Get(sys_motor_out_temp_low_ctl))
		{
			if(DataBase_Get(sys_out_motor_temp) <= DataBase_Get(sys_out_pres_trans_temp) + 100)
			{
				if(0 == timer)
				{
					timer = get_tick_count();
				}
				
				if(get_elapsed_tick_count(timer) > 40000)
				{
					DataBase_Push(sys_motor_out_temp_low_ctl,1);
					timer = 0;
				}
			}
			else
			{
				timer = 0;
			}
		}
		else
		{
			if(DataBase_Get(sys_out_motor_temp) > DataBase_Get(sys_out_pres_trans_temp) + 150)
			{
				if(0 == timer)
				{
					timer = get_tick_count();
				}
				
				if(get_elapsed_tick_count(timer) > 10000)
				{
					DataBase_Push(sys_motor_out_temp_low_ctl,0);
					timer = 0;
				}
			}
			else
			{
				timer = 0;
			}
		}

	}
	else
	{
		timer = 0;
	}
}

void motor_out_temp_high_check()
{
	static uint32_t timer = 0;
	if(get_elapsed_tick_count(DataBase_Get(sys_motor_run_time)) > 150000)
	{
		if(0 == DataBase_Get(sys_motor_out_temp_high_ctl))
		{
			if(DataBase_Get(sys_out_motor_temp) >= 1500)
			{
				if(0 == timer)
				{
					timer = get_tick_count();
				}
				
				if(get_elapsed_tick_count(timer) > 3000)
				{
					DataBase_Push(sys_motor_out_temp_high_ctl,1);
					timer = 0;
				}
			}
			else
			{
				timer = 0;
			}
		}
		else
		{
			if(DataBase_Get(sys_out_motor_temp) < 1350)
			{
				if(0 == timer)
				{
					timer = get_tick_count();
				}
				
				if(get_elapsed_tick_count(timer) > 3000)
				{
					DataBase_Push(sys_motor_out_temp_high_ctl,0);
					timer = 0;
				}
			}
			else
			{
				timer = 0;
			}
		}
	}
	else
	{
		timer = 0;
	}
}

void app_fault_check(fault_check_s *g_tfault_check)
{
	for(uint8_t i = 0;i < faule_check_list_max;i++)
	{
		if(0 == g_tfault_check[i].fault_en)
		{
			continue;
		}

		if(g_tfault_check[i].sys_tick < g_tfault_check[i].period)
		{
			continue;
		}
		g_tfault_check[i].sys_tick = 0;
		g_tfault_check[i].fault_check();
	}
}

void app_fault_check_handle(void)
{
	for(uint8_t i = 0;i < CHECK_NUM;i++)
	{
		app_fault_check(g_tfault_check[i]);
	}
	envir_low_check();
	motor_out_temp_low_check();
	motor_out_temp_high_check();
	app_wdt_feed();
	DataBase_Down();
}

uint8_t heat_err = 0;
uint8_t pump_err = 0;
uint8_t ref_err = 0;
uint8_t low_ref_err = 0;
void fault_level_check(void)
{
	uint32_t *temp;
	temp = (uint32_t *)(&g_tfault_out);
	for(uint8_t i = 0;i < sizeof(g_tlevel1_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_tlevel1_fault[i]))
		{
			DataBase_Push(sys_fault_level,1);
			return;
		}
	}

	for(uint8_t i = 0;i < sizeof(g_tlevel2_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_tlevel2_fault[i]))
		{
			DataBase_Push(sys_fault_level,2);
			return;
		}
	}

	for(uint8_t i = 0;i < sizeof(g_tlevel3_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_tlevel3_fault[i]))
		{
			DataBase_Push(sys_fault_level,3);
			return;
		}
	}

	DataBase_Push(sys_fault_level,0);
}

void fault_sum_check(void)
{
	uint32_t *temp;
	temp = (uint32_t *)(&g_tfault_out);

	for(uint8_t i = 0;i < sizeof(g_thardware_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_thardware_fault[i]))
		{
			g_tfault_sum.hard_fault = 1;
			break;
		}
		g_tfault_sum.hard_fault = 0;
	}

	for(uint8_t i = 0;i < sizeof(g_twater_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_twater_fault[i]))
		{
			g_tfault_sum.water_fault = 1;
			break;
		}
		g_tfault_sum.water_fault = 0;
	}

	for(uint8_t i = 0;i < sizeof(g_tref_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_tref_fault[i]))
		{
			g_tfault_sum.ref_fault = 1;
			break;
		}
		g_tfault_sum.ref_fault = 0;
	}

	for(uint8_t i = 0;i < sizeof(g_theat_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_theat_fault[i]))
		{
			g_tfault_sum.heat_fault = 1;
			break;
		}
		g_tfault_sum.heat_fault = 0;
	}

	for(uint8_t i = 0;i < sizeof(g_tcommunity_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_tcommunity_fault[i]))
		{
			g_tfault_sum.community_fault = 1;
			break;
		}
		g_tfault_sum.community_fault = 0;
	}

	for(uint8_t i = 0;i < sizeof(g_tsensor_fault);i++)
	{
		if(1 == GetU32Bit(temp,g_tsensor_fault[i]))
		{
			g_tfault_sum.sensor_fault = 1;
			break;
		}
		g_tfault_sum.sensor_fault = 0;
	}

	DataBase_Push(sys_fault_sum,*(uint16_t*)(&g_tfault_sum));
}
fault_code1_s g_tfault_code1;
fault_code2_s g_tfault_code2;
fault_code3_s g_tfault_code3;
void mods_fault_code_put(void)
{
	g_tfault_code1.pump = g_tfault_out.pump;
	g_tfault_code1.rvd1 = 0;
	g_tfault_code1.in_water_pres_low = g_tfault_out.in_water_pres_low;
	g_tfault_code1.out_water_pres_high = g_tfault_out.out_water_pres_high;
	g_tfault_code1.rvd2 = 0;
	g_tfault_code1.power_control_board =  g_tfault_out.power_control_board;
	g_tfault_code1.power_expan = g_tfault_out.power_expan;
	
	g_tfault_code2.in_motor_pres_low = g_tfault_out.in_motor_pres_low;
	g_tfault_code2.ptc = g_tfault_out.ptc;
	g_tfault_code2.fan = g_tfault_out.fan1 | g_tfault_out.fan2 | g_tfault_out.fan3;
	g_tfault_code2.out_motor_pres_high = g_tfault_out.out_motor_pres_high;
	g_tfault_code2.rvd1 = 0;
	g_tfault_code2.out_motor_pres_sensor = g_tfault_out.out_motor_pres_sensor;
	g_tfault_code2.communication_motor = g_tfault_out.communication_motor;
	g_tfault_code2.adcref_control_board = g_tfault_out.adcref_control_board;
	g_tfault_code2.out_motor_temp_sensor = g_tfault_out.out_motor_temp_sensor;
	g_tfault_code2.in_motor_temp_sensor = g_tfault_out.in_motor_temp_sensor;
	g_tfault_code2.in_water_pres_sensor = g_tfault_out.in_water_pres_sensor;
	g_tfault_code2.out_water_pres_sensor = g_tfault_out.out_water_pres_sensor;
	g_tfault_code2.in_motor_pres_sensor = g_tfault_out.in_motor_pres_sensor;
	g_tfault_code2.out_motor_temp_high = g_tfault_out.out_motor_temp_high;

	g_tfault_code3.out_water_temp_sensor = g_tfault_out.out_water_temp_sensor;
	g_tfault_code3.in_watar_temp_sensor = g_tfault_out.in_watar_temp_sensor;
	g_tfault_code3.environment_temp_sensor = g_tfault_out.environment_temp_sensor;
	g_tfault_code3.out_water_temp_high = g_tfault_out.out_water_temp_high;
	g_tfault_code3.in_water_pres_sensor = g_tfault_out.in_water_pres_sensor;
	g_tfault_code3.rvd1 = 0;
	g_tfault_code3.out_water_temp_low = g_tfault_out.out_water_temp_low;
	g_tfault_code3.rvd2 = 0;
	g_tfault_code3.compre_drive = g_tfault_out.compre_drive;
	g_tfault_code3.communication_bms = g_tfault_out.communication_bms;
	g_tfault_code3.envit_temp_over = g_tfault_out.envit_temp_over;
	g_tfault_code3.rvd3 = 0;

	DataBase_Push(sys_fault_code1,*(uint16_t*)(&g_tfault_code1));
	DataBase_Push(sys_fault_code2,*(uint16_t*)(&g_tfault_code2));
	DataBase_Push(sys_fault_code3,*(uint16_t*)(&g_tfault_code3));
}

void app_fault_code_put(void)
{
	DataBase_Push(sys_fault_code,*(uint32_t*)(&g_tfault_out));


	pump_err = 	g_tfault_in_value[in_water_pres_low].value 		|
				g_tfault_in_value[out_water_pres_high].value 	|
				g_tfault_in_value[pump].value 	                |
				g_tfault_in_value[adcref_control_board].value 	|
				g_tfault_in_value[power_control_board].value 	|
				DataBase_Get(sys_envir_low)                     |
				(g_tfault_in_value[out_water_temp_sensor].value &
				g_tfault_in_value[in_watar_temp_sensor].value   &
				DataBase_Get(sys_ctl_temp_set) != bms_avr_temp);

	heat_err = 	g_tfault_in_value[out_water_temp_high].value 	|
				g_tfault_in_value[in_water_pres_low].value 		|
				g_tfault_in_value[out_water_pres_high].value 	|
				g_tfault_in_value[pump].value 	                |
				g_tfault_in_value[adcref_control_board].value   |
				g_tfault_in_value[power_control_board].value 	|
				g_tfault_in_value[ptc].value 	                |
				DataBase_Get(sys_envir_low)                     |
				(g_tfault_in_value[out_water_temp_sensor].value &
				g_tfault_in_value[in_watar_temp_sensor].value   &
				DataBase_Get(sys_ctl_temp_set) != bms_avr_temp);

	ref_err = 	g_tfault_in_value[in_water_pres_low].value 	|
				g_tfault_in_value[out_water_pres_high].value 	|
				g_tfault_in_value[pump].value 	                |
				g_tfault_in_value[adcref_control_board].value 	|
				g_tfault_in_value[power_expan].value 	        |
				g_tfault_in_value[power_control_board].value 	|
				g_tfault_in_value[out_water_temp_low].value 	|
				g_tfault_in_value[in_motor_pres_low].value 	    |
				g_tfault_in_value[in_motor_pres_sensor].value 	|
				g_tfault_in_value[in_motor_temp_sensor].value 	|
				g_tfault_in_value[out_motor_pres_sensor].value 	|
				g_tfault_in_value[out_motor_temp_sensor].value 	|
				(g_tfault_in_value[out_motor_pres_high].value == 3)|
				g_tfault_in_value[out_motor_temp_high].value 	|
				g_tfault_in_value[compre_drive].value 	        |
				g_tfault_in_value[fan1].value 	                |
				g_tfault_in_value[fan2].value 	                |
				g_tfault_in_value[fan3].value 	                |
				g_tfault_in_value[envit_temp_over].value 		|
				DataBase_Get(sys_envir_low)                     |
				(g_tfault_in_value[out_water_temp_sensor].value &
				g_tfault_in_value[in_watar_temp_sensor].value   &
				DataBase_Get(sys_ctl_temp_set) != bms_avr_temp);

	low_ref_err = 	g_tfault_in_value[in_water_pres_low].value 		|
					g_tfault_in_value[out_water_pres_high].value 	|
					g_tfault_in_value[pump].value 	                |
					g_tfault_in_value[adcref_control_board].value 	|
					g_tfault_in_value[power_control_board].value 	|
					g_tfault_in_value[out_water_temp_low].value 	|
					g_tfault_in_value[communication_motor].value 	|
					g_tfault_in_value[fan1].value 	                |
					g_tfault_in_value[fan2].value 	                |
					g_tfault_in_value[fan3].value 	                |
					DataBase_Get(sys_envir_low)                     |
					(g_tfault_in_value[out_water_temp_sensor].value &
				    g_tfault_in_value[in_watar_temp_sensor].value   &
					DataBase_Get(sys_ctl_temp_set) != bms_avr_temp);


	fault_level_check();
	fault_sum_check();
}


void app_fault_reset(fault_diag_list index)
{
	uint32_t *temp;
	temp = (uint32_t *)(&g_tfault_out);

	g_tfault_in_value[index].value = 0;
	g_tfault_in_value[index].num = 0;
	g_tfault_in_value[index].timer[0] = 0;
	g_tfault_in_value[index].timer[1] = 0;
	g_tfault_in_value[index].timer[2] = 0;
	g_tfault_single[index].status = SINGLE_ST1;
	g_tfault_single[index].temp_status = SINGLE_ST1;

	if(1 == GetU32Bit(temp,index))
	{
		app_wdt_feed();
		SetU32Bit(temp,index,0);
		key_data_put(fault_rem,index);
	}
}

void app_check_reset(fault_check_list index)
{
	g_tfault_in_check[index].fault_en = g_tfault_in_check[index].def_fault_en;
	g_tfault_in_check[index].period = g_tfault_in_check[index].def_period;
	g_tfault_in_check[index].sys_tick = 0;

	g_tfault_out_check[index].fault_en = g_tfault_out_check[index].def_fault_en;
	g_tfault_out_check[index].period = g_tfault_out_check[index].def_period;
	g_tfault_out_check[index].sys_tick = 0;
}

void app_fault_off(fault_check_list index)
{
	g_tfault_in_check[index].fault_en = 0;
	g_tfault_in_check[index].period = g_tfault_in_check[index].def_period;
	g_tfault_in_check[index].sys_tick = 0;

	g_tfault_out_check[index].fault_en = 0;
	g_tfault_out_check[index].period = g_tfault_out_check[index].def_period;
	g_tfault_out_check[index].sys_tick = 0;
}

void app_fault_reset_handle(void)
{
	for(uint8_t i = 0;i < fault_diag_max;i++)
	{
		app_fault_reset(i);
	}	

	for(uint8_t i = 0;i < faule_check_list_max;i++)
	{
		app_check_reset(i);
	}
	low_pres_single.status = SINGLE_ST2;
	low_pres_single.temp_status = SINGLE_ST2;
}

void app_fault_shield(void)
{
	for(uint8_t i = 0;i < fault_diag_max;i++)
	{
		app_fault_reset(i);
	}

	for(uint8_t i = 0;i < faule_check_list_max;i++)
	{
		app_fault_off(i);
	}

}

void tmep_sensor_fault_manage(void)
{
	static temp_sensor_fault_e state = sensor_normal;
	static ctl_temp_e ctl_temp;
	switch (state)
	{
		case sensor_normal:
			ctl_temp = DataBase_Get(sys_ctl_temp_set);
			if((1 == g_tfault_in_value[in_watar_temp_sensor].value) && (0 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = in_water_fault;
			}

			if((0 == g_tfault_in_value[in_watar_temp_sensor].value) && (1 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = out_water_fault;
			}

			if((1 == g_tfault_in_value[in_watar_temp_sensor].value) && (1 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = sensor_fault;
			}

		break;

		case in_water_fault:
			if(in_water_temp == DataBase_Get(sys_ctl_temp_set))
			{
				DataBase_Push(sys_ctl_temp_set,out_water_temp);
			}
			
			if((1 == g_tfault_in_value[in_watar_temp_sensor].value) && (1 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = sensor_fault;
			}

			if((0 == g_tfault_in_value[in_watar_temp_sensor].value) && (0 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = sensor_normal;
				DataBase_Push(sys_ctl_temp_set,ctl_temp);
			}	

			if((0 == g_tfault_in_value[in_watar_temp_sensor].value) && (1 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = out_water_fault;
			}

		break;

		case out_water_fault:
			if(out_water_temp == DataBase_Get(sys_ctl_temp_set))
			{
				DataBase_Push(sys_ctl_temp_set,in_water_temp);
			}

			if((1 == g_tfault_in_value[in_watar_temp_sensor].value) && (1 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = sensor_fault;
			}

			if((0 == g_tfault_in_value[in_watar_temp_sensor].value) && (0 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = sensor_normal;
				DataBase_Push(sys_ctl_temp_set,ctl_temp);
			}	

			if((1 == g_tfault_in_value[in_watar_temp_sensor].value) && (0 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = in_water_fault;
			}

		break;

		case sensor_fault:
			if((0 == g_tfault_in_value[in_watar_temp_sensor].value) && (0 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = sensor_normal;
				DataBase_Push(sys_ctl_temp_set,ctl_temp);
			}	

			if((1 == g_tfault_in_value[in_watar_temp_sensor].value) && (0 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = in_water_fault;
			}

			if((0 == g_tfault_in_value[in_watar_temp_sensor].value) && (1 == g_tfault_in_value[out_water_temp_sensor].value))
			{
				state = out_water_fault;
			}

		break;
		
		default:
		break;
	}
}

void fault_display_manage(void)
{
	if(sys_ref != DataBase_Get(sys_mode_run))
	{	
		app_fault_reset(out_motor_pres_high);
		app_fault_reset(in_motor_pres_low);
		app_fault_reset(out_motor_temp_high);
		app_fault_reset(envit_temp_over);
		app_fault_reset(out_water_temp_low);

		app_fault_check_en_set(idx_in_motor_pres_low_check ,0);
		app_fault_check_en_set(idx_out_motor_pres_high_check ,0);
		app_fault_check_en_set(idx_out_motor_temp_high_check ,0);
		app_fault_check_en_set(idx_envit_temp_over_check ,0);
		app_fault_check_en_set(idx_out_water_temp_low_check ,0);
	}
	else
	{
//		app_fault_check_en_set(idx_in_motor_pres_low_check,1);
		if(0 == g_tfault_in_value[out_motor_pres_high].value)
		{
			app_fault_check_en_set(idx_out_motor_pres_high_check,1);
		}
		if(0 == g_tfault_in_value[out_motor_temp_high].value)
		{
			app_fault_check_en_set(idx_out_motor_temp_high_check ,1);
		}

		if(0 == g_tfault_in_value[envit_temp_over].value)
		{
			app_fault_check_en_set(idx_envit_temp_over_check ,1);
		}

		if(0 == g_tfault_in_value[out_water_temp_low].value)
		{
			app_fault_check_en_set(idx_out_water_temp_low_check ,1);
		}

	}

	if(sys_heat != DataBase_Get(sys_mode_run))
	{
		app_fault_reset(out_water_temp_high);
		app_fault_check_en_set(idx_out_water_temp_high_check ,0);
	}
	else
	{
		if(0 == g_tfault_in_value[out_water_temp_high].value)
		{
			app_fault_check_en_set(idx_out_water_temp_high_check ,1);
		}
	}

}

void fault_host_community(void)
{
	if(1 != g_tfault_in_value[communication_bms].value)
	{
		return;
	}

	if(bms_avr_temp == DataBase_Get(sys_ctl_temp_set))
	{
		DataBase_Push(sys_ctl_temp_set,out_water_temp);
	}

	DataBase_Push(sys_run_mode_next,auto_mode);
}

void app_fault_manage(void)
{
	fault_display_manage();
	tmep_sensor_fault_manage();	
	fault_host_community();	
}	


void app_fault_diag_handle(void)
{
	app_fault_check_handle();
	app_fault_manage();
	app_fault_code_put();
}








