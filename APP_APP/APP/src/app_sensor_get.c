/*
 * app_sensor_get.c
 *
 *  Created on: 2024年3月22日
 *      Author: Dell
 */

#include "app_sensor_get.h"
temp_correct_s g_ttemp_correct;
extern fault_in_value_s g_tfault_in_value[fault_diag_max];

const float BoardTempNTCTab[TEMP_NTC_TABLE_LEN] =
{
	3147,     3139,     3130,     3121,     3111,     3101,     3090,     3079,     3067,     3056,	  /*	 -40  ~ -31  	*/
	3043,     3030,     3017,     3003,     2988,     2973,     2958,     2942,     2926,     2909,   /*	 -30  ~ -21  	*/
	2891,     2873,     2854,     2835,     2815,     2795,     2774,     2753,     2731,     2708,   /*	 -20  ~ -11  	*/
	2685,     2662,     2638,     2613,     2588,     2562,     2536,     2509,     2482,     2454,   /*	 -10  ~ -1	 	*/
	2426,     2398,     2369,     2339,     2310,     2280,     2249,     2219,     2188,     2157,   /*	   0  ~ 9	 	*/
	2126,     2094,     2063,     2031,     1999,     1967,     1935,     1903,     1872,     1840,   /*	  10  ~ 19	 	*/
	1808,     1776,     1774,     1713,     1681,     1650,     1619,     1588,     1558,     1528,   /*	  20  ~ 29	 	*/
	1498,     1468,     1438,     1409,     1380,     1352,     1324,     1296,     1269,     1241,   /*	  30  ~ 39	 	*/
	1215,     1188,     1162,     1137,     1112,     1087,     1063,     1039,     1015,     992,    /*	  40  ~ 49	 	*/
	970,      947,      925,      904,      883,      862,      842,      823,      803,      784,    /*	  50  ~ 59	 	*/
	766,      748,      730,      713,      696,      679,      663,      647,      632,      616,    /*	  60  ~ 69	 	*/
	602,      587,      573,      559,      546,      533,      520,      507,      495,      483,    /*	  70  ~ 79	 	*/
	472,      461,      449,      439,      428,      418,      408,      398,      388,      379,    /*	  80  ~ 89	 	*/
	369,      360,      352,      343,      335,      327,      319,      311,      304,      296,    /*	  90  ~ 99	 	*/
	289,      282,      275,      269,      262,      256,      250,      244,      239,      233,    /*	  100 ~ 109	 	*/
	228,      223,      218,      213,      208,      203,      198,      193,      188,      184,    /*	  110 ~ 119	 	*/
	179,      175,      170,      166,      162,      158,  										  /*	  120 ~ 125	 	*/
};

const float Press_TempTab[PRESS_TABLE_LEN] =
{
	52,   	54,   	57,   	61,   	64,   	68,   	72,   	76,   	80,   	84,   	/*	  -65  ~ -56  	*/
	89,   	93,   	98,   	103,  	108,  	113,  	119,  	125,  	131,  	138,	/*	  -55  ~ -46  	*/
	144,  	151,  	157,  	165,  	172,  	181,  	188,  	196,  	206,  	215, 	/*	  -45  ~ -36  	*/
	224,  	235,  	243,  	255,  	264,  	275,  	286,  	298,  	311,  	324,    /*	  -35  ~ -26  	*/
	334,  	348,  	363,  	375,  	391,  	404,  	424,  	435,  	453,  	468,    /*	  -25  ~ -16  	*/
	483,  	504,  	520,  	538,  	556,  	579,  	598,  	618,  	639,  	660,    /*	  -15  ~ -6  	*/
	682,  	705,  	728,  	752,  	777,  	803,  	823,  	851,  	879,  	903,    /*	  -5   ~ 4  	*/
	937,  	962,  	994,  	1020, 	1050, 	1090, 	1110, 	1150,	1180, 	1220,   /*	   5   ~ 14  	*/
	1250, 	1280, 	1320, 	1350, 	1400, 	1440, 	1470, 	1520, 	1560, 	1600,   /*	   15  ~ 24  	*/
	1640, 	1680, 	1730, 	1780, 	1820, 	1880, 	1910, 	1960, 	2030, 	2080,   /*	   25  ~ 34  	*/
	2130, 	2180, 	2240, 	2290, 	2350, 	2410, 	2460, 	2510, 	2580, 	2650,	/*	   35  ~ 44  	*/
	2710, 	2770, 	2840, 	2910, 	2980, 	3050, 	3100, 	3180, 	3250, 	3320,   /*	   45  ~ 54  	*/
	3400, 	3480,   3540, 	3630,  	3720, 	3780, 	3900,  	3970,   4094,   4184,   /*	   55  ~ 64  	*/
	4275, 	4369,   4464, 	4562,  	4600											/*	   65  ~ 69  	*/
};

