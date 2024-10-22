/*
 * app_vacuum_mode.c
 *
 *  Created on: 2024年6月19日
 *      Author: Dell
 */
#include "app_vacuum_mode.h"

void vacuum_mode_run(void)
{
	app_expan_run_vacuum(1);
}

uint8_t vacuum_mode_exit(void)
{
	app_expan_run_vacuum(0);
	if(0 == DataBase_Get(sys_expan_open))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}



