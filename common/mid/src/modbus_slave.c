/*
 * modbus_slave.c
 *
 *  Created on: 2024年3月8日
 *      Author: Dell
 */


#include "modbus_slave.h"

#ifdef APP_SOFT

extern bsp_uart_str g_tISORS485_Uart;

typedef struct
{
    ModBus_Register mods_index;
    DataBase_Index_List database_index;
}data_push_s;

void bms_community_cb(void)
{
    DataBase_Push(sys_bms_community_err,1);
}

uint8_t Fan_void_0(uint16_t value)
{
	if(value)
	{

	}

	return 0;
}

uint8_t Fan_void_1(uint16_t value)
{
	if(value)
	{

	}

	return 1;
}

void add_0x1001_pro(void)
{
	if(set_mods_auto == DataBase_Get(sys_mods_set))
	{
        DataBase_Push(sys_power_set,1);
        DataBase_Push(sys_run_mode_next,auto_mode);
	}

    if (set_mods_self == DataBase_Get(sys_mods_set))
	{
        DataBase_Push(sys_power_set,1);
        DataBase_Push(sys_run_mode_next,self_cir_mode);
	}

    if (set_mods_heat == DataBase_Get(sys_mods_set))
	{
        DataBase_Push(sys_power_set,1);
        DataBase_Push(sys_run_mode_next,heat_mode);
	}

	if (set_mods_ref == DataBase_Get(sys_mods_set))
	{
        DataBase_Push(sys_power_set,1);
        DataBase_Push(sys_run_mode_next,ref_mode);
	}

    if (set_mods_power_off == DataBase_Get(sys_mods_set))
	{
    	DataBase_Push(sys_power_set,0);
	} 
    else
    {
    	DataBase_Push(sys_power_set,1);
    }
}

void add_0x1003_pro(void)
{
	if(in_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		DataBase_Push(sys_in_water_ctl_ref_temp,DataBase_Get(sys_ref_temp));
	}
	else if(out_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		DataBase_Push(sys_out_water_ctl_ref_temp,DataBase_Get(sys_ref_temp));
	}
	else if(bms_avr_temp == DataBase_Get(sys_ctl_temp_set))
	{
		DataBase_Push(sys_bms_temp_ctl_ref_temp,DataBase_Get(sys_ref_temp));
	}
}

void add_0x1005_pro(void)
{
	if(in_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		DataBase_Push(sys_in_water_ctl_heat_temp,DataBase_Get(sys_heat_temp));
	}
	else if(out_water_temp == DataBase_Get(sys_ctl_temp_set))
	{
		DataBase_Push(sys_out_water_ctl_heat_temp,DataBase_Get(sys_heat_temp));
	}
	else if(bms_avr_temp == DataBase_Get(sys_ctl_temp_set))
	{
		DataBase_Push(sys_bms_temp_ctl_heat_temp,DataBase_Get(sys_heat_temp));
	}
}

uint16_t mods_data[ModBus_Register_End];

