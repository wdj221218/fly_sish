/*
 * app_keydata_save.c
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */


#include "keydata_save.h"

#ifdef APP_SOFT
key_data_str g_tkey_data;

void key_date_download(uint32_t addr,uint16_t cnt,uint8_t add_size)
{
	uint16_t crc_temp;
	uint16_t temp_cnt;
	temp_cnt = cnt;
#ifdef FLASH
#ifdef APP_SOFT
	app_wdt_feed();
#endif
	bsp_flash_erase(addr, KEY_DATA_SIZE*1024);
	bsp_flash_write(addr,&temp_cnt,1);

	if(cnt > NUM_MAX)
	{
		crc_temp = bsp_flash_crc(g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);
		bsp_flash_write(addr + add_size * 1024 - 2,&crc_temp,1);
		bsp_flash_write(addr + 2,g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);
	}
	else
	{
		crc_temp = bsp_flash_crc(g_tkey_data.key_data_buff[0],cnt * DATA_LEN);
		bsp_flash_write(addr + add_size * 1024 - 2,&crc_temp,1);
		bsp_flash_write(addr + 2,g_tkey_data.key_data_buff[0],cnt * DATA_LEN);
	}
#endif

#ifdef EEPROM
	eeprom_write_u16(addr,&temp_cnt,1);

	if(cnt > NUM_MAX)
	{
		crc_temp = bsp_flash_crc(g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);
		eeprom_write_u16(addr + add_size - 2,&crc_temp,1);
		eeprom_write_u16(addr + 2,g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);
	}
	else
	{
		crc_temp = bsp_flash_crc(g_tkey_data.key_data_buff[0],cnt * DATA_LEN);
		eeprom_write_u16(addr + add_size - 2,&crc_temp,1);
		eeprom_write_u16(addr + 2,g_tkey_data.key_data_buff[0],cnt * DATA_LEN);
	}
#endif

}

void key_data_init(void)
{
	uint8_t i;
	uint16_t temp;
	uint16_t crc_temp;
#ifdef FLASH
	bsp_flash_read(KEY_DATA_BASE,&temp,1);
	bsp_flash_read(KEY_DATA_BASE + KEY_DATA_SIZE * 1024 - 2,&crc_temp,1);

	if(temp == 0xFFFF)
	{
		g_tkey_data.write_index = 0;
		g_tkey_data.read_index = 0;
		DataBase_Push(sys_key_data_cnt,0);
		return;
	}

    if(temp < NUM_MAX)
	{
		g_tkey_data.write_index = temp;
		g_tkey_data.read_index = 0;
		DataBase_Push(sys_key_data_cnt,temp);

		bsp_flash_read(KEY_DATA_BASE + 2,g_tkey_data.key_data_buff[0],temp * DATA_LEN);

		if(crc_temp != bsp_flash_crc(g_tkey_data.key_data_buff[0],temp * DATA_LEN))
		{
			bsp_flash_read(KEY_DATA_BK_BASE + 2,g_tkey_data.key_data_buff[0],temp * DATA_LEN);
		}
	}
	else if(temp >= NUM_MAX)
	{
		g_tkey_data.write_index = temp % NUM_MAX;
		g_tkey_data.read_index = g_tkey_data.write_index;
		DataBase_Push(sys_key_data_cnt,temp);

	    bsp_flash_read(KEY_DATA_BASE + 2,g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);


		if(crc_temp != bsp_flash_crc(g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN))
		{
			bsp_flash_read(KEY_DATA_BK_BASE + 2,g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);
		}
	}
#endif

#ifdef EEPROM
	eeprom_read_u16(EE_KEY_DATA_BASE,&temp,1);
	eeprom_read_u16(EE_KEY_DATA_BASE + EE_KEY_DATA_SIZE - 2,&crc_temp,1);

	if(temp == 0xFFFF)
	{
		g_tkey_data.write_index = 0;
		g_tkey_data.read_index = 0;
		DataBase_Push(sys_key_data_cnt,0);
		return;
	}

    if(temp < NUM_MAX)
	{
		g_tkey_data.write_index = temp;
		g_tkey_data.read_index = 0;
		DataBase_Push(sys_key_data_cnt,temp);

		eeprom_read_u16(EE_KEY_DATA_BASE + 2,g_tkey_data.key_data_buff[0],temp * DATA_LEN);

		if(crc_temp != bsp_flash_crc(g_tkey_data.key_data_buff[0],temp * DATA_LEN))
		{
			eeprom_read_u16(EE_KEY_DATA_BK_BASE + 2,g_tkey_data.key_data_buff[0],temp * DATA_LEN);
		}
	}
	else if(temp >= NUM_MAX)
	{
		g_tkey_data.write_index = temp % NUM_MAX;
		g_tkey_data.read_index = g_tkey_data.write_index;
		DataBase_Push(sys_key_data_cnt,temp);

	    eeprom_read_u16(EE_KEY_DATA_BASE + 2,g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);


		if(crc_temp != bsp_flash_crc(g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN))
		{
			eeprom_read_u16(EE_KEY_DATA_BK_BASE + 2,g_tkey_data.key_data_buff[0],NUM_MAX * DATA_LEN);
		}
	}
#endif

}

void key_data_put(tigger_type_list reason,tigger_reason_list type)
{
	uint16_t cnt_temp;
	cnt_temp = DataBase_Get(sys_key_data_cnt);
	cnt_temp++;

	DataBase_Push(sys_key_data_cnt,cnt_temp);

	g_tkey_data.key_data_buff[g_tkey_data.write_index][YEAR] = DataBase_Get(sys_year);
    g_tkey_data.key_data_buff[g_tkey_data.write_index][MON] = DataBase_Get(sys_mon);
	g_tkey_data.key_data_buff[g_tkey_data.write_index][DAY] = DataBase_Get(sys_day);
	g_tkey_data.key_data_buff[g_tkey_data.write_index][HOUR] = DataBase_Get(sys_hour);
    g_tkey_data.key_data_buff[g_tkey_data.write_index][MIN] = DataBase_Get(sys_min);
    g_tkey_data.key_data_buff[g_tkey_data.write_index][SEC] = DataBase_Get(sys_sec);
	g_tkey_data.key_data_buff[g_tkey_data.write_index][REASON] = reason;
    g_tkey_data.key_data_buff[g_tkey_data.write_index][TYPE] = type;
	g_tkey_data.key_data_buff[g_tkey_data.write_index][CNT] = DataBase_Get(sys_power_control_board_err_num + type);

	g_tkey_data.write_index = (g_tkey_data.write_index + 1) % NUM_MAX;

	if(cnt_temp >= NUM_MAX)
	{
		g_tkey_data.read_index = g_tkey_data.write_index;
	}
#ifdef FLASH
	key_date_download(KEY_DATA_BASE,cnt_temp,KEY_DATA_SIZE);
	key_date_download(KEY_DATA_BK_BASE,cnt_temp,KEY_DATA_BK_SIZE);
#endif

#ifdef EEPROM
	key_date_download(EE_KEY_DATA_BASE,cnt_temp,EE_KEY_DATA_SIZE);
	key_date_download(EE_KEY_DATA_BK_BASE,cnt_temp,EE_KEY_DATA_BK_SIZE);
#endif

}

void key_data_get(uint8_t cnt,uint8_t *buff)
{
	for(uint8_t i = 0;i < DATA_LEN;i++)
	{
		*(buff + i) = (uint8_t)(g_tkey_data.key_data_buff[(g_tkey_data.read_index + cnt - 1) % NUM_MAX][i]);
	}
}

#endif
