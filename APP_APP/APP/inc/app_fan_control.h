/*
 * app_fan_control.h
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_FAN_CONTROL_H_
#define APP_APP_APP_INC_APP_FAN_CONTROL_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

#define TIME_INTERVAL_FAN  60000

#define FAN1_FB_RCM		    RCM_APB2_PERIPH_GPIOE
#define FAN1_FB_PORT 		GPIOE
#define FAN1_FB_PIN  		GPIO_PIN_2

#define FAN2_FB_RCM		    RCM_APB2_PERIPH_GPIOE
#define FAN2_FB_PORT 		GPIOE
#define FAN2_FB_PIN  		GPIO_PIN_6

#define FAN3_FB_RCM		    RCM_APB2_PERIPH_GPIOE
#define FAN3_FB_PORT 		GPIOE
#define FAN3_FB_PIN  		GPIO_PIN_14

typedef enum
{
    Init_State_Fan = 0,
    Run_State_Fan,
    Stop_Status_Fan,
	Err_State_Fan,
}Fan_StateList;

typedef struct
{
	uint8_t io_now;
	uint8_t io_last;
	uint16_t plus_num;
	uint16_t rpm;
}fan_speeds;

void app_fan_run(uint16_t temp_envir,uint16_t out_pres_trans_temp,Fan_StateList state);
void app_get_fan_fb(void);
uint8_t app_get_fan_duty(void);
void app_fan_run_low_power(uint16_t temp_now,Fan_StateList state);
#endif /* APP_APP_APP_INC_APP_FAN_CONTROL_H_ */