data_push_s g_tdata_push[ModBus_Register_End] =
{
    {mods_unit_state,                   sys_mods_state},
	{mods_rvd_0001,                   	sys_mods_rvd},
    {mods_relay_out,                    sys_relay_out},
    {mods_out_water_temp,               sys_out_water_temp},
    {mods_in_water_temp,                sys_in_water_temp},
	{mods_rvd_0005,						sys_mods_rvd},
	{mods_rvd_0006,						sys_mods_rvd},
	{mods_rvd_0007,						sys_mods_rvd},
    {mods_envir_temp,                   sys_environment_temp},
	{mods_rvd_0009,						sys_mods_rvd},
    {mods_expan_open,                   sys_expan_open},
    {mods_pump_duty,                    sys_pump_duty},
    {mods_motor_speed,                  sys_motor_speed_now},
    {mods_fan_duty,                     sys_fan_duty},
    {mods_out_water_pres,               sys_out_water_press},
    {mods_in_water_pres,                sys_in_water_press},
    {mods_out_motor_pres,               sys_out_motor_press},
    {mods_fault_code1,                  sys_fault_code1},
    {mods_fault_code2,                  sys_fault_code2},
    {mods_fault_code3,                  sys_fault_code3},
	{mods_rvd_0001C,					sys_mods_rvd},
	{mods_rvd_0001D,					sys_mods_rvd},
	{mods_rvd_0001E,					sys_mods_rvd},
	{mods_rvd_0001F,					sys_mods_rvd},
	{mods_rvd_00020,					sys_mods_rvd},
	{mods_fault_level,					sys_fault_level},
	{mods_fault_sum,					sys_fault_sum},
	{mods_rvd_00024,					sys_mods_rvd},
	{mods_rvd_00025,					sys_mods_rvd},
	{mods_rvd_00026,					sys_mods_rvd},
    {mods_motor_power,                  sys_motor_power},
	{mods_rvd_003D,						sys_mods_rvd},
	{mods_rvd_003E,						sys_mods_rvd},
    {mods_ptc_run_time,                 sys_ptc_run_time_total},
	{mods_rvd_0040,						sys_mods_rvd},
    {mods_motor_run_time,               sys_motor_run_time_totaol},
	{mods_rvd_0042,						sys_mods_rvd},
	{mods_rvd_0043,						sys_mods_rvd},
	{mods_rvd_0044,						sys_mods_rvd},
    {mods_motor_volt,                   sys_motor_volt},

    {mods_start_mode,                   sys_start_mode},
    {mods_mode_set,                     sys_mods_set},
    {mods_ctl_temp_set,                 sys_ctl_temp_set},
    {mods_ref_temp,                     sys_ref_temp},
    {mods_ref_backlash,                 sys_ref_backlash},
    {mods_heat_temp,                    sys_heat_temp},
    {mods_heat_backlash,                sys_heat_backlash},
	{mods_rvd_1007,						sys_mods_rvd},
	{mods_rvd_1008,						sys_mods_rvd},
    {mods_alarm_out_water_temp_high,    sys_out_water_temp_high},
    {mods_alarm_out_water_temp_low,     sys_out_water_temp_low},
    {mods_alarm_in_water_pres_low,      sys_in_water_low_pres_protect},
    {mods_alarm_out_water_pres_high,    sys_out_water_high_pres_protect},
	{mods_rvd_100D,						sys_mods_rvd},
	{mods_rvd_100E,						sys_mods_rvd},
	{mods_rvd_100F,						sys_mods_rvd},
    {mods_address,                      sys_rs485_add},
    {mods_baud,                         sys_rs485_bd},
    {mods_bms_temp_max,                 sys_bms_temp_max},
    {mods_bms_temp_min,                 sys_bms_temp_min},
    {mods_bms_temp_avr,                 sys_bms_temp_avr},
    {mods_bms_state,                    sys_bms_state},
    {mods_self_cir_start_dev,           sys_self_cir_start_dev},
    {mods_self_cir_stop_dev,            sys_self_cir_stop_dev},
    {mods_year,                         sys_year},
    {mods_mon,                          sys_mon},
    {mods_day,                          sys_day},
    {mods_hour,                         sys_hour},
    {mods_min,                          sys_min},
    {mods_sec,                          sys_sec},
};

