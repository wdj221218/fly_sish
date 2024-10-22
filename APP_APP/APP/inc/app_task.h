/*
 * app_task.h
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_TASK_H_
#define APP_APP_APP_INC_APP_TASK_H_

#include <stdint.h>
#include "bsp.h"
#include "mid.h"
#include "app.h"

/*
#define SUPPORT_TASK_NUM  8

#define TASK_2MS_ID       0
#define TASK_10MS_ID      1
#define TASK_20MS_ID      2
#define TASK_50MS_ID      3
#define TASK_500MS_ID     4
#define TASK_1000MS_ID    5
#define TASK_2000MS_ID    6
#define TASK_20000MS_ID   7
*/

typedef enum
{
 TASK_2MS_ID = 0,
 TASK_10MS_ID,
 TASK_20MS_ID,
 TASK_50MS_ID,
 TASK_500MS_ID,
 TASK_1000MS_ID,
 TASK_2000MS_ID,
 TASK_20000MS_ID,
 SUPPORT_TASK_NUM,
}e_task_list;

typedef struct
{
	uint16_t Period;
	uint16_t RunMinPeriod;
	uint16_t Offset;
	uint32_t RunLastTick;
	volatile uint16_t TaskCnt;
	volatile uint8_t  RunFlag;
}TaskList;

void TaskListInit(void);
void TaskRun1msIsrCallback(void);
void Task2msRun(void);
void Task10msRun(void);
void Task20msRun(void);
void Task50msRun(void);
void Task200msRun(void);
void Task1000msRun(void);
uint8_t TaskActive(uint8_t task_id);

#endif /* APP_APP_APP_INC_APP_TASK_H_ */
