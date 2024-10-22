/*
 * modbus_slave.h
 *
 *  Created on: 2024年3月8日
 *      Author: Dell
 */

#ifndef COMMON_MID_INC_MODBUS_SLAVE_H_
#define COMMON_MID_INC_MODBUS_SLAVE_H_

#include <stdint.h>
#include "bsp.h"
#include "mid.h"
#include "app.h"
#include "UserLib.h"


#define READ       1
#define WRITE      2

#define MODBUS_ERR 0
#define MODBUS_OK  1

typedef enum
{
    Normal = 0,
    Err_Cmd ,
    Err_Reg_Addr,
    Err_Value, 
    Err_Write,
    Err_Rec,
}ModBus_ErrCode;

typedef enum
{
    ModBus_03H = 0x03,
    ModBus_06H = 0x06,
    ModBus_10H = 0x10, 
}ModBus_ID;

typedef enum
{
	trans_null = 0,
	trans_add,
	trans_sub,
	trans_mult,
	trans_divi,
	trans_mult_10_add,//先乘后加
	trans_sub_divi_10,//先减后除
}trans_cmd_e;

typedef struct 
{
	uint16_t 	Address;//地址
    uint8_t 	limit;//权限
    uint16_t 	value;//值
	uint16_t 	defmax;//最大值
	uint16_t 	defmin;//最小值
    uint16_t 	write_flag;
    uint8_t	 	(*Write)(uint16_t value);
    trans_cmd_e	in_trans_cmd;
	uint16_t 	in_trans_value;
	trans_cmd_e out_trans_cmd;
	uint16_t 	out_trans_value;
	void	 	(*Data_Pro)(void);
}ModBus_Address;

typedef enum
{
//0x0000
	mods_unit_state,
	mods_rvd_0001,
	mods_relay_out,
	mods_out_water_temp,
	mods_in_water_temp,
	mods_rvd_0005,
	mods_rvd_0006,
	mods_rvd_0007,
	mods_envir_temp,
	mods_rvd_0009,
	mods_expan_open,
	mods_pump_duty,
	mods_motor_speed,
	mods_fan_duty,
	mods_out_water_pres,
	mods_in_water_pres,
	mods_out_motor_pres,
	mods_fault_code1,
	mods_fault_code2,
	mods_fault_code3,
	mods_rvd_0001C,
	mods_rvd_0001D,
	mods_rvd_0001E,
	mods_rvd_0001F,
	mods_rvd_00020,
	mods_fault_level,
	mods_fault_sum,
	mods_rvd_00024,
	mods_rvd_00025,
	mods_rvd_00026,
	mods_motor_power,
	mods_rvd_003D,
	mods_rvd_003E,
	mods_ptc_run_time,
	mods_rvd_0040,
	mods_motor_run_time,
	mods_rvd_0042,
	mods_rvd_0043,
	mods_rvd_0044,
	mods_motor_volt,

//0x1000
	mods_start_mode,
	mods_mode_set,
	mods_ctl_temp_set,
	mods_ref_temp,
	mods_ref_backlash,
	mods_heat_temp,
	mods_heat_backlash,
	mods_rvd_1007,
	mods_rvd_1008,
	mods_alarm_out_water_temp_high,
	mods_alarm_out_water_temp_low,
	mods_alarm_in_water_pres_low,
	mods_alarm_out_water_pres_high,
	mods_rvd_100D,
	mods_rvd_100E,
	mods_rvd_100F,
	mods_address,
	mods_baud,
	mods_bms_temp_max,
	mods_bms_temp_min,
	mods_bms_temp_avr,
	mods_bms_state,
	mods_self_cir_start_dev,
	mods_self_cir_stop_dev,
	mods_year,
	mods_mon,
	mods_day,
	mods_hour,
	mods_min,
	mods_sec,
    ModBus_Register_End,
}ModBus_Register;


void modbus_cmd_analyze(uint8_t *pBuffer,uint8_t len);
void bms_community_cb(void);
#endif /* COMMON_MID_INC_MODBUS_SLAVE_H_ */

