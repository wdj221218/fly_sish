/*
 * app_valve_set.h
 *
 *  Created on: 2024年8月30日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_VALVE_CONTROL_H_
#define APP_APP_APP_INC_APP_VALVE_CONTROL_H_


#include "bsp.h"
#include "mid.h"
#include "app.h"

typedef enum
{
	valve_close = 0,
	valve_open,
}valve_state_e;

void app_valve_set(valve_state_e state);
valve_state_e app_valve_get(void);


#endif /* APP_APP_APP_INC_APP_VALVE_CONTROL_H_ */
