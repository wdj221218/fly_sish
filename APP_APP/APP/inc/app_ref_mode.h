/*
 * app_ref_mode.h
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_REF_MODE_H_
#define APP_APP_APP_INC_APP_REF_MODE_H_

#include "app.h"
#include "mid.h"
#include "bsp.h"

typedef enum
{
	ref_idle = 0,
    ref_low_power_init,
	ref_normal_init,
	ref_low_power_run,
	ref_normal_run,
	ref_low_power_run_stop,
	ref_normal_run_stop,
	ref_low_power_err,
	ref_normal_err,
}ref_run_mode_state;


mode_run_state_e ref_mode_run(mode_run_state_e state_next);
uint8_t ref_mode_exit(void);

#endif /* APP_APP_APP_INC_APP_REF_MODE_H_ */
