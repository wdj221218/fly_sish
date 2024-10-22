/*
 * bsp_flash.h
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_FLASH_H_
#define COMMON_BSP_INC_BSP_FLASH_H_

#include <stdint.h>
#include "apm32f10x_fmc.h"
#include "bsp.h"
#include "mid.h"
#include "app.h"

#define FBL_ADDRESS        0x08000000
#define SBL_ADDRESS        0x08002000
#define APP_ADDRESS        0x08007000
#define FLASH_SIZE_MAX     0x08080000

#define FLASH_PAGE_SIZE    2048

uint8_t bsp_flash_erase(uint32_t address,uint32_t size);
uint8_t bsp_flash_write(uint32_t address,uint16_t *data,uint16_t size);
uint8_t bsp_flash_read(uint32_t address,uint16_t *data,uint16_t size);
uint16_t bsp_flash_crc(uint16_t *data, uint16_t size);
#endif /* COMMON_BSP_INC_BSP_FLASH_H_ */
