/*
 * app_wdt.c
 *
 *  Created on: 2024年5月21日
 *      Author: Dell
 */
#include "app_wdt.h"

void app_wdt_init(void)
{
	if(RCM_ReadStatusFlag(RCM_FLAG_WWDTRST) == SET)
	{
		RCM_ClearStatusFlag();
	}

	RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_WWDT);

	WWDT_ConfigTimebase(WWDT_TIME_BASE_8);
	/*窗口期 17ms~43ms*/
	WWDT_ConfigWindowData(127);//窗口值设置
	WWDT_Enable(127);	//设置计数器数值
}
void app_wdt_feed(void)
{
	WWDT_ConfigCounter(127);//设置计数器数值
}
void app_wdt_disable(void)
{
	RCM_DisableAPB1PeriphClock(RCM_APB1_PERIPH_WWDT);
}