float Serch_Harf_Temp(uint16_t res,const float *table,uint16_t size)
{
	uint16_t index_left,index_right,index_middle;
	index_left = 0;
	index_right = size - 1;

	while(index_left < index_right)
	{
		if(index_right - index_left == 1)
		{
			return (index_left+(table[index_left] - res)/(table[index_left] - table[index_right])) * 10;
		}

		index_middle = index_left + (index_right - index_left)/2;
		if(table[index_middle] < res)
		{
			index_right = index_middle;
		}
		else if(table[index_middle] > res)
		{
			index_left = index_middle;
		}
		else
		{
			return (index_middle) * 10;
		}
	}

	return -1;
}
float Serch_Harf_Pres(uint16_t pres,const float *table,uint16_t size)
{
	uint16_t index_left,index_right,index_middle;
	float temp;
	index_left = 0;
	index_right = size - 1;

	while(index_left < index_right)
	{
		if(index_right - index_left == 1)
		{
			temp = index_left+(pres - table[index_left])/(table[index_right] - table[index_left]);
			if(temp < 25.0f)
			{
				temp = 25.0f;
				return (temp - 25) * 10;
			}
			else
			{
				return (temp - 25) * 10;
			}
		}

		index_middle = index_left + (index_right - index_left)/2;
		if(table[index_middle] > pres)
		{
			index_right = index_middle;
		}
		else if(table[index_middle] < pres)
		{
			index_left = index_middle;
		}
		else
		{
			if(index_middle < 25)
			{
				index_middle = 25;
			}
			return (index_middle - 25) * 10;
		}
	}

	return -1;
}

uint16_t Get_Temp(ADC_LIST_E list)
{
	if (bsp_adc_volt_get(list) > 3196)
	{
		return 0;
	}

	if (bsp_adc_volt_get(list) < 108)
	{
		return 1650;
	}

	return Serch_Harf_Temp(bsp_adc_volt_get(list),BoardTempNTCTab,TEMP_NTC_TABLE_LEN);
}

uint16_t Get_Press(ADC_LIST_E list)
{
	uint16_t temp;

	temp = bsp_adc_volt_get(list) * 1.5;

	if(list == PRESS_INPUT_WATER|| list == PRESS_OUTPUT_WATER)
	{
		if(temp < 500)
		{
			return 0;
		}
		else if(temp > 4500)
		{
			return 100;
		}

		return ((temp - 500) * 100 / 4000);
	}
	else if(list == PRESS_IN_MOTOR)
	{
		if(temp < 500)
		{
			return 0;
		}
		else if(temp > 4500)
		{
			return 5000;
		}

		return ((temp - 500) * 2000 / 4000);
	}
	else
	{
		if(temp < 500)
		{
			return 0;
		}
		else if(temp > 4500)
		{
			return 5000;
		}

		return ((temp - 500) * 5000 / 4000);
	}

}

uint16_t app_get_temp_sensor(uint8_t index)
{
	return Get_Temp(index);
}

uint16_t app_get_pres_sensor(uint8_t index)
{
	return Get_Press(index);
}

uint16_t app_get_satura_temp(uint16_t pres)
{
	uint16_t pres_temp;

	pres_temp = pres;

	if(pres_temp < 53)
	{
		return 0;
	}
	if(pres_temp > 4600)
	{
		return 1090;
	}
	return Serch_Harf_Pres(pres_temp,Press_TempTab,PRESS_TABLE_LEN);
}

uint16_t app_get_over_heat(uint16_t temp,uint16_t pres)
{
	float temp1 = 0,temp2 = 0;
	float over_heat_temp;

	temp1 = temp - 400;
	temp2 = app_get_satura_temp(pres) - 400;

	over_heat_temp = temp1 - temp2 ;

	if(over_heat_temp < -400)
	{
		over_heat_temp = -400;
	}

	over_heat_temp += 400;

	return over_heat_temp;
}

uint16_t app_get_volt_24V(void)
{
	return bsp_adc_volt_get(power_24) * 11 + 500;
}

uint16_t app_get_volt_15V(void)
{
	return bsp_adc_volt_get(power_15) * 6;
}

uint16_t app_get_volt_12V(void)
{
	return bsp_adc_volt_get(power_12) * 4;
}

uint16_t app_get_ref(void)
{
    return bsp_adc_volt_get(adc_ref);
}

