/*
 * app_ptc_control.h
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_PTC_CONTROL_H_
#define APP_APP_APP_INC_APP_PTC_CONTROL_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

#define TIME_INTERVAL_PTC  60000

typedef enum
{
	PTC_OFF = 0,
    PTC_ON,
    PTC_ERR,
}PTC_Status_List;

void app_ptc_control(PTC_Status_List status);
uint8_t app_get_ptc(void);
void app_ptc_control_manual(uint8_t status);
void app_ptc_run_time(void);
#endif /* APP_APP_APP_INC_APP_PTC_CONTROL_H_ */