ModBus_Address mods_reg_list[ModBus_Register_End] =
{
	{0x0000,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//机组运行状态
	{0x0001,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0001
	{0x0002,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//继电器输出
	{0x0003,    READ,               0,      1650,       0,      0,      Fan_void_1,     trans_add,              400,    trans_sub,              400,    NULL},//出水温度
	{0x0004,    READ,               0,      1650,       0,      0,      Fan_void_1,     trans_add,              400,    trans_sub,              400,    NULL},//回水温度
	{0x0005,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0005
	{0x0006,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0006
	{0x0007,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0007
	{0x0008,    READ,               0,      1650,       0,      0,      Fan_void_1,     trans_add,              400,    trans_sub,              400,    NULL},//环境温度
	{0x0009,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0009
	{0x000B,    READ,               0,      500,        0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//膨胀阀开度
	{0x000C,    READ,               0,      100,        0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//水泵当前转速
	{0x000D,    READ,               0,      110,        0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//压缩机当前转速
	{0x000E,    READ,               0,      110,        0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//风机当前转速
	{0x0012,    READ,               0,      1000,       0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//出水压力
	{0x0013,    READ,               0,      1000,       0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//回水压力
	{0x0015,    READ,               0,      5000,       0,      0,      Fan_void_1,     trans_mult,             10,     trans_divi,             10,     NULL},//排气压力
	{0x0019,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//故障码1
	{0x001A,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//故障码2
    {0x001B,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//故障码3
	{0x001C,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_001C
	{0x001D,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_001D
	{0x001E,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_001E
	{0x001F,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_001F
	{0x0020,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0020
	{0x0022,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//故障级别
	{0x0023,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//故障汇总
	{0x0024,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0024
	{0x0025,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0025
	{0x0026,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0026
	{0x003C,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//压缩机运行功率
	{0x003D,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_003D
	{0x003E,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_003E
	{0x003F,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//电加热累计运行时间
	{0x0040,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0040
	{0x0041,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//压缩机累计运行时间
	{0x0042,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0042
	{0x0043,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0043
	{0x0044,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_0044
	{0x0045,    READ,               0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//电网电压

	{0x1000,    READ | WRITE,       0,      1,          0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//启动模式设置
	{0x1001,    READ | WRITE,       0,      4,          0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      add_0x1001_pro},//工作模式设置
	{0x1002,    READ | WRITE,       0,      2,          0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//控制温度选择
	{0x1003,    READ | WRITE,       400,    750,       	0,      0,      Fan_void_1,     trans_mult_10_add,      400,    trans_sub_divi_10,      400,    add_0x1003_pro},//制冷目标温度
	{0x1004,    READ | WRITE,       0,      100,        0,      0,      Fan_void_1,     trans_mult,             10,     trans_divi,             10,     NULL},//制冷回差
	{0x1005,    READ | WRITE,       400,    750,       	0,      0,      Fan_void_1,     trans_mult_10_add,      400,    trans_sub_divi_10,      400,    add_0x1005_pro},//制热目标温度
	{0x1006,    READ | WRITE,       0,      100,        0,      0,      Fan_void_1,     trans_mult,             10,     trans_divi,             10,     NULL},//制热回差
	{0x1007,    READ | WRITE,       0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_1007
	{0x1008,    READ | WRITE,       0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_1008
	{0x1009,    READ | WRITE,       0,      1650,       0,      0,      Fan_void_1,     trans_mult_10_add,      400,    trans_sub_divi_10,      400,    NULL},//出水温度过高报警点
	{0x100A,    READ | WRITE,       0,      1650,       0,      0,      Fan_void_1,     trans_mult_10_add,      400,    trans_sub_divi_10,      400,    NULL},//出水温度过低报警点
	{0x100B,    READ | WRITE,       0,      100,       	0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//回水压力过低报警点
	{0x100C,    READ | WRITE,       0,      100,       	0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//出水过高报警点
	{0x100D,    READ | WRITE,       0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_100D
	{0x100E,    READ | WRITE,       0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_100E
	{0x100F,    READ | WRITE,       0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//mods_rvd_100F
	{0x1010,    READ | WRITE,       1,      128,        0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//通讯地址
	{0x1011,    READ | WRITE,       0,      2,          0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//通讯波特率
	{0x1100,    READ | WRITE,       0,      1650,       0,      0,      Fan_void_1,     trans_add,              400,    trans_sub,              400,    NULL},//电芯最大温度
	{0x1101,    READ | WRITE,       0,      1650,       0,      0,      Fan_void_1,     trans_add,              400,    trans_sub,              400,    NULL},//电芯最小温度
	{0x1102,    READ | WRITE,       0,      1650,       0,      0,      Fan_void_1,     trans_add,              400,    trans_sub,              400,    NULL},//电芯平均温度
	{0x1103,    READ | WRITE,       0,      2,          0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//电池运行状态
	{0x1104,    READ | WRITE,       0,      350,        0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//自循环启动温差
	{0x1105,    READ | WRITE,       0,      350,        0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//自循环停止温差
	{0x1106,    READ | WRITE,       0,      65535,      0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//年
	{0x1107,    READ | WRITE,       0,      12,         1,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//月
	{0x1108,    READ | WRITE,       0,      31,         1,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//日
	{0x1109,    READ | WRITE,       0,      23,         0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//时
	{0x110A,    READ | WRITE,       0,      59,         0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//分
	{0x110B,    READ | WRITE,       0,      59,         0,      0,      Fan_void_1,     trans_null,             0,      trans_null,             0,      NULL},//秒
};

void data_trans(trans_cmd_e cmd,uint16_t* src_data,uint16_t* dest_data,uint16_t trans_data)
{
	int16_t temp;
	switch(cmd)
	{
		case trans_add:
			*(int16_t*)dest_data = *(int16_t*)src_data + trans_data;
		break;

		case trans_sub:
			*(int16_t*)dest_data = *(int16_t*)src_data - trans_data;
		break;

		case trans_mult:
			*(int16_t*)dest_data = *(int16_t*)src_data * trans_data;
		break;

		case trans_divi:
			*(int16_t*)dest_data = *(int16_t*)src_data / trans_data;
		break;

		case trans_mult_10_add:
			*(int16_t*)dest_data = *(int16_t*)src_data * 10 + trans_data;
		break;

		case trans_sub_divi_10:
			*(int16_t*)dest_data = (*(int16_t*)src_data - trans_data) / 10;
		break;

		default:
			*(int16_t*)dest_data = *(int16_t*)src_data;
		break;
	}
}

void mods_data_trans_out(ModBus_Register index)
{
	data_trans(mods_reg_list[index].out_trans_cmd,&mods_reg_list[index].value,&mods_data[index],mods_reg_list[index].out_trans_value);
}

void mods_data_trans_out_handle(void)
{
	for(uint8_t i = 0;i < ModBus_Register_End;i++)
	{
		mods_data_trans_out(i);
	}
}

void mods_data_trans_in(ModBus_Register index,uint16_t *src_value,uint16_t *destvalue)
{
	data_trans(mods_reg_list[index].in_trans_cmd,src_value,destvalue,mods_reg_list[index].in_trans_value);
}

void MODS_Data_Get(void)
{
    for(uint8_t i = 0; i< ModBus_Register_End; i++)
    {
    	mods_reg_list[i].value = DataBase_Get(g_tdata_push[i].database_index);
    }
    mods_data_trans_out_handle();
}

void MODS_Data_Push(void)
{
    for(uint8_t i = 0; i< ModBus_Register_End; i++)
    {
        if(0 == mods_reg_list[i].write_flag)
        {
        	continue;
        }

        DataBase_Push(g_tdata_push[i].database_index,mods_reg_list[i].value);
        mods_reg_list[i].write_flag = 0;

        if(NULL == mods_reg_list[i].Data_Pro)
        {
        	continue;
        }
        mods_reg_list[i].Data_Pro();
    }
	DataBase_Down();
}

void MODS_SendWithCRC(bsp_uart_str *pUart,uint8_t *Buff, uint16_t Len)
{
	uint16_t crc;
	uint8_t buf[100];

	BufferCopy(buf, Buff, Len);
	crc = CRC16_Modbus(Buff, Len);
	buf[Len++] = crc >> 8;
	buf[Len++] = crc;

	Per_UARTSend(pUart,buf, Len);
}

void MODS_SendAckErr(uint8_t ErrCode,ModBus_ID id)
{
    uint8_t buff[3];

    buff[0] = DataBase_Get(sys_rs485_add);
    buff[1] = id | 0x80;
    buff[2] = ErrCode;

    MODS_SendWithCRC(&g_tISORS485_Uart,buff,3);
}

uint16_t get_reg_add(uint16_t reg_add)
{
    uint16_t index_left,index_right,index_middle;
	index_left = 0;
	index_right = ModBus_Register_End - 1;

	while(index_left < index_right)
	{
		if(index_left == index_right - 1)
		{
			return 0xFFFF;
		}

		index_middle = index_left + (index_right - index_left)/2;

		if(mods_reg_list[index_middle].Address == reg_add)
		{
			return index_middle;
		}
		else if(mods_reg_list[index_left].Address == reg_add)
		{
			return index_left;
		}
		else if(mods_reg_list[index_right].Address == reg_add)
		{
			return index_right;
		}
		else if(mods_reg_list[index_middle].Address > reg_add)
		{
			index_right = index_middle;
		}
		else if(mods_reg_list[index_middle].Address < reg_add)
		{
			index_left = index_middle;
		}
	}
	return 0xFFFF;
}

ModBus_ErrCode Mods_Read_Register(uint16_t Register_Address,uint16_t *pvalue)
{
    ModBus_ErrCode err_code = Normal;
    uint16_t add_temp = 0;
    add_temp = get_reg_add(Register_Address);

    if(0xFFFF == add_temp)
    {
        err_code = Err_Reg_Addr;
        return err_code;
    }

    *pvalue = mods_data[add_temp];

    return err_code;
}

ModBus_ErrCode Mods_Write_Register(uint16_t Register_Address,uint16_t value)
{
    ModBus_ErrCode err_code = Normal;
    uint8_t add_temp = 0;
    add_temp = get_reg_add(Register_Address);
    uint16_t value_temp;
    uint16_t value_in;

    value_temp = value;
    mods_data_trans_in(add_temp,&value_temp,&value_in);

    if(255 == add_temp)
    {
        err_code = Err_Reg_Addr;
        return err_code;
    }

    if(0 == (mods_reg_list[add_temp].limit & WRITE))
    {
        err_code = Err_Write;
        return err_code;
    }
    
    if(0 == mods_reg_list[add_temp].Write(value))
    {
        err_code = Err_Write;
        return err_code;
    }

    if (value_in > mods_reg_list[add_temp].defmax || value_in < mods_reg_list[add_temp].defmin)
    {
        err_code = Err_Value;
        return err_code;
    }

    mods_reg_list[add_temp].value = value_in;
    mods_reg_list[add_temp].write_flag = 1;

    return err_code;   
}

void ModBus_03H_Handle(uint8_t *pBuffer,uint8_t len,ModBus_ID id)
{
    uint8_t i;
    uint8_t Tx_Buff[100];
    uint8_t Tx_Cnt = 0;
    uint16_t Register_Start;
    uint8_t Register_Num;
    uint16_t temp;
    ModBus_ErrCode err_code = Normal;
    sys_timer_start(SYS_TIMER_ID_BMS,SYS_TIMR_ID_ONE,30000,bms_community_cb);
    DataBase_Push(sys_bms_community_err,0);

    if(len < 8)
    {
        return;
    }

    Register_Start = pBuffer[2] << 8 | pBuffer[3];
    Register_Num = pBuffer[4] << 8 | pBuffer[5];

    Tx_Buff[0] = DataBase_Get(sys_rs485_add);
    Tx_Buff[1] = id;
    Tx_Buff[2] = Register_Num * 2;

    Tx_Cnt = 3;

    for(i = 0;i < Register_Num;i++)
    {
        err_code = Mods_Read_Register(Register_Start + i,&temp);

        if(err_code == Normal)
        {
            Tx_Buff[3 + i*2] = temp >> 8;
            Tx_Buff[3 + i*2 + 1] = temp;
            Tx_Cnt += 2;        
        }
        else
        {
            MODS_SendAckErr(err_code,id);
            return;
        }
    }

    MODS_SendWithCRC(&g_tISORS485_Uart,Tx_Buff,Tx_Cnt);
}

void ModBus_06H_Handle(uint8_t *pBuffer,uint8_t len,ModBus_ID id)
{
    uint8_t Tx_Buff[6];
    uint16_t Register_Start;
    uint16_t Register_Value;
    ModBus_ErrCode err_code = Normal; 
    uint16_t temp;
    sys_timer_start(SYS_TIMER_ID_BMS,SYS_TIMR_ID_ONE,30000,bms_community_cb);
    DataBase_Push(sys_bms_community_err,0);
    if(len != 8)
    {
        return;
    }

    Tx_Buff[0] = DataBase_Get(sys_rs485_add);
    Tx_Buff[1] = id;
    Tx_Buff[2] = pBuffer[2];
    Tx_Buff[3] = pBuffer[3];

    Register_Start = pBuffer[2] << 8 | pBuffer[3];
    Register_Value = pBuffer[4] << 8 | pBuffer[5]; 

    err_code = Mods_Write_Register(Register_Start,Register_Value);
    if(err_code != Normal)
    {
        MODS_SendAckErr(err_code,id);
        return;
    }
    mods_data_trans_out(get_reg_add(Register_Start));
    err_code = Mods_Read_Register(Register_Start,&temp);
    if(err_code != Normal)
    {
        MODS_SendAckErr(err_code,id);
        return;
    }

    Tx_Buff[4] = temp >> 8;
    Tx_Buff[5] = temp;

    if(err_code == Normal)
    {
        MODS_SendWithCRC(&g_tISORS485_Uart,Tx_Buff,6);
    }
    else
    {
        MODS_SendAckErr(err_code,id);
    }
}

void ModBus_10H_Handle(uint8_t *pBuffer,uint8_t len,ModBus_ID id)
{
    uint8_t i;
    uint8_t Tx_Buff[6];
    uint16_t Value_Buff[100];
    uint8_t Value_Num;
    uint16_t Register_Start;
    ModBus_ErrCode err_code = Normal; 
    sys_timer_start(SYS_TIMER_ID_BMS,SYS_TIMR_ID_ONE,30000,bms_community_cb);
    DataBase_Push(sys_bms_community_err,0);
    Tx_Buff[0] = DataBase_Get(sys_rs485_add);
    Tx_Buff[1] = id;
    Tx_Buff[2] = pBuffer[2];
    Tx_Buff[3] = pBuffer[3];
    Tx_Buff[4] = pBuffer[4];
    Tx_Buff[5] = pBuffer[5]; 

    Register_Start = pBuffer[2] << 8 | pBuffer[3];
    Value_Num = pBuffer[6];//字节数
    if(len < 9 + Value_Num)
    {
    	err_code = Err_Write;
    	MODS_SendAckErr(err_code,id);
    	return;
    }
    for ( i = 0; i < Value_Num/2; i++)
    {
        Value_Buff[i] = pBuffer[7 + i*2] << 8 | pBuffer[7 + i*2 + 1];
    }
    
    for (i = 0; i < Value_Num/2; i++)
    {
        err_code = Mods_Write_Register(Register_Start + i,Value_Buff[i]);
        
        if(err_code != Normal)
        {
            MODS_SendAckErr(err_code,id); 
            return;
        }
    }
    MODS_SendWithCRC(&g_tISORS485_Uart,Tx_Buff,6);
}


void modbus_cmd_analyze(uint8_t *pBuffer,uint8_t len)
{
    ModBus_ErrCode err_code = 0;
    switch (pBuffer[1])
    {
        case ModBus_03H:
            ModBus_03H_Handle(pBuffer,len,ModBus_03H);
        break;

        case ModBus_06H:
            ModBus_06H_Handle(pBuffer,len,ModBus_06H);
        break;

        case ModBus_10H:
            ModBus_10H_Handle(pBuffer,len,ModBus_10H);
        break;

        default:
            err_code = Err_Cmd;
            MODS_SendAckErr(err_code,pBuffer[1]);
        break;
    }
}

#endif
