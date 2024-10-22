#ifndef __DEBUG_LOG_H
#define __DEBUG_LOG_H

#include "stdint.h"
#include "stdarg.h"
#include "stdio.h"
#include "mid.h"
#include "app.h"
#include "bsp.h"

#define LOG_HEAD                        LOG_ID_APP
#define LOG_CACHE_SIZE 					4096
#define LOG_TEMP_SIZE 					128

#define LOG_ID_SUB_ERROR   				(1)
#define LOG_ID_SUB_WARING  				(1<< 1)
#define LOG_ID_SUB_N   					(1<< 2)
#define LOG_ID_SUB_0   					(1<< 3)
#define LOG_ID_SUB_1   					(1<< 4)
#define LOG_ID_SUB_2   					(1<< 5)
#define LOG_ID_SUB_3   					(1<< 6)
#define LOG_ID_SUB_4   					(1<< 7)
#define LOG_ID_SUB_5   					(1<< 8)
#define LOG_ID_SUB_6   					(1<< 9)
#define LOG_ID_SUB_7  					(1<< 10)
#define LOG_ID_SUB_8  					(1<< 11)
#define LOG_ID_SUB_9  					(1<< 12)
#define LOG_ID_SUB_10  					(1<< 13)
#define LOG_ID_SUB_11  					(1<< 14)
#define LOG_ID_SUB_12  					(1<< 15)
#define LOG_ID_SUB_ALL 					0XFFFF


#define LOG_ID_SUB_BSP_ADC              LOG_ID_SUB_0
#define LOG_ID_SUB_BSP_FLASH            LOG_ID_SUB_1
#define LOG_ID_SUB_BSP_GPIO            	LOG_ID_SUB_2
#define LOG_ID_SUB_BSP_IIC	            LOG_ID_SUB_3
#define LOG_ID_SUB_BSP_SYSTICK	       	LOG_ID_SUB_5
#define LOG_ID_SUB_BSP_TIMER            LOG_ID_SUB_6
#define LOG_ID_SUB_BSP_SYSTICK          LOG_ID_SUB_7
#define LOG_ID_SUB_BSP_UART          	LOG_ID_SUB_8
#define LOG_ID_SUB_BSP_WDT          	LOG_ID_SUB_8

#define LOG_ID_SUB_MID_DATABASE         LOG_ID_SUB_0
#define LOG_ID_SUB_MID_EEPROM	        LOG_ID_SUB_1
#define LOG_ID_SUB_MID_KEYDATA	        LOG_ID_SUB_2
#define LOG_ID_SUB_MID_MODS_M	        LOG_ID_SUB_3
#define LOG_ID_SUB_MID_MODS_S	        LOG_ID_SUB_4
#define LOG_ID_SUB_MID_PARAM	        LOG_ID_SUB_5
#define LOG_ID_SUB_MID_COMMUNITY	    LOG_ID_SUB_6
#define LOG_ID_SUB_MID_LOG	    		LOG_ID_SUB_7


#define LOG_ID_SUB_APP_TASK         	LOG_ID_SUB_0
#define LOG_ID_SUB_APP_UNIT	        	LOG_ID_SUB_1
#define LOG_ID_SUB_APP_COMPRE          	LOG_ID_SUB_2
#define LOG_ID_SUB_APP_EXPAN        	LOG_ID_SUB_3
#define LOG_ID_SUB_APP_FAN          	LOG_ID_SUB_4
#define LOG_ID_SUB_APP_PUMP          	LOG_ID_SUB_5
#define LOG_ID_SUB_APP_PTC          	LOG_ID_SUB_6
#define LOG_ID_SUB_APP_FAULT          	LOG_ID_SUB_7
#define LOG_ID_SUB_APP_SENSOR          	LOG_ID_SUB_8


#define LOG_ID_SUB_SYS_INIT         	LOG_ID_SUB_0

enum
{
    LOG_ID_SYS = 0,
    LOG_ID_BSP,
    LOG_ID_MID,
    LOG_ID_APP,
	LOG_ID_MAX,//日志模块最大值定义保留
} ;

#define LOG_ID_SYS_MARK  LOG_ID_SUB_ALL
#define LOG_ID_BSP_MARK  0
#define LOG_ID_MID_MARK  0
#define LOG_ID_APP_MARK  0

#pragma pack (1)
typedef struct
{
	u8  head;
	u32 id;
	u16 cmd[4];
} bsp_uart_log_s;

typedef struct
{
	uint32_t mark;
	uint8_t* col;
} log_ctrl_s;

typedef struct
{
    uint16_t r_idx;
    uint16_t w_idx;
    uint16_t all_size;
    uint8_t send_buf[LOG_CACHE_SIZE];
    uint8_t cache_buf[LOG_TEMP_SIZE];
} log_ctx_s;

typedef struct
{
    u8 data[14];
} ctrl_msg_s;
#pragma pack ()
void Send_Log(uint8_t *buff,uint8_t len);
uint32_t dev_dbg_get_mark(uint16_t log_id, uint16_t log_sub_id);
void wdbg_printf(uint8_t log_id, uint16_t sub_id, const char * format, ... );
void wdbg_dump(uint8_t log_id, uint16_t sub_id, uint8_t*data, uint16_t size);
void dev_dbg_set_mark(u16 log_id, u32 log_sub_id);

#define WDBG(sub, fmt, ...)         wdbg_printf(LOG_HEAD, sub,"%d:INFO %s %d:"fmt"\n",get_tick_count(),  __func__, __LINE__, ##__VA_ARGS__)
#define WDBG_HEX(sub, data, size)   wdbg_dump(LOG_HEAD, sub, data, size)
void mid_log_print_server(void);
void mid_log_uart_init(void);
#endif
