/*
 * bsp_iic.c
 *
 *  Created on: 2024年4月30日
 *      Author: Dell
 */
#include "bsp_iic.h"
uint8_t iic_delay_time = 5;


void bsp_iic_delay(void)  
{   
    volatile u32 i = 0; //这里可以优化速度    ，经测试最低到5还能写入

    while(iic_delay_time--)
    { 
        for(i = 0;i < 3;i++)
        {
            __NOP();
        } 
    } 
}

void bsp_iic_init(void)
{
    GPIO_Config_T gpioConfig;
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);

    gpioConfig.mode = GPIO_MODE_OUT_OD;
    gpioConfig.pin = SDA_PIN;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(SDA_PORT,&gpioConfig);

    gpioConfig.mode = GPIO_MODE_OUT_OD;
    gpioConfig.pin = SCL_PIN;
    gpioConfig.speed = GPIO_SPEED_50MHz;
    GPIO_Config(SCL_PORT,&gpioConfig);

    SDA_SET;
    SCL_SET;
}

void bsp_iic_start(void)
{
    SDA_SET;        
    SCL_SET;
    bsp_iic_delay();
    SDA_CLR;
    bsp_iic_delay();
    SCL_CLR;
}

void bsp_iic_stop(void)
{
    SCL_CLR;
    SDA_CLR;
    bsp_iic_delay();
    SCL_SET; 
    SDA_SET;
    bsp_iic_delay();
}

uint8_t bsp_iic_wait_ack(void)
{
    uint8_t ucErrTime = 0;
 
    SDA_SET;
    bsp_iic_delay();     
    SCL_SET;
    bsp_iic_delay();   
    while(GET_SDA)
    {
        ucErrTime++;
        if(ucErrTime > 250)
        {
        	bsp_iic_stop();
            return 1;
        }
    }
    SCL_CLR;//时钟输出0      
    return 0;  
}

void bsp_iic_ack(void)
{
    SCL_CLR;
    SDA_CLR;
    bsp_iic_delay(); 
    SCL_SET;
    bsp_iic_delay(); 
    SCL_CLR;
}

void bsp_iic_nack(void)
{
    SCL_CLR;
    SDA_SET;
    bsp_iic_delay(); 
    SCL_SET;
    bsp_iic_delay(); 
    SCL_CLR;
}

void bsp_iic_send_byte(uint8_t txd)
{
    uint8_t t;       
    SCL_CLR;//拉低时钟开始数据传输
    for(t = 0;t < 8;t++)
    {              
        if((txd & 0x80) >> 7)
        {
            SDA_SET;
        }
        else
        {
            SDA_CLR;
        }
        txd <<= 1;      
        bsp_iic_delay();    
        SCL_SET;
        bsp_iic_delay();  
        SCL_CLR;  
        bsp_iic_delay(); 
    }   
}

uint8_t bsp_iic_read_byte(uint8_t ack)
{
    uint8_t i,receive = 0;
    for(i = 0;i < 8;i++)
    {
        SCL_CLR; 
        bsp_iic_delay();
        SCL_SET;
        receive <<= 1;
        if(GET_SDA)
        {
            receive++;      
        }
        bsp_iic_delay(); 
    }                    
    if(0 == ack)
    {
        bsp_iic_nack();//发送nACK
    }
    else
    {
        bsp_iic_ack(); //发送ACK   
    }
    return receive;
}



