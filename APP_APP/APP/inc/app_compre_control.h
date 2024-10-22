/*
 * app_compre_control.h
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_COMPRE_CONTROL_H_
#define APP_APP_APP_INC_APP_COMPRE_CONTROL_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

#define TIME_INTERVAL_COMPRESS  60000
#define TIME_INIT_COMPRESS      180000

#define FRE_START_MAX           50

#define FC1_MAX					540
#define FC2_MIN					540
#define FC2_MAX					640
#define FC3_MIN					640
#define FC3_MAX					700
#define FC4_MIN					700
#define FC4_MAX					780
#define FC5_MIN					780
#define FC5_MAX					860
#define FC6_MIN					860

typedef enum
{
    Init_State_Compre = 0,
    Run_State_Compre,
}Compressor_StateList;

typedef enum
{
    compre_run = 0,
    compre_stop,
    compre_err,
}Compre_Mach_StateList;

typedef enum
{
    normal = 0,
    start,
    speed_change,
	end,
}start_in_statelist;

typedef struct
{
    int16_t temp_err_last;
    int16_t temp_err_now;
    int16_t temp_err_rate;
}temp_data_s;

void compressor_off_call_back(void);
void app_compressor_run(uint16_t temp_envir,uint16_t temp_now,Compre_Mach_StateList state_mach);
void app_motor_run_time(void);
#endif /* APP_APP_APP_INC_APP_COMPRE_CONTROL_H_ */
