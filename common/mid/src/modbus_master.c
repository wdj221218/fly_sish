/*
 * modbus_master.c
 *
 *  Created on: 2024年3月27日
 *      Author: Dell
 */

#include "modbus_master.h"

#ifdef APP_SOFT

MODH_T g_tModH;
extern bsp_uart_str g_tRs485_Uart;

uint8_t Motor_err_flag = 0;

uint16_t get_point_speed(void)
{
	return DataBase_Get(sys_motor_speed_temp);
}

uint16_t set_compre_act(void)
{
	return 1;
}

uint16_t set_compre_unact(void)
{
	return 0;
}

void modbus_master_cb(void)
{
	DataBase_Push(sys_motor_community_err,1);
}


modbud_master_t g_tmodbus_send_cfg1[] =
{
	{FREQUENCY_RDG2,0x03,NULL},
	{FREQUENCY_RDG3,0x03,NULL},
	{FREQUENCY_RDG4,0x03,NULL},
	{FREQUENCY_RDG6,0x03,NULL},
	{FREQUENCY_RDG7,0x03,NULL},
};

modbud_master_t g_tmodbus_send_cfg2[] =
{
	{FREQUENCY_RDG1,0x06,get_point_speed},
	{FREQUENCY_RDG5,0x06,set_compre_act},
};

modbud_master_t g_tmodbus_send_cfg3[] =
{
	{FREQUENCY_RDG1,0x06,get_point_speed},
};

modbud_master_t g_tmodbus_send_cfg4[] =
{
	{FREQUENCY_RDG1,0x06,get_point_speed},
	{FREQUENCY_RDG5,0x06,set_compre_unact},
};

void MODH_Send03H(uint8_t _addr, uint16_t _reg, uint16_t _num)
{
	g_tModH.TxCount = 0;
	g_tModH.TxBuf[g_tModH.TxCount++] = _addr;		
	g_tModH.TxBuf[g_tModH.TxCount++] = 0x03;			
	g_tModH.TxBuf[g_tModH.TxCount++] = _reg >> 8;	
	g_tModH.TxBuf[g_tModH.TxCount++] = _reg;		
	g_tModH.TxBuf[g_tModH.TxCount++] = _num >> 8;
	g_tModH.TxBuf[g_tModH.TxCount++] = _num;		

	g_tModH.fAck03H = 0;		
	g_tModH.RegNum = _num;		
	g_tModH.Reg03H = _reg;		

    MODS_SendWithCRC(&g_tRs485_Uart,g_tModH.TxBuf,g_tModH.TxCount);
}

void MODH_Send06H(uint8_t _addr, uint16_t _reg, uint16_t _value)
{
	g_tModH.TxCount = 0;
	g_tModH.TxBuf[g_tModH.TxCount++] = _addr;			/* 从站地址 */
	g_tModH.TxBuf[g_tModH.TxCount++] = 0x06;			/* 功能码 */	
	g_tModH.TxBuf[g_tModH.TxCount++] = _reg >> 8;		/* 寄存器编号 高字节 */
	g_tModH.TxBuf[g_tModH.TxCount++] = _reg;			/* 寄存器编号 低字节 */
	g_tModH.TxBuf[g_tModH.TxCount++] = _value >> 8;		/* 寄存器值 高字节 */
	g_tModH.TxBuf[g_tModH.TxCount++] = _value;			/* 寄存器值 低字节 */

    g_tModH.fAck06H = 0;

	MODS_SendWithCRC(&g_tRs485_Uart,g_tModH.TxBuf,g_tModH.TxCount);
}

