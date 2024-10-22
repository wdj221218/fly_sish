/*
 * app_upgrade.c
 *
 *  Created on: 2024年3月15日
 *      Author: Dell
 */

#include "app_upgrade.h"

typedef void (*pFunction)(void);
 extern upgradeinfo g_tUpGradeInfo;
void Sys_GotoApplication(void)
{
	pFunction Jump_To_Application;
	uint32_t JumpAddress;

	if (((*(__IO uint32_t*)APP_BASE) & 0x2FFE0000 ) == 0x20000000)//判断程序所占用的RAM空间是否正常
	{
		JumpAddress = *(__IO uint32_t*) (APP_BASE + 4);
		Jump_To_Application = (pFunction) JumpAddress;

		__set_PRIMASK(1);
		__set_MSP(*(__IO uint32_t*) APP_BASE);  			/*更新栈顶指针*/

		Jump_To_Application();/* 跳转到APP */
	}
}

void BOOT_Check(void)
{
	file_info_s* file_info = (file_info_s*)(APP_INFO);
	if(file_info->head_crc32 == HEAD_CRC32_ENABLE) //APP有效标志位置位
	{
		if( *(uint32_t*)UPGRADE_ADDR != UPGRADE_VALUE) //重编程标志位
		{
			Sys_GotoApplication();
		}
		else
		{
			*(uint32_t*)UPGRADE_ADDR =0xFAFAFAFA;
		}
	}
}


void Progress_Check(void)
{
	static uint32_t timer = 0;

	if(g_tUpGradeInfo.progress == 0 || g_tUpGradeInfo.progress == 100)
	{
	  return;
	}
	if(g_tUpGradeInfo.progress != g_tUpGradeInfo.progress_last)
	{
		g_tUpGradeInfo.progress_last = g_tUpGradeInfo.progress;
		timer = 0;
		return;
	}
	if(timer == 0)
	{
	  timer = get_tick_count();
	}

	if((get_elapsed_tick_count(timer) > 5000) && (0 != timer))
	{
		*(uint32_t*)UPGRADE_ADDR = 0xFFFFFFFF;
		bsp_flash_erase(CODE_BK_BASE,CODE_BK_SIZE * 1024);
		sys_reset();
	}
}







