/*
 * modbus_master.h
 *
 *  Created on: 2024年3月27日
 *      Author: Dell
 */

#ifndef COMMON_MID_INC_MODBUS_MASTER_H_
#define COMMON_MID_INC_MODBUS_MASTER_H_

#include "stdio.h"
#include "bsp.h"
#include "mid.h"
#include "app.h"
#include "sys_lib.h"


#define SlaveAddr				0x01
#define MODS_BUFF_SIZE			128

#define FREQUENCY_RDG1          1999    /* 设定输出机械频率				写/读   */
#define FREQUENCY_RDG2          2102    /* 实际运行机械频率				读     */
#define FREQUENCY_RDG3          2100    /*故障码1					    读	  */
#define FREQUENCY_RDG4          2109    /*故障码2					    读	  */
#define FREQUENCY_RDG5          2000    /*压缩机控制开关					读/写	  */
#define FREQUENCY_RDG6          2119    /*压缩机功率					读/写	  */
#define FREQUENCY_RDG7          2105    /*电网电压                    读    */


#define MODS_OVER_TIM           300
#define ARRAY_SIZE(array) 		(sizeof(array)/sizeof(array[0]))
typedef struct
{
	uint8_t 	RxBuf[MODS_BUFF_SIZE];
	uint8_t 	RxCount;
	uint8_t	 	RxStatus;
	uint8_t 	RxNewFlag;

	uint8_t 	RspCode;

	uint8_t 	TxBuf[MODS_BUFF_SIZE];
	uint8_t 	TxCount;
	
	uint16_t 	Reg03H;

	uint8_t 	RegNum;			/* 寄存器个数 */

	/* 应答命令标志 0 表示执行失败 1表示执行成功 */

	uint8_t 	fAck03H;
	uint8_t 	fAck06H;
	uint8_t 	fAck10H;
	
	uint16_t 	Send_cmd_temp;
	uint16_t 	Send_reg_temp;
	uint16_t 	Send_value_temp;
	uint16_t	Send_num_temp;
	uint16_t*   Send_buff_temp;

	uint8_t 	send_cnt;
	uint32_t    err_cnt;

}MODH_T;

typedef struct
{
	uint16_t addr;
	uint8_t  cmd;
	uint16_t (*get_value)(void);
}modbud_master_t;

uint8_t MODH_ReadParam_03H(uint16_t _reg, uint16_t _num);
uint8_t MODH_WriteParam_06H(uint16_t _reg, uint16_t _value);
uint8_t MODH_WriteParam_10H(uint16_t _reg, uint8_t _num, uint16_t *_buf);
void modbus_master_cb(void);
#endif /* COMMON_MID_INC_MODBUS_MASTER_H_ */

