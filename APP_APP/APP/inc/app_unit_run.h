/*
 * app_unit_run.h
 *
 *  Created on: 2024年3月25日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_UNIT_RUN_H_
#define APP_APP_APP_INC_APP_UNIT_RUN_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"


typedef enum
{
	unit_mode_power_off,
	unit_mode_ref,
	unit_mode_heat,
	unit_mode_cir,
}mods_unit_mode_e;

typedef enum
{
	unit_run_idle,
	unit_run_wait_start,
	unit_run_run,
	unit_run_wait_stop,
	unit_run_fault,
}mods_unit_run_e;

typedef struct
{
	uint16_t unit_run_state			 		:	8;
	uint16_t unit_mode_state 				:	8;
}unit_run_state_s;

typedef struct
{
	uint16_t rvd1			 				:	3;
	uint16_t relay_fan		 				:	1;
	uint16_t relay_pump		 				:	1;
	uint16_t relay_ptc	 					:	1;
	uint16_t rvd2							:   1;
	uint16_t relay_belt	 					:	1;
	uint16_t rvd3			 				:	8;
}relay_out_state_s;

typedef struct
{
	uint16_t rvd1			 				:	3;
	uint16_t relay_fan		 				:	1;
	uint16_t relay_pump		 				:	1;
	uint16_t relay_ptc	 					:	1;
	uint16_t relay_belt	 					:	1;
	uint16_t rvd2			 				:	9;
}relay_out_state_pc_s;

#endif /* APP_APP_APP_INC_APP_UNIT_RUN_H_ */
