/*
 * app_sensor_get.h
 *
 *  Created on: 2024年3月22日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_SENSOR_GET_H_
#define APP_APP_APP_INC_APP_SENSOR_GET_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

#define TEMP_NTC_TABLE_LEN      166
#define PRESS_TABLE_LEN 		135

#define TEMP_SENSOR_NUM   5
#define PRES_SENSOR_NUM   4


#define TEMP_INPUT_WATER             temp3
#define TEMP_OUTPUT_WATER            temp4
#define TEMP_ENVIRONMENT             temp5
#define TEMP_MOTOR_IN             	 temp2
#define TEMP_MOTOR_OUT               temp1

#define PRESS_INPUT_WATER            pres3
#define PRESS_OUTPUT_WATER           pres4
#define PRESS_IN_MOTOR               pres2
#define PRESS_OUT_MOTOR              pres1
typedef enum
{
    temp_in_water_idx = 0,
    temp_out_water_idx,
    temp_envir_idx,
    temp_in_motor_idx,
    temp_out_motor_idx,
}temp_correct_e;
typedef struct
{
    int16_t temp_in_water_correct;
    int16_t temp_out_water_correct;
    int16_t temp_environment_correct;
    int16_t temp_in_motor_correct;
    int16_t temp_out_motor_correct;
}temp_correct_s;

void app_get_sensor_handle(void);
void app_temp_correct_input(temp_correct_e index,int16_t temp);
int16_t app_temp_correct_get(temp_correct_e index);
#endif /* APP_APP_APP_INC_APP_SENSOR_GET_H_ */
