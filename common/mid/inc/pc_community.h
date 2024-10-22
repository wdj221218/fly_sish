/*
 * pc_community.h
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */

#ifndef COMMON_MID_INC_PC_COMMUNITY_H_
#define COMMON_MID_INC_PC_COMMUNITY_H_

#include <stdint.h>
#include "bsp.h"
#include "mid.h"
#include "app.h"
#include "UserLib.h"


#define HOST_PROTOCOL_HEAD 		0x68
#define HOST_PROTOCOL_TAIL 		0x16

#define PACK_SIZE               1024

#define REAL_DATA               0x10
#define PARAM_WRITE             0x11
#define PARAM_READ              0x12
#define SN_WRITE                0x13
#define KEY_DATA_GET            0x14
#define TEMP_CORRECT			0x15
#define PARAM_RESET             0x16
#define SN_READ            		0x17
#define TIME_CALI               0x18
#define MANUAL_MODE				0x19
#define VACUUM_MODE             0x20
#define TEST_MODE            	0x21
#define FAULT_RESET            	0x22
#define POWER_SET            	0x23
#define UNIT_MODE_SET           0x24
#define PUMP_FAULT				0x25
#define MOTOR_FAULT				0x26
#define SELF_CHECK_SET		    0x27
#define SELF_CHECK_CTL			0x28
#define FAULT_EN_CTL			0x29
#define RUN_TIME_CNT_CLEAN      0x2A

#pragma pack (1)
typedef struct
{
  uint8_t  head;
  uint8_t  addr;
  uint8_t  cmd;
  uint16_t size;
  uint16_t sys_board_volt;
  uint16_t sys_ipm_volt;
  uint16_t sys_expan_volt;
  uint16_t sys_adc_ref;
  uint16_t sys_temp_in_motor;
  uint16_t sys_temp_out_motor;
  uint16_t sys_temp_in_motor_pres;
  uint16_t sys_temp_out_motor_pres;
  uint16_t sys_expan_open;
  uint16_t sys_over_heat_point;
  uint16_t sys_over_heat_now;
  int16_t  sys_over_heat_rate;
  int16_t  sys_temp_rate;
  uint16_t sys_motor_run_time;
  uint16_t sys_motor_run_cnt;
  uint16_t sys_motor_power;
  uint16_t sys_ptc_run_time;
  uint16_t sys_ptc_run_cnt;
  uint16_t sys_app_version;
  uint16_t sys_sbl_version;
  uint16_t sys_bms_temp_max;
  uint16_t sys_bms_temp_min;
  uint16_t sys_bms_temp_avr;
  uint8_t  sys_bms_state;
  uint8_t  sys_unit_state;
  uint8_t  sys_unit_mode;
  uint8_t  sys_ctl_temp_set;
  uint8_t  sys_motor_fre;
  uint8_t  sys_fan1_level;
  uint8_t  sys_fan1_duty;
  uint8_t  sys_fan2_level;
  uint8_t  sys_fan2_duty;
  uint8_t  sys_fan3_level;
  uint8_t  sys_fan3_duty;
  uint8_t  sys_pump_level;
  uint8_t  sys_pump_duty;
  uint8_t  ptc_state;
  uint16_t sys_ref_temp;
  uint16_t sys_ref_backlash;
  uint16_t sys_heat_temp;
  uint16_t sys_heat_backlash;
  uint16_t sys_evvir_temp;
  uint16_t sys_temp_out_water;
  uint16_t sys_temp_in_water;
  uint16_t sys_pres_out_water;
  uint16_t sys_pres_in_water;
  uint16_t sys_pres_out_motor;
  uint16_t sys_pres_in_motor;
  uint16_t rvd2;
  uint32_t sys_fault_code;
  uint8_t  sys_year;
  uint8_t  sys_mon;
  uint8_t  sys_day;
  uint8_t  sys_hour;
  uint8_t  sys_min;
  uint8_t  sys_sec;
  uint16_t sys_motor_volt;
  uint8_t  sys_belt_state;
  uint8_t  rvd1;
  int16_t  sys_temp_err_now;
  int16_t  sys_temp_err_last;
  uint16_t sys_over_heat_last;
  uint16_t sys_unit_run_state;
  uint16_t sys_relay_out;
  uint16_t sys_fault_level;
  uint16_t sys_fault_code_sum;
  uint16_t sys_valve_state;
  uint16_t sys_compre_fre_point;
}real_time_data_read_s;

