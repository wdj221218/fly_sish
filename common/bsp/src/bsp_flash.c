/*
 * bsp_flash.c
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */
#include "bsp_flash.h"

uint16_t bsp_flash_crc(uint16_t *data, uint16_t size)
{
	uint16_t pflash_crc = 0;
	uint16_t i = 0;
	pflash_crc += size;
	for (i = 0; i < size; i++)
	{
		pflash_crc += data[i];
	}
	pflash_crc ^= 0xFFFF;
	return pflash_crc;
}

uint8_t bsp_flash_erase(uint32_t address,uint32_t size)
{
    uint8_t all_cnt = 0;
    uint8_t earse_cnt;
    volatile FMC_STATUS_T FLASHStatus = FMC_STATUS_COMPLETE;

    if((address < SBL_ADDRESS ) | (address + size > FLASH_SIZE_MAX))
    {
        return 1;
    }

    FMC_ClearStatusFlag(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE | FMC_FLAG_OBE);

    all_cnt = size / FLASH_PAGE_SIZE;

    if(size % FLASH_PAGE_SIZE)
    {
        all_cnt++;
    }

    __set_PRIMASK(1); 
    FMC_Unlock();

    for(earse_cnt = 0; (earse_cnt < all_cnt) && (FLASHStatus == FMC_STATUS_COMPLETE); earse_cnt++)
    {
        FLASHStatus = FMC_ErasePage(address + (FLASH_PAGE_SIZE * earse_cnt));
    }

    FMC_Lock();
    __set_PRIMASK(0);

    if(FLASHStatus != FMC_STATUS_COMPLETE)
    {
        return 1;
    }

    return 0;
}

uint8_t bsp_flash_write(uint32_t address,uint16_t *data,uint16_t size)
{
    uint16_t cnt = 0;
    uint16_t *temp;
    FMC_ClearStatusFlag(FMC_FLAG_OC | FMC_FLAG_PE | FMC_FLAG_WPE | FMC_FLAG_OBE);

    if((address < FBL_ADDRESS ) || (address + size > FLASH_SIZE_MAX))
    {
        return 1;
    }

    __set_PRIMASK(1); 
    FMC_Unlock();

    temp = data;
    for(cnt = 0;cnt < size;cnt++)
    {
        if(*(uint16_t*)address != 0xffff)
        {
            return 1;
        }
        if ((FMC_ProgramHalfWord(address,temp[cnt]) != FMC_STATUS_COMPLETE) || (*(uint16_t*)address != temp[cnt]))
        {
            return 1;
        }
        address += 2;
    }

    FMC_Lock();
    __set_PRIMASK(0);

    return 0;
}

uint8_t bsp_flash_read(uint32_t address,uint16_t *data,uint16_t size)
{
    uint16_t i = 0;

    if((address < FBL_ADDRESS ) | (address + size > FLASH_SIZE_MAX))
    {
        return 1;
    }

    for(i = 0;i < size;i++)
    {
       *(data + i) =  *(uint16_t *)(address + i * 2);
    }

    return 0;
}


