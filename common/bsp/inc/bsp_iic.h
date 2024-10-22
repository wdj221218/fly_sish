/*
 * bsp_iic.h
 *
 *  Created on: 2024年4月30日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_IIC_H_
#define COMMON_BSP_INC_BSP_IIC_H_

#include "bsp.h"

#define ADD_WRITE   0xA0
#define ADD_READ    0xA1

#define SDA_PORT    GPIOB
#define SCL_PORT    GPIOB

#define SDA_PIN     GPIO_PIN_7
#define SCL_PIN     GPIO_PIN_6
 
#define SDA_SET     GPIO_SetBit(SDA_PORT,SDA_PIN)
#define SDA_CLR     GPIO_ResetBit(SDA_PORT,SDA_PIN)

#define SCL_SET     GPIO_SetBit(SCL_PORT,SCL_PIN)
#define SCL_CLR     GPIO_ResetBit(SCL_PORT,SCL_PIN)

#define GET_SDA     GPIO_ReadInputBit(SDA_PORT,SDA_PIN)

void bsp_iic_init(void);
void bsp_iic_start(void);
void bsp_iic_stop(void);
uint8_t bsp_iic_wait_ack(void);
void bsp_iic_ack(void);
void bsp_iic_nack(void);
void bsp_iic_send_byte(uint8_t txd);
uint8_t bsp_iic_read_byte(uint8_t ack);


#endif /* COMMON_BSP_INC_BSP_IIC_H_ */
