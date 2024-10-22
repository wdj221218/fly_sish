/*
 * time_cali.h
 *
 *  Created on: 2024年9月6日
 *      Author: Dell
 */

#ifndef COMMON_MID_INC_TIME_CALI_H_
#define COMMON_MID_INC_TIME_CALI_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

typedef struct
{
    u16 year;
    u8 mon;
    u8 day;
    u8 hour;
    u8 minute;
    u8 seconds;
}time_cali_s;

void time_cali_get(time_cali_s *Time);
void time_cali_set(time_cali_s *Time);
void time_cali_server(void);
#endif /* COMMON_MID_INC_TIME_CALI_H_ */
