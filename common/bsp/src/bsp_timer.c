/*
 * bsp_timer.c
 *
 *  Created on: 2024年3月18日
 *      Author: Dell
 */

#include "bsp_timer.h"

void bsp_pwm_init(void)
{
    GPIO_Config_T GPIO_ConfigStruct;
    TMR_BaseConfig_T TMR_TimeBaseStruct;
    TMR_OCConfig_T OCCongigStruct;

    RCM_EnableAPB2PeriphClock(PWM_IO_RCM);
    RCM_EnableAPB2PeriphClock(PWM_TIM_RCM);

	GPIO_ConfigStructInit(&GPIO_ConfigStruct);
	GPIO_ConfigStruct.pin = PUMP_IO_PIN | PCS_PUMP_IO_PIN | FAN_IO_PIN;
	GPIO_ConfigStruct.mode = GPIO_MODE_AF_PP;
	GPIO_ConfigStruct.speed = GPIO_SPEED_50MHz;
	GPIO_Config(PWM_IO_PORT, &GPIO_ConfigStruct);

	TMR_TimeBaseStruct.clockDivision = TMR_CLOCK_DIV_1;
	TMR_TimeBaseStruct.countMode = TMR_COUNTER_MODE_UP;
	TMR_TimeBaseStruct.division = PWM_TIM_DIV;
    TMR_TimeBaseStruct.period = PWM_TIM_PERIOD;
	TMR_ConfigTimeBase(PWM_TIM, &TMR_TimeBaseStruct);

    OCCongigStruct.idleState = TMR_OC_IDLE_STATE_SET;
    OCCongigStruct.mode = TMR_OC_MODE_PWM2;
    OCCongigStruct.nIdleState = TMR_OC_NIDLE_STATE_RESET;
    OCCongigStruct.nPolarity = TMR_OC_NPOLARITY_LOW;
    OCCongigStruct.outputNState = TMR_OC_NSTATE_ENABLE;
    OCCongigStruct.outputState = TMR_OC_STATE_ENABLE;
    OCCongigStruct.polarity = TMR_OC_POLARITY_LOW;
    OCCongigStruct.pulse = 0;

    TMR_ConfigOC2(PWM_TIM, &OCCongigStruct);
    TMR_ConfigOC3(PWM_TIM, &OCCongigStruct);
    TMR_ConfigOC4(PWM_TIM, &OCCongigStruct);

	TMR_EnablePWMOutputs(PWM_TIM);
	TMR_Enable(PWM_TIM);
}



void bsp_set_pump_duty(uint8_t duty)
{
	if(duty > 100)
	{
		return; 
	}

    switch(PUMP_CHN)
    {
        case TMR_CHANNEL_1:
            TMR_ConfigCompare1(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_2:
            TMR_ConfigCompare2(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_3:
            TMR_ConfigCompare3(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_4:
            TMR_ConfigCompare4(PWM_TIM, duty);
        break;

        default:
        break;
    }
}

void bsp_set_pump_pcs_duty(uint8_t duty)
{
	if(duty > 100)
	{
		return;
	}

    switch(PCS_PUMP_CHN)
    {
        case TMR_CHANNEL_1:
            TMR_ConfigCompare1(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_2:
            TMR_ConfigCompare2(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_3:
            TMR_ConfigCompare3(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_4:
            TMR_ConfigCompare4(PWM_TIM, duty);
        break;

        default:
        break;
    }
}


void bsp_set_fan_duty(uint8_t duty)
{
    if(duty > 100)
    {
        return; 
    }

    switch(FAN_CHN)
    {
        case TMR_CHANNEL_1:
            TMR_ConfigCompare1(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_2:
            TMR_ConfigCompare2(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_3:
            TMR_ConfigCompare3(PWM_TIM, duty);
        break;

        case TMR_CHANNEL_4:
            TMR_ConfigCompare4(PWM_TIM, duty);
        break;

        default:
        break;
    }
}



