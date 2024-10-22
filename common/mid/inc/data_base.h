/*
 * app_database.h
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_DATA_BASE_H_
#define APP_APP_APP_INC_DATA_BASE_H_

#include <stdint.h>

#include "bsp.h"
#include "mid.h"
#include "app.h"


#define WATERCYCLE_CONDITION   1
#define HEAT_CONDITION         1
#define REFRIGERRATE_CONDITION 1

#define SINGLE_WATER		   0
#define DOUBLE_WATER		   1

typedef enum
{
	sys_other = 0,
	sys_self_cir,
	sys_ref,
	sys_heat,
}sys_mode_run_e;

typedef enum
{
	sys_auto_mode = 0,
	sys_self_cir_mode,
	sys_ref_mode,
	sys_heat_mode,
	sys_manual_mode,
	sys_test_mode,
	sys_vacumm_mode,
	sys_power_off,
}sys_mode_e;

typedef enum
{
	auto_mode = 0,
	self_cir_mode,
	heat_mode,
	ref_mode,
}power_on_run_mode_e;

typedef enum
{
	power_off_mode = 0,
	power_on_mode,
	test_mode,
	manual_mode,
	vacuum_mode,
}unit_mode_e;

typedef enum
{
	out_water_temp = 0,
	in_water_temp,
	bms_avr_temp,
}ctl_temp_e;

typedef enum
{
	auto_self_cir = 0,
	auto_heat,
	auto_ref,
}auto_run_state_e;

typedef enum
{
	test_self_cir = 0,
	test_heat,
	test_ref,
}test_run_state_e;

typedef enum
{
    mode_init = 0,
	mode_run,
	mode_run_to_stop,
	mode_stop,
	mode_err,
	mode_idle,
	mode_power_off,
	mode_end,
}mode_run_state_e;

typedef enum
{
    ctl_start = 0,
	self_start,
}start_mode_e;

typedef enum
{
	set_mods_power_off = 0,
	set_mods_ref,
	set_mods_heat,
	set_mods_self,
	set_mods_auto,
}mods_set_e;

typedef enum
{
	//参数
	DataBase_Index_Start = 0, 
	sys_rs485_bd = DataBase_Index_Start,
	sys_rs485_add,
	sys_self_cir_start_dev,
	sys_self_cir_stop_dev,
	sys_out_water_ctl_ref_temp,
	sys_out_water_ctl_heat_temp,
	sys_in_water_ctl_ref_temp,
	sys_in_water_ctl_heat_temp,
	sys_bms_temp_ctl_ref_temp,
	sys_bms_temp_ctl_heat_temp,
	sys_ref_backlash,
	sys_heat_backlash,
	sys_pump_high_duty,
	sys_pump_middle_duty,
	sys_pump_low_duty,
	sys_fan_high_duty,
	sys_fan_middle_duty,
	sys_fan_low_duty,
	sys_fan_inc_temp,
	sys_fan_dec_temp,
	sys_high_pres_protect_temp,
	sys_low_pres_protect_temp,
	sys_out_motor_high_temp_protect,
	sys_in_water_low_pres_protect,
	sys_out_water_high_pres_protect,
	sys_expan_init_open_1,
	sys_expan_init_open_2,
	sys_expan_init_open_3,
	sys_expan_init_open_4,
	sys_expan_init_open_5,
	sys_over_heat_point1,
	sys_over_heat_point2,
	sys_over_heat_point3,
	sys_over_heat_point4,
	sys_over_heat_point5,
	sys_expan_min_open,
	sys_expan_ctl_interval,
	sys_expan_init_hold_time,
	sys_unit_interval,
	sys_motor_start_interval,
	sys_ref_min_temp,
	sys_ref_max_temp,
	sys_motor_fre_limit1,
	sys_motor_fre_limit2,
	sys_motor_fre_limit3,
	sys_motor_fre_limit4,
	sys_motor_fre_limit5,
	sys_motor_fre_limit6,
	sys_motor_fre_max,
	sys_motor_fre_min,
	sys_test_hold_time,
	sys_ctl_temp_set,
	sys_out_water_temp_high,
	sys_out_water_temp_low,
	sys_start_mode,
	sys_unit_select,
	sys_valve_start_temp,
	sys_compre_start_max,
	sys_sn1,
	sys_sn2,
	sys_sn3,
	sys_sn4,
	sys_sn5,
	sys_sn6,
	sys_sn7,
	sys_sn8,
	sys_sn9,
	sys_sn10,
	sys_sn11,
	sys_sn12,
	sys_sn13,
	sys_sn14,
	sys_sn15,
	sys_sn16,
	sys_power_control_board_err_num,
	sys_power_expan_err_num,
	sys_adcref_control_board_err_num,
	sys_communication_motor_err_num,
	sys_in_watar_temp_sensor_err_num,
	sys_out_water_temp_sensor_err_num,
	sys_out_motor_temp_sensor_err_num,
	sys_in_motor_temp_sensor_err_num,
	sys_environment_temp_sensor_err_num,
	sys_in_water_pres_sensor_err_num,
	sys_out_water_pres_sensor_err_num,
	sys_out_motor_pres_sensor_err_num,
	sys_in_motor_pres_sensor_err_num,
	sys_out_motor_pres_high_err_num,
	sys_in_motor_pres_low_err_num,
	sys_out_motor_temp_high_err_num,
	sys_in_water_pres_low_err_num,
	sys_out_water_pres_high_err_num,
	sys_out_water_temp_low_err_num,
	sys_out_water_temp_high_err_num,
	sys_fan1_err_num,
	sys_fan2_err_num,
	sys_fan3_err_num,
	sys_compre_fre_err_num,
	sys_pump_err_num,
	sys_ptc_err_num,
	sys_communication_host_err_num,
	sys_envir_temp_over_err_num,
	sys_ptc_run_time_total,
	sys_motor_run_time_totaol,
	sys_ptc_run_cnt,
	sys_motor_run_cnt,
	sys_ptc_run_time_temp,
	sys_motor_run_time_temp,
//电压采集
    sys_power_volt,
	sys_ipm_volt,
	sys_expan_volt,
	sys_adc_ref,
//模式数据
	sys_power_state,
	sys_mode,
	sys_mode_run,

	sys_unit_mode,
	sys_unit_mode_next,

	sys_run_mode,
	sys_run_mode_next,
	sys_run_mode_status,

	sys_auto_run_mode,
	sys_auto_run_mode_next,

	sys_test_run_mode,
	sys_test_run_mode_next,

	sys_unit_mode_set,
	sys_power_set,
	sys_test_set,
	sys_manual_set,
	sys_vacuum_set,

	sys_mods_state,
	sys_mods_set,
//故障数据及处理
	sys_fault_code,
	sys_motor_out_temp_low_ctl,
	sys_motor_out_temp_high_ctl,
	sys_fre_limit,
	sys_fre_red,
	sys_motor_community_err,
	sys_bms_community_err,
	sys_motor_fault_code1,
	sys_motor_fault_code2,
	sys_envir_low,
	sys_pump_fault_code,
	sys_fault_code1,
	sys_fault_code2,
	sys_fault_code3,
	sys_fault_level,
	sys_fault_sum,
//BMS数据
	sys_bms_temp_min,
	sys_bms_temp_avr,
	sys_bms_temp_max,
	sys_bms_state,
//传感器数据
	sys_in_water_temp,
	sys_out_water_temp,
	sys_environment_temp,
	sys_in_motor_temp,
	sys_out_motor_temp,
	sys_in_water_press,
	sys_out_water_press,
	sys_in_motor_press,
	sys_out_motor_press,
	sys_over_heat,
	sys_out_pres_trans_temp,
	sys_in_pres_trans_temp,
//零部件运行状态
	sys_motor_speed_now,
	sys_motor_speed_temp,
	sys_expan_open,
	sys_expan_open_temp,
	sys_pump_level,
	sys_pump_duty,
	sys_fan_level,
	sys_fan_duty,
	sys_ptc_state,
	sys_heat_belt_state,
	sys_motor_power,
	sys_motor_volt,
	sys_fan1_fb,
	sys_fan2_fb,
	sys_fan3_fb,
	sys_relay_out,
	sys_relay_out_pc,
	sys_valve_state,
	sys_compre_fre_point,
//手动模式数据
	sys_manual_pump_level,
	sys_manual_fan_level,
	sys_manual_ptc_state,
	sys_manual_expan_open,
	sys_manual_motor_speed,
	sys_manual_heat_belt_state,
	sys_manual_valve_state,
//数据记录数量
	sys_key_data_cnt,
//系统数据
	sys_motor_start_time,
	sys_motor_end_time,
	sys_ref_temp,
	sys_heat_temp,
	sys_fault_shield,
	sys_temp_err_rate,
	sys_over_heat_last,
	sys_over_heat_point,
	sys_over_heat_rate,
//系统时间
	sys_year,
	sys_mon,
	sys_day,
	sys_hour,
	sys_min,
	sys_sec,
	sys_motor_run_time,	
	sys_ptc_run_time,	
//版本号
	sys_app_version,
	sys_sbl_version,
//自检
	sys_self_check_set,
	sys_self_pump,
	sys_self_fan,
	sys_self_ptc,
	sys_self_expan,
	sys_self_compre,
	sys_self_belt,
	sys_self_valve,
	DataBase_Index_End,	

	sys_mods_rvd,
}DataBase_Index_List;

typedef struct
{
	uint32_t value;
}databasae_str;

void DataBase_Init(void);
void DataBase_Push(uint8_t Index,uint32_t data);
uint32_t DataBase_Get(uint8_t Index);
void DataBase_Down(void);
#endif /* APP_APP_APP_INC_DATA_BASE_H_ */