void MODH_Send10H(uint8_t _addr, uint16_t _reg, uint8_t _num, uint16_t *_buf)
{
	uint16_t i;
	
	g_tModH.TxCount = 0;
	g_tModH.TxBuf[g_tModH.TxCount++] = _addr;		/* 从站地址 */
	g_tModH.TxBuf[g_tModH.TxCount++] = 0x10;		/* 从站地址 */	
	g_tModH.TxBuf[g_tModH.TxCount++] = _reg >> 8;	/* 寄存器编号 高字节 */
	g_tModH.TxBuf[g_tModH.TxCount++] = _reg;		/* 寄存器编号 低字节 */
	g_tModH.TxBuf[g_tModH.TxCount++] = _num >> 8;	/* 寄存器个数 高字节 */
	g_tModH.TxBuf[g_tModH.TxCount++] = _num;		/* 寄存器个数 低字节 */
	g_tModH.TxBuf[g_tModH.TxCount++] = 2 * _num;	/* 数据字节数 */
	
	for (i = 0; i < 2 * _num; i++)
	{
		if (g_tModH.TxCount > MODS_BUFF_SIZE - 3)
		{
			return;		/* 数据超过缓冲区超度，直接丢弃不发送 */
		}
		g_tModH.TxBuf[g_tModH.TxCount++] = _buf[i] << 8;		/* 后面的数据长度 */
		g_tModH.TxBuf[g_tModH.TxCount++] = _buf[i];
	}
	 g_tModH.fAck10H = 0;
     
	MODS_SendWithCRC(&g_tRs485_Uart,g_tModH.TxBuf,g_tModH.TxCount);	
}

ModBus_ErrCode MODH_Read_03H(uint8_t *buff,uint8_t len)
{
	uint8_t bytes;
	ModBus_ErrCode err_temp;
	sys_timer_start(SYS_TIMER_ID_MODS_M,SYS_TIMR_ID_ONE,30000,modbus_master_cb);
	DataBase_Push(sys_motor_community_err,0);
    g_tModH.fAck03H = 1;

    if (len == 0)
    {
        return Err_Rec;
    }

    if(5 == len)
    {
        if((buff[1] & 0x80) != 0x80)
        {
            return Err_Rec;
        }
        return (ModBus_ErrCode)(buff[1] & 0x7F);
    }

    bytes = buff[2];	/* 数据长度 字节数 */
    for(uint8_t i = 0;i < bytes/2;i++)
    {
        switch (g_tModH.Reg03H)
        {
            case FREQUENCY_RDG1:

            break;

            case FREQUENCY_RDG2:
            	DataBase_Push(sys_motor_speed_now,(uint16_t)buff[4]);
            break;

            case FREQUENCY_RDG3:
 				DataBase_Push(sys_motor_fault_code1,((uint16_t)(buff[3]) << 8) + (uint16_t)(buff[4]));
            break;

            case FREQUENCY_RDG4:
				DataBase_Push(sys_motor_fault_code2,((uint16_t)(buff[3]) << 8) + (uint16_t)(buff[4]));
            break;

            case FREQUENCY_RDG6:
            	DataBase_Push(sys_motor_power,((uint16_t)(buff[3]) << 8) + (uint16_t)(buff[4]));
            break;

			case FREQUENCY_RDG7:
            	DataBase_Push(sys_motor_volt,((uint16_t)(buff[3]) << 8) + (uint16_t)(buff[4]));
            break;

            default:

            break;
        }

        g_tModH.Reg03H++;
    }				
    return Normal;
}

ModBus_ErrCode MODH_Read_06H(uint8_t *buff,uint8_t len)
{
	sys_timer_start(SYS_TIMER_ID_MODS_M,SYS_TIMR_ID_ONE,30000,modbus_master_cb);
	DataBase_Push(sys_motor_community_err,0);
    g_tModH.fAck06H = 1;		/* 接收到应答 */
	if (len == 0)
    {
        return Err_Rec;
    }

    if(5 == len)
    {
        if((buff[1] & 0x80) != 0x80)
        {
            return Err_Rec;
        }
        return (ModBus_ErrCode)(buff[1] & 0x7F);
    }

    if (buff[0] == SlaveAddr)		
    {
        return Normal;
    }

	return Err_Rec;
}

ModBus_ErrCode MODH_Read_10H(uint8_t *buff,uint8_t len)
{
	sys_timer_start(SYS_TIMER_ID_MODS_M,SYS_TIMR_ID_ONE,30000,modbus_master_cb);
	DataBase_Push(sys_motor_community_err,0);
    g_tModH.fAck10H = 1;		/* 接收到应答 */
	if (len == 0)
    {
        return Err_Rec;
    }

    if(5 == len)
    {
        if((buff[1] & 0x80) != 0x80)
        {
            return Err_Rec;
        }
        return (ModBus_ErrCode)(buff[1] & 0x7F);
    }

    if (buff[0] == SlaveAddr)		
    {
        return Normal;
    }
	return Err_Rec;
}

