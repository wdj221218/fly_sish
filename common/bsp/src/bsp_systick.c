/*
 * bsp_systick.c
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */
#include "bsp_systick.h"

bsp_soft_timer_str  tTimer[TIMER_NUM];

uint32_t SysTickms_Cnt = 0;

uint32_t get_tick_count(void)
{
	return SysTickms_Cnt;
}

uint32_t get_interval_by_tick(uint32_t old_tick, uint32_t new_tick)
{
	if (new_tick >= old_tick)
	{
		return new_tick - old_tick;
	}
	return new_tick + ((uint32_t) 0xFFFFFFFFu - old_tick);
}

uint32_t get_elapsed_tick_count(uint32_t last_tick)
{
	return get_interval_by_tick(last_tick, get_tick_count());
}

void bsp_soft_timer_init(void)
{
    uint8_t i;

    for(i = 0;i < TIMER_NUM;i++)
    {
        tTimer[i].Count = 0;
        tTimer[i].Flag = 0;
        tTimer[i].PreLoad = 0;
        tTimer[i].Mode = TIMER_ONCE_MODE;
    }
}

void bsp_once_timer_start(uint8_t id, uint32_t period)
{
	if(id > TIMER_NUM)
	{
		return;
	}
	__set_PRIMASK(1); 
	
	tTimer[id].Count = period;			
	tTimer[id].PreLoad = period;		
	tTimer[id].Flag = 0;							
	tTimer[id].Mode = TIMER_ONCE_MODE;	
	
	__set_PRIMASK(0); 
}

void bsp_auto_timer_start(uint8_t id, uint32_t period)
{
	if(id > TIMER_NUM)
	{
		return;
	}
	__set_PRIMASK(1); 
	
	tTimer[id].Count = period;			
	tTimer[id].PreLoad = period;		
	tTimer[id].Flag = 0;							
	tTimer[id].Mode = TIMER_AUTO_MODE;	
	
	__set_PRIMASK(0); 
}

void bsp_timer_stop(uint8_t id)
{
	if(id > TIMER_NUM)
	{
		return;
	}
	__set_PRIMASK(1); 
	
	tTimer[id].Count = 0;			
	tTimer[id].PreLoad = 0;		
	tTimer[id].Flag = 0;							
	tTimer[id].Mode = TIMER_ONCE_MODE;
	
	__set_PRIMASK(0); 
}

uint8_t bsp_timer_check(uint8_t id)
{
	if (id >= TIMER_NUM)
	{
		return 0;
	}
	if (tTimer[id].Flag == 1)
	{
		tTimer[id].Flag = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

void bsp_soft_timer_updata(bsp_soft_timer_str *tmr)
{
	if (tmr->Count > 0)
	{
		if (--tmr->Count == 0)
		{
			tmr->Flag = 1;

			if(tmr->Mode == TIMER_AUTO_MODE)
			{
				tmr->Count = tmr->PreLoad;
			}
		}
	}
}


uint32_t delay_time = 0;
void bsp_delay_ms(uint16_t tim)
{
	delay_time = 0;
	while(delay_time < tim);
}

void bsp_systick_init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
    bsp_soft_timer_init();
}

void SysTick_Handler(void)
{
	uint8_t i;

	__set_PRIMASK(1); 
	SysTickms_Cnt++;
	delay_time++;
	__set_PRIMASK(0); 
#ifdef APP_SOFT
	app_get_fan_fb();
	app_fault_tick_inc_handle();
#endif
	TaskRun1msIsrCallback();

	
	for (i = 0; i < TIMER_NUM; i++)
	{
		bsp_soft_timer_updata(&tTimer[i]);
	}
}

