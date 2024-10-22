/*
 * bsp_uart.h
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */

#ifndef COMMON_BSP_INC_BSP_UART_H_
#define COMMON_BSP_INC_BSP_UART_H_

#include "stdint.h"
#include "apm32f10x_usart.h"
#include "apm32f10x_gpio.h"
#include "apm32f10x_rcm.h"
#include "apm32f10x_misc.h"
#include "bsp.h"
//#include "mid.h"

/*******************非隔离485相关引脚定义*******************************/
#define RS485_UART_RCM       RCM_APB1_PERIPH_UART4
#define RS485_TR_IO_RCM      RCM_APB2_PERIPH_GPIOC
#define RS485_TX_PORT		 GPIOC
#define RS485_TX_PIN         GPIO_PIN_10
#define RS485_RX_PORT		 GPIOC
#define RS485_RX_PIN         GPIO_PIN_11

#define RS485_TEN_IO_RCM     RCM_APB2_PERIPH_GPIOC
#define RS485_TXEN_PORT      GPIOC
#define RS485_TXEN_PIN       GPIO_PIN_12
#define RS485_RX_EN()		 GPIO_ResetBit(RS485_TXEN_PORT,RS485_TXEN_PIN)
#define RS485_TX_EN()		 GPIO_SetBit(RS485_TXEN_PORT,RS485_TXEN_PIN)

#define RS485_3_3CTR_IO_RCM  RCM_APB2_PERIPH_GPIOD
#define RS485_3_3CTRL_PORT   GPIOD
#define RS485_3_3CTRL_PIN    GPIO_PIN_0
#define RS485_3_3CTRL_EN()	 GPIO_SetBit(RS485_3_3CTRL_PORT,RS485_3_3CTRL_PIN)
#define RS485_3_3CTRL_DIS()	 GPIO_ResetBit(RS485_3_3CTRL_PORT,RS485_3_3CTRL_PIN)

/********************隔离485相关引脚定义*******************************/
#define ISO_RS485_UART_RCM      RCM_APB1_PERIPH_USART2  //重定义
#define ISO_RS485_TR_IO_RCM     RCM_APB2_PERIPH_GPIOD|RCM_APB2_PERIPH_AFIO
#define ISO_RS485_TX_PORT		GPIOD
#define ISO_RS485_TX_PIN     	GPIO_PIN_5
#define ISO_RS485_RX_PORT		GPIOD
#define ISO_RS485_RX_PIN     	GPIO_PIN_6

#define ISO_RS485_TXEN_IO_RCM  	RCM_APB2_PERIPH_GPIOD
#define ISO_RS485_TXEN_PORT   	GPIOD
#define ISO_RS485_TXEN_PIN    	GPIO_PIN_7
#define ISO_RS485_RX_EN()		GPIO_ResetBit(ISO_RS485_TXEN_PORT,ISO_RS485_TXEN_PIN)
#define ISO_RS485_TX_EN()		GPIO_SetBit(ISO_RS485_TXEN_PORT,ISO_RS485_TXEN_PIN)

#define ISO_RS485_3_3CTR_IO_RCM  RCM_APB2_PERIPH_GPIOB
#define ISO_RS485_3_3CTRL_PORT   GPIOB
#define ISO_RS485_3_3CTRL_PIN    GPIO_PIN_5
#define ISO_RS485_3_3CTRL_EN()	 GPIO_SetBit(ISO_RS485_3_3CTRL_PORT,ISO_RS485_3_3CTRL_PIN)
#define ISO_RS485_3_3CTRL_DIS()	 GPIO_ResetBit(ISO_RS485_3_3CTRL_PORT,ISO_RS485_3_3CTRL_PIN)

#define ISO_RS485_5CTR_IO_RCM  	 RCM_APB2_PERIPH_GPIOB
#define ISO_RS485_5CTRL_PORT     GPIOB
#define ISO_RS485_5CTRL_PIN      GPIO_PIN_4
#define ISO_RS485_5CTRL_EN()	 GPIO_SetBit(ISO_RS485_5CTRL_PORT,ISO_RS485_5CTRL_PIN)
#define ISO_RS485_5CTRL_DIS()	 GPIO_ResetBit(ISO_RS485_5CTRL_PORT,ISO_RS485_5CTRL_PIN)

/*************************232相关引脚定义*******************************/
#define RS232_UART_RCM       RCM_APB2_PERIPH_USART1
#define RS232_TR_IO_RCM      RCM_APB2_PERIPH_GPIOA
#define RS232_TX_PORT		 GPIOA
#define RS232_TX_PIN         GPIO_PIN_9
#define RS232_RX_PORT		 GPIOA
#define RS232_RX_PIN         GPIO_PIN_10

#define RS232_3_3CTR_IO_RCM  RCM_APB2_PERIPH_GPIOD
#define RS232_3_3CTRL_PORT   GPIOD
#define RS232_3_3CTRL_PIN    GPIO_PIN_1
#define RS232_3_3CTRL_EN()	 GPIO_SetBit(RS232_3_3CTRL_PORT,RS232_3_3CTRL_PIN)
#define RS232_3_3CTRL_DIS()	 GPIO_ResetBit(RS232_3_3CTRL_PORT,RS232_3_3CTRL_PIN)

/*************************log232相关引脚定义*******************************/
#define LOG_RS232_UART_RCM       RCM_APB1_PERIPH_USART3
#define LOG_RS232_TR_IO_RCM      RCM_APB2_PERIPH_GPIOD
#define LOG_RS232_TX_PORT		 GPIOD
#define LOG_RS232_TX_PIN         GPIO_PIN_8
#define LOG_RS232_RX_PORT		 GPIOD
#define LOG_RS232_RX_PIN         GPIO_PIN_9



#define UART_TX_BUF_SIZE 2048
#define UART_RX_BUF_SIZE 2048

typedef struct 
{
	USART_T  *uart;     							
	uint8_t  *pTxBuf;									 
	uint8_t  *pRxBuf;									 
	uint16_t  TxBufSize;		          
	uint16_t  RxBufSize;		          
	uint32_t  BaudRate;							
	
    uint16_t TxWrite;			     
    uint16_t TxRead;			     
    uint16_t TxCount;			     

    uint16_t RxWrite;			   
    uint16_t RxRead;			      
    uint16_t RxCount;			    

    void (*SendBefor)(void);          
    void (*SendOver)(void); 	         
    void (*ReciveNew)(void);
}bsp_uart_str;

void bsp_uart_init(void);
void Per_UARTSend(bsp_uart_str *pUart,uint8_t *Buff, uint16_t Len);
#endif /* COMMON_BSP_INC_BSP_UART_H_ */
