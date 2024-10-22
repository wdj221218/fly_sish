/*
 * flash_alloca.h
 *
 *  Created on: 2024年3月11日
 *      Author: Dell
 */

#ifndef COMMON_MID_INC_FLASH_ALLOCA_H_
#define COMMON_MID_INC_FLASH_ALLOCA_H_

#include "stdint.h"

#define FLASH_SIZE           512

#define FBL_BASE             0x08000000
#define FBL_SIZE             8

#define SBL_BASE             0x08002000
#define SBL_SIZE             20
#define SBL_INFO			 0x08006FF0
#define SBL_VERSION    		 SBL_BASE + 0x144


#define APP_BASE             0x08007000
#define APP_SIZE             200
#define APP_INFO			 0x08038FF0

#define CODE_BK_BASE         0x08039000
#define CODE_BK_SIZE         200
#define CODE_BK_INFO         0x0806AFF0

#define PARAM_BASE           0x0806B000
#define PARAM_SIZE           4

#define PARAM_BK_BASE        0x0806C000
#define PARAM_BK_SIZE        4

#define KEY_DATA_BASE        0x0806D000
#define KEY_DATA_SIZE        4

#define KEY_DATA_BK_BASE     0x0806E000
#define KEY_DATA_BK_SIZE     4

#define UPGRADE_ADDR         0x2001FFFB
#define UPGRADE_VALUE        0x5A5A5A5A

#define APP_VALID            0xA5A5A5A5

#define HEAD_CRC32_ENABLE    0XA5A5A5A5

#define EE_PARAM_BASE        0x00
#define EE_PARAM_SIZE        0x100

#define EE_PARAM_BK_BASE     0x100
#define EE_PARAM_BK_SIZE     0x100

#define EXPAN_OPEN_H         0//0x400 - 3
#define EXPAN_OPEN_L         1//0X400 - 2
#define EXPAN_WRITE_FLAG     2//0x400 - 1

#define EE_KEY_DATA_BASE     0x400
#define EE_KEY_DATA_SIZE     0x200

#define EE_KEY_DATA_BK_BASE  0x600
#define EE_KEY_DATA_BK_SIZE  0x200

typedef struct
{
    uint32_t address;
    uint32_t size;
    uint32_t file_crc32;
    uint32_t head_crc32;
} file_info_s;


#endif /* COMMON_MID_INC_FLASH_ALLOCA_H_ */
