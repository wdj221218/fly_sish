/*
 * app_pump_control.h
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_PUMP_CONTROL_H_
#define APP_APP_APP_INC_APP_PUMP_CONTROL_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

#define TIME_INTERVAL_PUMP  30000

void app_pump_run_cir(uint16_t bms_min,uint16_t bms_max);
void app_pump_run_ref_heat(void);
void app_pump_off(void);
uint8_t app_get_pump_duty(void);

#endif /* APP_APP_APP_INC_APP_PUMP_CONTROL_H_ */
