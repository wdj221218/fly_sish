/*
 * bsp_systick.h
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_SYSTICK_H_
#define COMMON_BSP_INC_BSP_SYSTICK_H_


#include <stdint.h>
#include "apm32f10x.h"
#include "app_task.h"

#define TIMER_NUM  6 /* 软件定时器个数 */

#define TIMEOUT_RS485       0
#define TIMEOUT_ISO_RS485   1
#define TIMEOUT_RS232       2
#define TIMEOUT_MODS_REPLY  3
#define LOG_TIMEOUT_RS232   4

typedef enum
{
	TIMER_ONCE_MODE = 0,
	TIMER_AUTO_MODE = 1
}TIMER_MODE;

typedef struct  
{
	uint8_t Mode;		
    uint8_t Flag;	
	uint32_t Count;
	uint32_t PreLoad;
}bsp_soft_timer_str;

uint32_t get_tick_count(void);
uint32_t get_elapsed_tick_count(uint32_t last_tick);
void bsp_once_timer_start(uint8_t id, uint32_t period);
void bsp_auto_timer_start(uint8_t id, uint32_t period);
void bsp_timer_stop(uint8_t id);
uint8_t bsp_timer_check(uint8_t id);
void bsp_systick_init(void);
void bsp_delay_ms(uint16_t tim);



#endif /* COMMON_BSP_INC_BSP_SYSTICK_H_ */
