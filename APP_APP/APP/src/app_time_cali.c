/*
 * app_time_cali.c
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */
#include "app_time_cali.h"
time_cali_s g_time_cali;

void Time_init(void)
{
	g_time_cali.year = 2000;
	g_time_cali.mon = 0;
	g_time_cali.day = 0;
	g_time_cali.hour = 0;
	g_time_cali.minute = 0;
	g_time_cali.seconds = 0;
	time_cali_set(&g_time_cali);
}

void Time_set(void)
{
	g_time_cali.year = DataBase_Get(sys_year) + 2000;
	g_time_cali.mon = DataBase_Get(sys_mon);
	g_time_cali.day = DataBase_Get(sys_day);
	g_time_cali.hour = DataBase_Get(sys_hour);
	g_time_cali.minute = DataBase_Get(sys_min);
	g_time_cali.seconds  =DataBase_Get(sys_sec);
	time_cali_set(&g_time_cali);
}

void Time_updata(void)
{
	DataBase_Push(sys_year,(g_time_cali.year < 2000 ? 2000 : g_time_cali.year) - 2000);
	DataBase_Push(sys_mon,g_time_cali.mon);
	DataBase_Push(sys_day,g_time_cali.day);
	DataBase_Push(sys_hour,g_time_cali.hour);
	DataBase_Push(sys_min,g_time_cali.minute);
	DataBase_Push(sys_sec,g_time_cali.seconds);
}

void Time_Calibration(void)
{
	time_cali_server();
	time_cali_get(&g_time_cali);
	Time_updata();
}

