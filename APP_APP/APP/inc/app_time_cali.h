/*
 * app_time_cali.h
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_TIME_CALI_H_
#define APP_APP_APP_INC_APP_TIME_CALI_H_

#include <stdint.h>
#include "UserLib.h"
#include "bsp.h"
#include "mid.h"

typedef struct
{
	uint16_t year;
	uint16_t mon;
	uint16_t day;
	uint16_t hour;
	uint16_t min;
	uint16_t sec;
}sys_time_str;


#endif /* APP_APP_APP_INC_APP_TIME_CALI_H_ */
