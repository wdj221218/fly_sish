/*
 * app_mode_judge.h
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_MODE_JUDGE_H_
#define APP_APP_APP_INC_APP_MODE_JUDGE_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"
#include "sys.h"

#define ENTER 	SINGLE_ST1
#define EXIT  	SINGLE_ST2
#define NORMAL  SINGLE_ST3

typedef struct
{
	uint32_t enter_refrig           :	1;
	uint32_t exit_refrig            :	1;
	uint32_t enter_heat     		:	1;
	uint32_t exit_heat      		:	1;
	uint32_t self_cir_mode   		:	1;
	uint32_t refrig_mode			:	1;
	uint32_t heat_mode		    	:	1;
	uint32_t auto_mode				:	1;
	uint32_t power_on_mode   		:	1;
	uint32_t power_off_mode  		:	1;
	uint32_t test_mode      		:	1;
	uint32_t manual_mode 			:	1;
	uint32_t vacuum_mode			:	1;
	uint32_t rvd					:   19;
}event_s;

void start_mode_cb(void);

#endif /* APP_APP_APP_INC_APP_MODE_JUDGE_H_ */
