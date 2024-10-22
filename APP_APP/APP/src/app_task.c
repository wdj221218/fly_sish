/*
 * app_task.c
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */
#include "app_task.h"

TaskList tTaskList[SUPPORT_TASK_NUM];
uint8_t flag_start = 0;

void TaskListInit(void)
{
	tTaskList[TASK_2MS_ID].Period = 2;
	tTaskList[TASK_2MS_ID].RunMinPeriod = 1;
	tTaskList[TASK_2MS_ID].Offset = 0;
	tTaskList[TASK_2MS_ID].TaskCnt = 0;
	tTaskList[TASK_2MS_ID].RunFlag = 0;
	tTaskList[TASK_2MS_ID].RunLastTick = 0;

	tTaskList[TASK_10MS_ID].Period = 10;
	tTaskList[TASK_10MS_ID].RunMinPeriod = 5;
	tTaskList[TASK_10MS_ID].Offset = 0;
	tTaskList[TASK_10MS_ID].TaskCnt = 0;
	tTaskList[TASK_10MS_ID].RunFlag = 0;
	tTaskList[TASK_10MS_ID].RunLastTick = 0;

	tTaskList[TASK_20MS_ID].Period = 20;
	tTaskList[TASK_20MS_ID].RunMinPeriod = 10;
	tTaskList[TASK_20MS_ID].Offset = 0;
	tTaskList[TASK_20MS_ID].TaskCnt = 0;
	tTaskList[TASK_20MS_ID].RunFlag = 0;
	tTaskList[TASK_20MS_ID].RunLastTick = 0;

	tTaskList[TASK_50MS_ID].Period = 50;
	tTaskList[TASK_50MS_ID].RunMinPeriod = 25;
	tTaskList[TASK_50MS_ID].Offset = 0;
	tTaskList[TASK_50MS_ID].TaskCnt = 0;
	tTaskList[TASK_50MS_ID].RunFlag = 0;
	tTaskList[TASK_50MS_ID].RunLastTick = 0;

	tTaskList[TASK_500MS_ID].Period = 500;
	tTaskList[TASK_500MS_ID].RunMinPeriod = 500;
	tTaskList[TASK_500MS_ID].Offset = 0;
	tTaskList[TASK_500MS_ID].TaskCnt = 0;
	tTaskList[TASK_500MS_ID].RunFlag = 0;
	tTaskList[TASK_500MS_ID].RunLastTick = 0;

	tTaskList[TASK_1000MS_ID].Period = 1000;
	tTaskList[TASK_1000MS_ID].RunMinPeriod = 500;
	tTaskList[TASK_1000MS_ID].Offset = 0;
	tTaskList[TASK_1000MS_ID].TaskCnt = 0;
	tTaskList[TASK_1000MS_ID].RunFlag = 0;
	tTaskList[TASK_1000MS_ID].RunLastTick = 0;

	tTaskList[TASK_2000MS_ID].Period = 2000;
	tTaskList[TASK_2000MS_ID].RunMinPeriod = 1000;
	tTaskList[TASK_2000MS_ID].Offset = 0;
	tTaskList[TASK_2000MS_ID].TaskCnt = 0;
	tTaskList[TASK_2000MS_ID].RunFlag = 0;
	tTaskList[TASK_2000MS_ID].RunLastTick = 0;

	tTaskList[TASK_20000MS_ID].Period = 20000;
	tTaskList[TASK_20000MS_ID].RunMinPeriod = 10000;
	tTaskList[TASK_20000MS_ID].Offset = 0;
	tTaskList[TASK_20000MS_ID].TaskCnt = 0;
	tTaskList[TASK_20000MS_ID].RunFlag = 0;
	tTaskList[TASK_20000MS_ID].RunLastTick = 0;
}

void TaskRun1msIsrCallback(void)
{
	uint8_t i = 0;
	for(i = 0;i < SUPPORT_TASK_NUM;i++)
	{
		tTaskList[i].TaskCnt++;
		if(tTaskList[i].TaskCnt >= (tTaskList[i].Period + tTaskList[i].Offset))
		{
			tTaskList[i].RunFlag++;
			if(tTaskList[i].RunFlag > 1)
			{
				tTaskList[i].RunFlag = 1;
			}
			tTaskList[i].TaskCnt = tTaskList[i].Offset;
		}
	}
}

uint8_t TaskActive(uint8_t task_id)
{
	uint8_t res = 0;

	if(task_id < SUPPORT_TASK_NUM)
	{
		__set_PRIMASK(1); //关闭全局中断
		if(tTaskList[task_id].RunFlag > 0)
		{
			if(get_elapsed_tick_count(tTaskList[task_id].RunLastTick) >= tTaskList[task_id].RunMinPeriod)
			{
				tTaskList[task_id].RunLastTick = get_tick_count();
				tTaskList[task_id].RunFlag--;
				res = 1;
			}
		}
		__set_PRIMASK(0); //使能全局中断
	}
	return res;
}

void Task2msRun(void)
{
	if (TaskActive(TASK_2MS_ID) == 1)
	{
		ADC_Handle();
		app_get_sensor_handle();
	}
}

void Task10msRun(void)
{
	if (TaskActive(TASK_10MS_ID) == 1)
	{
		app_mode_judge_handle();
		app_unit_run();
		mid_log_print_server();
	}
}

void Task20msRun(void)
{
	if (TaskActive(TASK_20MS_ID) == 1)
	{
		app_wdt_feed();
		Expan_Open_Init();
		Expansion_State_Machine(DataBase_Get(sys_expan_open_temp));
	}
}

void Task50msRun(void)
{
	if (TaskActive(TASK_50MS_ID) == 1)
	{
		app_fault_diag_handle();
	}
}

void Task500msRun(void)
{
	if (TaskActive(TASK_500MS_ID) == 1)
	{
		modbus_master_send_handle();
	}
}

void Task1000msRun(void)
{
	static uint8_t i = 0;
	if (TaskActive(TASK_1000MS_ID) == 1)
	{
		Time_Calibration();
		app_get_fan_speed();
		app_ptc_run_time();
		app_motor_run_time();
		DataBase_Down();
		app_belt_control();
	}
}

void Task2000msRun(void)
{
	if (TaskActive(TASK_2000MS_ID) == 1)
	{

	}
}

void Task_Schedule(void)
{
	Task2msRun();
	Task10msRun();
	Task20msRun();
	Task50msRun();
	Task500msRun();
	Task1000msRun();
	Task2000msRun();
}

