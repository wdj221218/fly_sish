/*
 * main.c
 *
 *  Created on: 2024年3月18日
 *      Author: Dell
 */

#include "bsp.h"
#include "mid.h"
#include "app.h"

const uint32_t info_offset[2] = {SBL_INFO, APP_INFO};
const uint32_t code_offset[2] = {SBL_BASE, APP_BASE};

typedef  void (*pFunction)(void);



uint32_t headcrc32_check(uint32_t src, uint32_t dir)
{
    if(src == dir)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void move_run(void){
    file_info_s* file_info = (file_info_s*)(CODE_BK_INFO);
    /*备份区固件有效性检测*/
    uint32_t ret = headcrc32_check(file_info->head_crc32, HEAD_CRC32_ENABLE);

    if(ret)
    {
        if(file_info->address == SBL_BASE)
        {
        	bsp_flash_erase(file_info->address, SBL_SIZE * 1024);
        }
        else if(file_info->address == APP_BASE)
        {
        	bsp_flash_erase(file_info->address, APP_SIZE * 1024);
        }
        else
        {
            return;
        }

        uint32_t all_cnt = file_info->size/2;

        if(file_info->size%2 != 0)
        {
            all_cnt++;
        }

        bsp_flash_write(file_info->address, (uint16_t*)CODE_BK_BASE, all_cnt);

        uint32_t base_address = SBL_INFO;

        if(file_info->address  == APP_BASE)
        {
            base_address = APP_INFO;
        }

        bsp_flash_write(base_address, (uint16_t*)file_info, 8);
        bsp_flash_erase(CODE_BK_BASE,CODE_BK_SIZE * 1024);
        *(uint32_t*)(UPGRADE_ADDR) = 0;
    }
}

void main()
{
	uint32_t ret = 0;
    file_info_s* file_info = NULL;
    move_run();
    for (uint8_t i = 0; i< 2;i++)
    {
        file_info = (file_info_s*)(info_offset[i]);
        ret = headcrc32_check(file_info->head_crc32, HEAD_CRC32_ENABLE);

        if(ret)
        {
			uint32_t JumpAddress;
			uint32_t address = code_offset[i];
			if (((*(__IO uint32_t*)address) & 0x2FFE0000 ) == 0x20000000)
			{
				JumpAddress = *(__IO uint32_t *) (address + 4);
				pFunction Jump_To_Application = (pFunction) JumpAddress;

				__disable_irq();
				__set_PRIMASK(1);

				__set_MSP(*(__IO uint32_t *) address);
				Jump_To_Application();
			}
        }
    }
    sys_reset();
}