uint8_t MODH_ReadParam_03H(uint16_t _reg, uint16_t _num)
{
	MODH_Send03H (SlaveAddr, _reg, _num);

	g_tModH.Send_cmd_temp = 0x03;
	g_tModH.Send_reg_temp = _reg;
	g_tModH.Send_num_temp = _num;

	return 0;
}

uint8_t MODH_WriteParam_06H(uint16_t _reg, uint16_t _value)
{	
	MODH_Send06H (SlaveAddr, _reg, _value);

	g_tModH.Send_cmd_temp = 0x06;
	g_tModH.Send_reg_temp = _reg;
	g_tModH.Send_value_temp = _value;

	return 0;
}

uint8_t MODH_WriteParam_10H(uint16_t _reg, uint8_t _num, uint16_t *_buf)
{
    MODH_Send10H(SlaveAddr, _reg, _num, _buf);

	g_tModH.Send_cmd_temp = 0x10;
	g_tModH.Send_reg_temp = _reg;
	g_tModH.Send_num_temp = _num;
	g_tModH.Send_buff_temp = _buf;

    return 0;
}

void modbus_cmd_analyze_master(uint8_t *pBuffer,uint8_t len)
{	
	switch (pBuffer[1])			/* 第2个字节 功能码 */
	{
		case 0x03:	/* 读取保持寄存器 在一个或多个保持寄存器中取得当前的二进制值 */
			MODH_Read_03H(pBuffer,len);
		break;

		case 0x06:	/* 写单个寄存器 */
			MODH_Read_06H(pBuffer,len);
		break;		

		case 0x10:	 //写多个寄存器
			MODH_Read_10H(pBuffer,len);
		break;
		
		default:

		break;
	}
}
extern start_in_statelist start_in;

uint8_t master_send(modbud_master_t *pcfg)
{
	static uint8_t index = 0;
	static modbud_master_t *pcfg_temp = NULL;
	uint8_t res = 0;

	if(pcfg != pcfg_temp)
	{
		index = 0;
		pcfg_temp = pcfg;
	}

	if(0x03 == pcfg[index].cmd)
	{
		res = MODH_ReadParam_03H(pcfg[index].addr,1);
	}
	else if(0x06 == pcfg[index].cmd)
	{
		if(NULL != pcfg[index].get_value)
		{
			res = MODH_WriteParam_06H(pcfg[index].addr,pcfg[index].get_value());
		}
	}

	if(1 == res)
	{

	}
	else if(0 == res)
	{
		index++;
		if(pcfg == &g_tmodbus_send_cfg1)
		{
			index = index%ARRAY_SIZE(g_tmodbus_send_cfg1);
		}
		else if(pcfg == &g_tmodbus_send_cfg2)
		{
			index = index%ARRAY_SIZE(g_tmodbus_send_cfg2);
		}
		else if(pcfg == &g_tmodbus_send_cfg3)
		{
			index = index%ARRAY_SIZE(g_tmodbus_send_cfg3);
		}
		else if(pcfg == &g_tmodbus_send_cfg4)
		{
			index = index%ARRAY_SIZE(g_tmodbus_send_cfg4);
		}
	}

	return index;
}

void modbus_master_send_handle(void)
{
	uint8_t res;
	if(start == start_in)
	{
		res = master_send(&g_tmodbus_send_cfg2);
		if(0 == res)
		{
			start_in = normal;
		}

		return;
	}

	if(end == start_in)
	{
		res = master_send(&g_tmodbus_send_cfg4);
		if(0 == res)
		{
			start_in = normal;
		}

		return;
	}

	if(speed_change == start_in)
	{
		res = master_send(&g_tmodbus_send_cfg3);
		if(0 == res)
		{
			start_in = normal;
		}

		return;
	}

	res = master_send(&g_tmodbus_send_cfg1);
}

#endif




