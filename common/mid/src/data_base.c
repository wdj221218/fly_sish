/*
 * app_database.c
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */
#include "data_base.h"

static databasae_str g_tPDataBase_Buff[DataBase_Index_End] = {0};

void DataBase_Push(uint8_t Index,uint32_t data)
{
	if((Index < DataBase_Index_Start) ||(Index > DataBase_Index_End))
	{
		return ;
	}

	g_tPDataBase_Buff[Index].value = data;

}

uint32_t DataBase_Get(uint8_t Index)
{
	if((Index < DataBase_Index_Start) ||(Index > DataBase_Index_End))
	{
		return 0;
	}

	return g_tPDataBase_Buff[Index].value;
}

void DataBase_Init(void)
{
	for(uint8_t i = 0;i < Param_Index_End;i++)
	{
		DataBase_Push(i,Param_Get(i));
	}
}

void DataBase_Down(void)
{
	for(uint8_t i = 0;i < Param_Index_End;i++)
	{
		Param_Set(i,DataBase_Get(i));
	}
#ifdef APP_SOFT
	app_wdt_feed();
#endif
	Param_Download();
}
