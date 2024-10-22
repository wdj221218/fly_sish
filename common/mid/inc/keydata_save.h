/*
 * app_keydata_save.h
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */

#ifndef COMMON_MID_INC_KEYDATA_SAVE_H_
#define COMMON_MID_INC_KEYDATA_SAVE_H_

#include "stdio.h"
#include "bsp.h"
#include "mid.h"
//#include "app.h"

#define NUM_MAX   64
#define DATA_LEN  9

#define YEAR 	0
#define MON 	1
#define DAY 	2
#define HOUR 	3
#define MIN 	4
#define SEC 	5
#define REASON 	6
#define TYPE 	7
#define CNT 	8

typedef enum
{
	fault_con,
	fault_rem,
}tigger_type_list;

typedef enum
{
	fault_power_control_board = 0,
	fault_power_expan,
	fault_adcref_control_board,
	fault_communication_motor,
	fault_in_watar_temp_sensor,
	fault_out_water_temp_sensor,
	fault_out_motor_temp_sensor,
	fault_in_motor_temp_sensor,
	fault_environment_temp_sensor,
	fault_in_water_pres_sensor,
	fault_out_water_pres_sensor,
	fault_out_motor_pres_sensor,
	fault_in_motor_pres_sensor,
	fault_out_motor_pres_high,
	fault_in_motor_pres_low,
	fault_out_motor_temp_high,
	fault_in_water_pres_low,
	fault_out_water_pres_high,
	fault_out_water_temp_low,
	fault_out_water_temp_high,
	fault_fan1,
	fault_fan2,
	fault_fan3,
	fault_compre_drive,
	fault_pump,
	fault_ptc,
	fault_communication_bms,
	fault_envit_temp_over,
}tigger_reason_list;

typedef struct
{
	uint8_t  write_index;
	uint8_t  read_index;
	uint16_t key_data_buff[NUM_MAX][DATA_LEN];
}key_data_str;
void key_data_get(uint8_t cnt,uint8_t *buff);
void key_data_init(void);
#endif /* COMMON_MID_INC_KEYDATA_SAVE_H_ */
