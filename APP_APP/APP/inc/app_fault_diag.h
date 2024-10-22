/*
 * app_fault_diag.h
 *
 *  Created on: 2024年3月22日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_FAULT_DIAG_H_
#define APP_APP_APP_INC_APP_FAULT_DIAG_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"
#include "sys.h"

#define NORMAL_STATUS  	   SINGLE_ST1
#define ERROR_STATUS	   SINGLE_ST2

#define CHECK_NUM          2

typedef enum
{
	fault_level_1 = 1,
	fault_level_2,
	fault_level_3,
}faule_level_list;

typedef enum
{
	power_control_board = 0,
	power_expan,
	adcref_control_board,
	communication_motor,
	in_watar_temp_sensor,
	out_water_temp_sensor,
	out_motor_temp_sensor,
	in_motor_temp_sensor,
	environment_temp_sensor,
	in_water_pres_sensor,
	out_water_pres_sensor,
	out_motor_pres_sensor,
	in_motor_pres_sensor,
	out_motor_pres_high,
	in_motor_pres_low,
	out_motor_temp_high,
	in_water_pres_low,
	out_water_pres_high,
	out_water_temp_low,
	out_water_temp_high,
	fan1,
	fan2,
	fan3,
	compre_drive,
	pump,
	ptc,
	communication_bms,
	envit_temp_over,
	fault_diag_max,
}fault_diag_list;

typedef enum
{
	idx_power_control_board_check = 0,
	idx_power_expan_check,
	idx_adc_ref_check,
	idx_temp_sensor_check,
	idx_pres_sensor_check,
	idx_in_motor_pres_low_check,
	idx_out_motor_pres_high_check,
	idx_out_motor_temp_high_check,
	idx_fan_check,
	idx_compre_fre_check,
	idx_in_water_pres_low_check,
	idx_out_water_pres_high_check,
	idx_out_water_temp_low_check,
	idx_out_water_temp_high_check,
	idx_pump_check,
	idx_ptc_check,
	idx_motor_community_check,
	idx_host_community_check,
	idx_envit_temp_over_check,
	faule_check_list_max,
}fault_check_list;

typedef struct
{
	uint8_t idle_fault			 		:	1;  
	uint8_t lock_fault	 				:	1;
	uint8_t phase_or_over_current		:	1;
	uint8_t over_temp_fault			 	:	1;
	uint8_t power_fault 				:	1;
	uint8_t rvd   						:	3;
}pump_fault_out_s;

typedef struct
{
	void (*fault_check)(void);
	uint32_t period;
	uint32_t def_period;
	uint32_t sys_tick;
	uint8_t fault_en;
	uint8_t def_fault_en;
}fault_check_s;

typedef struct
{
	uint32_t power_control_board 		:	1;  
	uint32_t power_expan 				:	1;
	uint32_t adcref_control_board 		:	1;
	uint32_t communication_motor 		:	1;
	uint32_t in_watar_temp_sensor 		:	1;
	uint32_t out_water_temp_sensor 		:	1;
	uint32_t out_motor_temp_sensor 		:	1;
	uint32_t in_motor_temp_sensor		:	1;
	uint32_t environment_temp_sensor 	:	1;
	uint32_t in_water_pres_sensor 		:	1;
	uint32_t out_water_pres_sensor 		:	1;
	uint32_t out_motor_pres_sensor 		:	1;
	uint32_t in_motor_pres_sensor 		:	1;
	uint32_t out_motor_pres_high 		:	1;
	uint32_t in_motor_pres_low 			:	1;
	uint32_t out_motor_temp_high 		:	1;
	uint32_t in_water_pres_low 			:	1;
	uint32_t out_water_pres_high 		:	1;
	uint32_t out_water_temp_low 		:	1;
	uint32_t out_water_temp_high 		:	1;
	uint32_t fan1 						:	1;
	uint32_t fan2 						:	1;
	uint32_t fan3 						:	1;
	uint32_t compre_drive 				:	1;
	uint32_t pump 						:	1;
	uint32_t ptc 						:	1;
	uint32_t communication_bms 			:	1;
	uint32_t envit_temp_over            :   1;
	uint32_t rvd   						:	4;
}fault_out_s;

typedef struct
{
	uint16_t pump 						:	1;  
	uint16_t rvd1 						:	9;
	uint16_t in_water_pres_low			:	1;
	uint16_t out_water_pres_high		:	1;
	uint16_t rvd2 						:	2;
	uint16_t power_control_board 		:	1;  
	uint16_t power_expan 				:	1;
}fault_code1_s;

typedef struct
{
	uint16_t in_motor_pres_low 			:	1;  
	uint16_t ptc 						:	1;
	uint16_t fan				 		:	1;
	uint16_t out_motor_pres_high		:	1;
	uint16_t rvd1 						:	3;
	uint16_t out_motor_pres_sensor 		:	1;
	uint16_t communication_motor 		:	1;
	uint16_t adcref_control_board 		:	1;
	uint16_t out_motor_temp_sensor 		:	1;
	uint16_t in_motor_temp_sensor		:	1;
	uint16_t in_water_pres_sensor 		:	1;
	uint16_t out_water_pres_sensor 		:	1;
	uint16_t in_motor_pres_sensor 		:	1;
	uint16_t out_motor_temp_high 		:	1;
}fault_code2_s;

typedef struct
{
	uint16_t out_water_temp_sensor 		:	1;  
	uint16_t in_watar_temp_sensor 		:	1;
	uint16_t environment_temp_sensor 	:	1;
	uint16_t out_water_temp_high 		:	1;
	uint16_t in_water_pres_sensor 		:	1;
	uint16_t rvd1				 		:	1;
	uint16_t out_water_temp_low 		:	1;
	uint16_t rvd2 						:	3;
	uint16_t compre_drive				:	1;
	uint16_t communication_bms 			:	1;
	uint16_t envit_temp_over 			:	1;
	uint16_t rvd3 						:	3;
}fault_code3_s;

typedef struct
{
	uint16_t hard_fault			 		:	1;
	uint16_t ref_fault	 				:	1;
	uint16_t water_fault				:	1;
	uint16_t heat_fault					:	1;
	uint16_t community_fault	 		:	1;
	uint16_t sensor_fault				:	1;
	uint16_t rvd   						:	10;
}fault_sum_s;


typedef struct
{
	uint8_t 	value;
	uint16_t	num; 
	uint32_t    timer[3];
}fault_in_value_s;

typedef struct
{
	uint8_t 	value;
	uint16_t	num; 
}fault_out_value_s;

typedef enum
{
	sensor_normal = 0,//均正常
	in_water_fault,//进水温度传感器故障
	out_water_fault,//出水温度传感器故障
	sensor_fault,//均故障
}temp_sensor_fault_e;

void power_control_board_check_in();
void power_ipm_check_in();
void power_expan_check_in();
void adc_ref_check_in();
void temp_sensor_check_in();
void pres_sensor_check_in();
void in_motor_pres_low_check_in();
void out_motor_pres_high_check_in();
void out_motor_temp_high_check_in();
void fan_check_in();
void compre_fre_check_in();
void in_water_pres_low_check_in();
void out_water_pres_high_check_in();
void out_water_temp_low_check_in();
void out_water_temp_high_check_in();
void pump_check_in();
void ptc_check_in();
void motor_community_check_in();
void host_community_check_in();
void envit_temp_over_check_in();

void power_control_board_check_out();
void power_ipm_check_out();
void power_expan_check_out();
void adc_ref_check_out();
void temp_sensor_check_out();
void pres_sensor_check_out();
void in_motor_pres_low_check_out();
void out_motor_pres_high_check_out();
void out_motor_temp_high_check_out();
void fan_check_out();
void compre_fre_check_out();
void in_water_pres_low_check_out();
void out_water_pres_high_check_out();
void out_water_temp_low_check_out();
void out_water_temp_high_check_out();
void pump_check_out();
void ptc_check_out();
void motor_community_check_out();
void host_community_check_out();
void envit_temp_over_check_out();

void app_fault_reset(fault_diag_list index);
void app_fault_shield(void);

#endif /* APP_APP_APP_INC_APP_FAULT_DIAG_H_ */