typedef struct
{
	uint8_t  head;
	uint8_t  addr;
	uint8_t  cmd;
	uint16_t size;
	uint16_t sys_rs485_bd;
	uint16_t sys_rs485_add;
	uint16_t sys_self_cir_start_dev;
	uint16_t sys_self_cir_stop_dev;
	uint16_t sys_out_water_ctl_ref_temp;
	uint16_t sys_out_water_ctl_heat_temp;
	uint16_t sys_in_water_ctl_ref_temp;
	uint16_t sys_in_water_ctl_heat_temp;
	uint16_t sys_bms_temp_ctl_ref_temp;
	uint16_t sys_bms_temp_ctl_heat_temp;
	uint16_t sys_ref_backlash;
	uint16_t sys_heat_backlash;
	uint16_t sys_pump_high_duty;
	uint16_t sys_pump_middle_duty;
	uint16_t sys_pump_low_duty;
	uint16_t sys_fan_high_duty;
	uint16_t sys_fan_middle_duty;
	uint16_t sys_fan_low_duty;
	uint16_t sys_fan_inc_temp;
	uint16_t sys_fan_dec_temp;
	uint16_t sys_high_pres_protect_temp;
	uint16_t sys_low_pres_protect_temp;
	uint16_t sys_out_motor_high_temp_protect;
	uint16_t sys_out_water_low_pres_protect;
	uint16_t sys_out_water_high_pres_protect;
	uint16_t sys_expan_init_open_1;
	uint16_t sys_expan_init_open_2;
	uint16_t sys_expan_init_open_3;
	uint16_t sys_expan_init_open_4;
	uint16_t sys_expan_init_open_5;
	uint16_t sys_over_heat_point1;
	uint16_t sys_over_heat_point2;
	uint16_t sys_over_heat_point3;
	uint16_t sys_over_heat_point4;
	uint16_t sys_over_heat_point5;
	uint16_t sys_expan_min_open;
	uint16_t sys_expan_ctl_interval;
	uint16_t sys_expan_init_hold_time;
	uint16_t sys_unit_interval;
	uint16_t sys_motor_start_interval;
	uint16_t sys_ref_min_temp;
	uint16_t sys_ref_max_temp;
	uint16_t sys_motor_fre_limit1;
	uint16_t sys_motor_fre_limit2;
	uint16_t sys_motor_fre_limit3;
	uint16_t sys_motor_fre_limit4;
	uint16_t sys_motor_fre_limit5;
	uint16_t sys_motor_fre_limit6;
	uint16_t sys_motor_fre_max;
	uint16_t sys_motor_fre_min;
	uint16_t sys_test_hold_time;
	uint16_t sys_ctl_temp_set;
	uint16_t sys_out_water_temp_high;
	uint16_t sys_out_water_temp_low;
	uint16_t sys_start_mode;
	uint16_t sys_unit_select;
	uint16_t sys_valve_start_temp;
	uint16_t sys_compre_start_max;
}param_write_read_s;

typedef struct
{
	uint8_t  head;
	uint8_t  addr;
	uint8_t  cmd;
	uint16_t size;
	int8_t   sys_sn[16];
}sn_write_read_s;

typedef struct
{
	uint8_t  head;
	uint8_t  addr;
	uint8_t  cmd;
	uint16_t size;
	uint8_t  fault_year;
	uint8_t  fault_mon;
	uint8_t  fault_day;
	uint8_t  fault_hour;
	uint8_t  fault_min;
	uint8_t  fault_sec;
	uint8_t  data_type;
	uint8_t  data_reason;
	uint16_t fault_cnt;
	uint8_t  rvd1;
	uint8_t  rvd2;
}key_data_read_s;

typedef struct
{
	uint8_t  head;
	uint8_t  addr;
	uint8_t  cmd;
	uint16_t size;
	uint8_t  sys_year;
	uint8_t  sys_mon;
	uint8_t  sys_day;
	uint8_t  sys_hour;
	uint8_t  sys_min;
	uint8_t  sys_sec;
	uint8_t  rvd1;
	uint8_t  rvd2;
}tim_cali_s;

typedef struct
{
	uint8_t 	head;
	uint8_t 	addr;
	uint8_t 	cmd;
	uint16_t 	size;
	uint8_t 	sys_manual_set;
	uint8_t 	sys_manual_pump;
	uint8_t 	sys_manual_fan;
	uint8_t 	sys_manual_ptc;
	uint16_t 	sys_manual_expan;
	uint8_t 	sys_manual_motor;
	uint8_t 	sys_manual_belt;
	uint8_t 	sys_manual_value;
	uint8_t 	rvd1;
	uint16_t 	rvd2;
}manual_set_s;

typedef struct
{
	uint8_t 	head;
	uint8_t 	addr;
	uint8_t 	cmd;
	uint16_t 	size;
	uint16_t 	sys_in_water_temp;
	uint16_t 	sys_out_water_temp;
	uint16_t 	sys_envir_temp;
	uint16_t 	sys_out_motor_temp;
	uint16_t 	sys_in_motor_temp;
	uint16_t 	rvd;
}real_temp_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
}param_reset_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t sys_vacuum_set;
}vacuum_set_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t sys_test_set;
	uint8_t sys_test_mode_set;
	uint8_t rvd1;
	uint8_t rvd2;
}test_set_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
}fault_reset_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t power_set;
}power_set_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t run_mode_set;
}unit_mode_set_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t pump_fault;
}get_pump_fault_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint16_t motor_fault1;
	uint16_t motor_fault2;
}get_motor_fault_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t set;
}self_check_set_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t pump;
	uint8_t fan;
	uint8_t ptc;
	uint8_t expan;
	uint8_t compres;
	uint8_t belt;
	uint8_t valve;
	uint8_t rvd;
}self_check_ctl_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
	uint8_t en_ctl;
}fault_en_ctl_s;

typedef struct
{
	uint8_t head;
	uint8_t addr;
	uint8_t cmd;
	uint16_t size;
}time_cnt_clean_s;

#pragma pack ()

typedef struct
{
	uint16_t packnum;
	uint8_t  recvbuff[2048];
	uint32_t recvlen;
	uint32_t flashaddr;
	uint16_t totalpacknum;
	uint16_t  progress;
	uint16_t  progress_last;
}upgradeinfo;

void sys_reset(void);
void pc_cmd_analyze(uint8_t *buff,uint16_t len);

#endif /* COMMON_MID_INC_PC_COMMUNITY_H_ */
