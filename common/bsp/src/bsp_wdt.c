/*
 * bsp_wdt.c
 *
 *  Created on: 2024年3月18日
 *      Author: Dell
 */
#include "bsp_wdt.h"

void bsp_wdt_init(void)
{
	if(RCM_ReadStatusFlag(RCM_FLAG_WWDTRST) == SET)
	{
		RCM_ClearStatusFlag();
	}

	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_WWDT);

	WWDT_ConfigTimebase(WWDT_TIME_BASE_8);
	WWDT_ConfigWindowData(127);//窗口值设置
	WWDT_Enable(127);	//设置计数器数值

}

void WDT_Feed(void)
{
	WWDT_ConfigCounter(127);//设置计数器数值
}

void WDT_Disable(void)
{
	RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_WWDT);
}

