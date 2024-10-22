/*
 * bsp_wdt.h
 *
 *  Created on: 2024年3月18日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_WDT_H_
#define COMMON_BSP_INC_BSP_WDT_H_


#include "apm32f10x_wwdt.h"
#include "apm32f10x_rcm.h"
#include "apm32f10x_misc.h"
#include "stdio.h"

void bsp_wdt_init(void);
void WDT_Feed(void);
void WDT_Disable(void);



#endif /* COMMON_BSP_INC_BSP_WDT_H_ */
