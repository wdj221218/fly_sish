/*
 * bsp_timer.h
 *
 *  Created on: 2024年3月18日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_TIMER_H_
#define COMMON_BSP_INC_BSP_TIMER_H_

#include "stdio.h"
#include "apm32f10x_tmr.h"
#include "bsp.h"

#define PWM_TIM_RCM         RCM_APB2_PERIPH_TMR8
#define PWM_IO_RCM          RCM_APB2_PERIPH_GPIOC
#define PWM_TIM             TMR8
#define PWM_IO_PORT         GPIOC
#define PWM_TIM_FRE         200
#define PWM_TIM_PERIOD      100 - 1
#define PWM_TIM_DIV         SystemCoreClock/(PWM_TIM_FRE * (PWM_TIM_PERIOD + 1)) - 1

#define PUMP_IO_PIN         GPIO_PIN_8
#define PUMP_CHN            TMR_CHANNEL_3

#define PCS_PUMP_IO_PIN     GPIO_PIN_9
#define PCS_PUMP_CHN        TMR_CHANNEL_4

#define FAN_IO_PIN			GPIO_PIN_7
#define FAN_CHN        		TMR_CHANNEL_2





typedef struct
{
    uint8_t Channel;
    uint16_t Lo_Lvl_Ticks;
    uint16_t Hi_Lvl_Ticks;
    uint16_t Last_Tick;
    float Peroid;           //s
    float Freq;				//hz
    float Duty;				//%
}pwm_cap_str;

void bsp_pump_pwm_init(void);
void bsp_fan_pwm_init(void);
void bsp_set_pump_duty(uint8_t duty);
void bsp_set_fan_duty(uint8_t duty);
void bsp_set_pump_pcs_duty(uint8_t duty);
#endif /* COMMON_BSP_INC_BSP_TIMER_H_ */
