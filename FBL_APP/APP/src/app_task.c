/*
 * app_task.c
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */


#include "app_task.h"

TaskList tTaskList[SUPPORT_TASK_NUM];


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

	tTaskList[TASK_200MS_ID].Period = 200;
	tTaskList[TASK_200MS_ID].RunMinPeriod = 100;
	tTaskList[TASK_200MS_ID].Offset = 0;
	tTaskList[TASK_200MS_ID].TaskCnt = 0;
	tTaskList[TASK_200MS_ID].RunFlag = 0;
	tTaskList[TASK_200MS_ID].RunLastTick = 0;

	tTaskList[TASK_1000MS_ID].Period = 1000;
	tTaskList[TASK_1000MS_ID].RunMinPeriod = 500;
	tTaskList[TASK_1000MS_ID].Offset = 0;
	tTaskList[TASK_1000MS_ID].TaskCnt = 0;
	tTaskList[TASK_1000MS_ID].RunFlag = 0;
	tTaskList[TASK_1000MS_ID].RunLastTick = 0;
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

	}
}

void Task10msRun(void)
{
	if (TaskActive(TASK_10MS_ID) == 1)
	{

	}
}

void Task20msRun(void)
{
	if (TaskActive(TASK_20MS_ID) == 1)
	{

	}
}

void Task50msRun(void)
{
	if (TaskActive(TASK_50MS_ID) == 1)
	{

	}
}

void Task200msRun(void)
{
	if (TaskActive(TASK_200MS_ID) == 1)
	{

	}
}

void Task1000msRun(void)
{
	if (TaskActive(TASK_1000MS_ID) == 1)
	{

	}
}


