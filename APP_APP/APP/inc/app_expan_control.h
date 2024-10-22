/*
 * app_expan_control.h
 *
 *  Created on: 2024年3月21日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_EXPAN_CONTROL_H_
#define APP_APP_APP_INC_APP_EXPAN_CONTROL_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

#define TIME_INTERVAL_EXPAN  30000
#define TIME_INIT_EXPAN      150000

#define PLUS_PPS             50

#define MIN_OPEN             50
#define MAX_OPEN             500

#define VACUUM_EXPAN         400

#define EXPAN_READY          1
#define EXPAN_UNREADY        0

#define A_PHASE_ON      	GPIO_SetBit(GPIOA,GPIO_PIN_15);
#define A_PHASE_OFF     	GPIO_ResetBit(GPIOA,GPIO_PIN_15);
#define B_PHASE_ON      	GPIO_SetBit(GPIOB,GPIO_PIN_3);
#define B_PHASE_OFF     	GPIO_ResetBit(GPIOB,GPIO_PIN_3);
#define C_PHASE_ON      	GPIO_SetBit(GPIOB,GPIO_PIN_10);
#define C_PHASE_OFF     	GPIO_ResetBit(GPIOB,GPIO_PIN_10);
#define D_PHASE_ON      	GPIO_SetBit(GPIOB,GPIO_PIN_11);
#define D_PHASE_OFF     	GPIO_ResetBit(GPIOB,GPIO_PIN_11);

typedef enum
{
    Forward = 0,
    Reverses,
}Dir_List;

typedef enum
{
    Sleep = 0,
    Start,
    Run,
    Stop,
}Expan_State_List;

typedef enum
{
    Init_State_Expan = 0,
    Run_State_Expan,
}Expansion_StateList;

typedef enum
{
	Start_Open = 0,
	Open_240 ,
	Close_540,
	End_Open,
}Expan_Init_StateList;

typedef enum
{
    expan_run = 0,
    expan_stop,
    expan_err,
}Expan_Mach_StateList;

typedef struct
{
    int16_t over_heat_rate;
}over_heat_data_s;

void Expansion_IO_Init(void);
uint16_t expan_open_get(void);
void app_expansion_run(uint16_t over_heat,uint16_t envir_temp,Expan_Mach_StateList state);
void app_expan_run_vacuum(uint8_t state);
void app_expan_run_manual(uint16_t open_temp);

#endif /* APP_APP_APP_INC_APP_EXPAN_CONTROL_H_ */
