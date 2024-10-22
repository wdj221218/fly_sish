/*
 * app_heat_mode.h
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_HEAT_MODE_H_
#define APP_APP_APP_INC_APP_HEAT_MODE_H_

#include "app.h"
#include "mid.h"
#include "bsp.h"

mode_run_state_e heat_mode_run(mode_run_state_e state_next);

uint8_t heat_mode_exit(void);

#endif /* APP_APP_APP_INC_APP_HEAT_MODE_H_ */