void app_temp_correct_input(temp_correct_e index,int16_t temp)
{
	if(temp > 100)
	{
		temp = 100;
	}

	if(temp < -100)
	{
		temp = -100;
	}

	switch (index)
	{
		case temp_in_water_idx:
			g_ttemp_correct.temp_in_water_correct = temp;
		break;

		case temp_out_water_idx:
			g_ttemp_correct.temp_out_water_correct = temp;
		break;

		case temp_envir_idx:
			g_ttemp_correct.temp_environment_correct = temp;
		break;

		case temp_in_motor_idx:
			g_ttemp_correct.temp_in_motor_correct = temp;
		break;

		case temp_out_motor_idx:
			g_ttemp_correct.temp_out_motor_correct = temp;
		break;
		
		default:
		break;
	}
}
int16_t app_temp_correct_get(temp_correct_e index)
{
	switch (index)
	{
		case temp_in_water_idx:
			return g_ttemp_correct.temp_in_water_correct;
		break;

		case temp_out_water_idx:
			return g_ttemp_correct.temp_out_water_correct;
		break;

		case temp_envir_idx:
			return g_ttemp_correct.temp_environment_correct;
		break;

		case temp_in_motor_idx:
			return g_ttemp_correct.temp_in_motor_correct;
		break;

		case temp_out_motor_idx:
			return g_ttemp_correct.temp_out_motor_correct;
		break;
		
		default:
			return 0;
		break;
	}
}

void app_get_sensor_handle(void)
{
	static uint16_t last_temp = 0;
	DataBase_Push(sys_in_water_temp,app_get_temp_sensor(TEMP_INPUT_WATER) + app_temp_correct_get(temp_in_water_idx));
	DataBase_Push(sys_out_water_temp,app_get_temp_sensor(TEMP_OUTPUT_WATER) + app_temp_correct_get(temp_out_water_idx));
	
	if(1 == g_tfault_in_value[environment_temp_sensor].value)
	{
		if(sys_ref == DataBase_Get(sys_mode_run) || sys_heat == DataBase_Get(sys_mode_run))
		{
			if(last_temp != 0)
			{
				DataBase_Push(sys_environment_temp,last_temp);
			}
			else
			{
				DataBase_Push(sys_environment_temp,750);
			}
		}
		else
		{
			DataBase_Push(sys_environment_temp,750);
		}
	}
	else
	{
		DataBase_Push(sys_environment_temp,app_get_temp_sensor(TEMP_ENVIRONMENT) + app_temp_correct_get(temp_envir_idx));
		last_temp = DataBase_Get(sys_environment_temp);
	}	

	
	DataBase_Push(sys_in_motor_temp,app_get_temp_sensor(TEMP_MOTOR_IN) + app_temp_correct_get(temp_in_motor_idx));
	DataBase_Push(sys_out_motor_temp,app_get_temp_sensor(TEMP_MOTOR_OUT) + app_temp_correct_get(temp_out_motor_idx));

	if(1 == g_tfault_in_value[in_water_pres_sensor].value)
	{
		DataBase_Push(sys_in_water_press,10);
	}
	else
	{
		DataBase_Push(sys_in_water_press,app_get_pres_sensor(PRESS_INPUT_WATER));
	}

	if(1 == g_tfault_in_value[out_water_pres_sensor].value)
	{
		DataBase_Push(sys_out_water_press,10);
	}
	else
	{
		DataBase_Push(sys_out_water_press,app_get_pres_sensor(PRESS_OUTPUT_WATER));
	}	

	DataBase_Push(sys_in_motor_press,app_get_pres_sensor(PRESS_IN_MOTOR) + 100);
	DataBase_Push(sys_out_motor_press,app_get_pres_sensor(PRESS_OUT_MOTOR) + 100);


	DataBase_Push(sys_over_heat,app_get_over_heat(DataBase_Get(sys_in_motor_temp),DataBase_Get(sys_in_motor_press)));
	DataBase_Push(sys_out_pres_trans_temp,app_get_satura_temp(DataBase_Get(sys_out_motor_press)));
	DataBase_Push(sys_in_pres_trans_temp,app_get_satura_temp(DataBase_Get(sys_in_motor_press)));

	DataBase_Push(sys_power_volt,app_get_volt_24V());
	DataBase_Push(sys_ipm_volt,app_get_volt_15V());
	DataBase_Push(sys_expan_volt,app_get_volt_12V());
	DataBase_Push(sys_adc_ref,app_get_ref());
	DataBase_Push(sys_ptc_state,app_get_ptc());
}
