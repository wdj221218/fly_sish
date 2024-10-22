/*
 * bsp_uart.c
 *
 *  Created on: 2024年3月7日
 *      Author: Dell
 */
#include "bsp_uart.h"

bsp_uart_str g_tRs485_Uart;
bsp_uart_str g_tISORS485_Uart;
bsp_uart_str g_tRS232_Uart;
bsp_uart_str g_tlog_RS232_Uart;

uint8_t RS485_TxBuff[UART_TX_BUF_SIZE];
uint8_t RS485_RxBuff[UART_RX_BUF_SIZE];

uint8_t ISO_RS485_TxBuff[UART_TX_BUF_SIZE];
uint8_t ISO_RS485_RxBuff[UART_RX_BUF_SIZE];

uint8_t RS232_TxBuff[UART_TX_BUF_SIZE];
uint8_t RS232_RxBuff[UART_RX_BUF_SIZE];

uint8_t log_RS232_TxBuff[UART_TX_BUF_SIZE];
uint8_t log_RS232_RxBuff[UART_RX_BUF_SIZE];

uint8_t SendComplete(void)
{
	if(USART_ReadIntFlag(USART1, USART_INT_TXC) != RESET)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void RS485_SendBefor(void)		 
{
	uint32_t i = 10000;
	
	RS485_TX_EN();	
	
	while(i--);
}

void RS485_SendOver(void)			
{
	RS485_RX_EN();	
}

void RS485_ReciveNew(void)
{		
	bsp_once_timer_start(TIMEOUT_RS485,10);
}

void ISO_RS485_SendBefor(void)
{
	uint32_t i = 20000;

	ISO_RS485_TX_EN();

	while(i--);
}

void ISO_RS485_SendOver(void)
{
	ISO_RS485_RX_EN();
}

void ISO_RS485_ReciveNew(void)
{
	bsp_once_timer_start(TIMEOUT_ISO_RS485,20);
}

void RS232_SendBefor(void)
{

}

void RS232_SendOver(void)
{

}

void RS232_ReciveNew(void)
{

	bsp_once_timer_start(TIMEOUT_RS232,10);
}

void log_RS232_SendBefor(void)
{

}

void log_RS232_SendOver(void)
{

}

void log_RS232_ReciveNew(void)
{

	bsp_once_timer_start(LOG_TIMEOUT_RS232,10);
}

void RS485_Para_Init(bsp_uart_str *pUart)
{
	pUart->uart = UART4;
	pUart->pRxBuf = RS485_RxBuff;
	pUart->pTxBuf = RS485_TxBuff;
	pUart->RxBufSize = UART_RX_BUF_SIZE;
	pUart->TxBufSize = UART_TX_BUF_SIZE;
	pUart->TxWrite = 0;
	pUart->TxRead = 0;
	pUart->RxWrite = 0;
	pUart->RxRead = 0;
	pUart->RxCount = 0;
	pUart->TxCount = 0;
	pUart->BaudRate = 2400;
	pUart->SendBefor = RS485_SendBefor;
	pUart->SendOver = RS485_SendOver;
	pUart->ReciveNew = RS485_ReciveNew;
}

void RS485_IO_Init(bsp_uart_str *pUart)
{

	GPIO_Config_T		gpioConfig;
	USART_Config_T 	uartConfig;

	RCM_EnableAPB1PeriphClock(RS485_UART_RCM);
	RCM_EnableAPB2PeriphClock(RS485_TR_IO_RCM);
    RCM_EnableAPB2PeriphClock(RS485_TEN_IO_RCM);
	RCM_EnableAPB2PeriphClock(RS485_3_3CTR_IO_RCM);

	gpioConfig.pin  = RS485_3_3CTRL_PIN;
	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(RS485_3_3CTRL_PORT,&gpioConfig);

	gpioConfig.pin  = RS485_TXEN_PIN;
	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(RS485_TXEN_PORT,&gpioConfig);

	gpioConfig.pin  = RS485_TX_PIN;
	gpioConfig.mode = GPIO_MODE_AF_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(RS485_TX_PORT,&gpioConfig);

	gpioConfig.pin  = RS485_RX_PIN;
	gpioConfig.mode = GPIO_MODE_IN_FLOATING;
	GPIO_Config(RS485_RX_PORT,&gpioConfig);
	

	uartConfig.baudRate = pUart->BaudRate;
	uartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
	uartConfig.mode = USART_MODE_TX_RX;		
	uartConfig.parity = USART_PARITY_NONE;
	uartConfig.stopBits = USART_STOP_BIT_1;
	uartConfig.wordLength = USART_WORD_LEN_8B;
	USART_Config(pUart->uart,&uartConfig);
	USART_Enable(pUart->uart);

	RS485_3_3CTRL_EN();
	RS485_RX_EN();

	NVIC_EnableIRQRequest(UART4_IRQn, 1, 0);
	USART_EnableInterrupt(pUart->uart,USART_INT_RXBNE);
	USART_ClearIntFlag(pUart->uart,USART_INT_TXC);
}

void ISO_RS485_Para_Init(bsp_uart_str *pUart)
{
	pUart->uart = USART2;
	pUart->pRxBuf = ISO_RS485_RxBuff;
	pUart->pTxBuf = ISO_RS485_TxBuff;
	pUart->RxBufSize = UART_RX_BUF_SIZE;
	pUart->TxBufSize = UART_TX_BUF_SIZE;
	pUart->TxWrite = 0;
	pUart->TxRead = 0;
	pUart->RxWrite = 0;
	pUart->RxRead = 0;
	pUart->RxCount = 0;
	pUart->TxCount = 0;

    switch(DataBase_Get(sys_rs485_bd))
    {
    	case 0:
    		pUart->BaudRate = 9600;
    	break;

       	case 1:
        	pUart->BaudRate = 4800;
        break;

       	case 2:
        	pUart->BaudRate = 19200;
        break;

        default:
    	    pUart->BaudRate = 9600;
    	break;
    }
	pUart->SendBefor = ISO_RS485_SendBefor;
	pUart->SendOver = ISO_RS485_SendOver;
	pUart->ReciveNew = ISO_RS485_ReciveNew;
}

void ISO_RS485_IO_Init(bsp_uart_str *pUart)
{
	GPIO_Config_T		gpioConfig;
	USART_Config_T 	uartConfig;

	RCM_EnableAPB1PeriphClock(ISO_RS485_UART_RCM);
	RCM_EnableAPB2PeriphClock(ISO_RS485_TR_IO_RCM);
    RCM_EnableAPB2PeriphClock(ISO_RS485_TXEN_IO_RCM);
	RCM_EnableAPB2PeriphClock(ISO_RS485_3_3CTR_IO_RCM);
	RCM_EnableAPB2PeriphClock(ISO_RS485_5CTR_IO_RCM);

	GPIO_ConfigPinRemap(GPIO_REMAP_USART2);

	gpioConfig.pin  = ISO_RS485_3_3CTRL_PIN;
	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(ISO_RS485_3_3CTRL_PORT,&gpioConfig);

	gpioConfig.pin  = ISO_RS485_5CTRL_PIN;
	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(ISO_RS485_5CTRL_PORT,&gpioConfig);

	gpioConfig.pin  = ISO_RS485_TXEN_PIN;
	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(ISO_RS485_TXEN_PORT,&gpioConfig);

	gpioConfig.pin  = ISO_RS485_TX_PIN;
	gpioConfig.mode = GPIO_MODE_AF_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(ISO_RS485_TX_PORT,&gpioConfig);

	gpioConfig.pin  = ISO_RS485_RX_PIN;
	gpioConfig.mode = GPIO_MODE_IN_FLOATING;
	GPIO_Config(ISO_RS485_RX_PORT,&gpioConfig);


	uartConfig.baudRate = pUart->BaudRate;
	uartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
	uartConfig.mode = USART_MODE_TX_RX;
	uartConfig.parity = USART_PARITY_NONE;
	uartConfig.stopBits = USART_STOP_BIT_1;
	uartConfig.wordLength = USART_WORD_LEN_8B;
	USART_Config(pUart->uart,&uartConfig);
	USART_Enable(pUart->uart);

	ISO_RS485_3_3CTRL_EN();
	ISO_RS485_5CTRL_EN();
	ISO_RS485_RX_EN();

	NVIC_EnableIRQRequest(USART2_IRQn, 0, 2);

	USART_EnableInterrupt(pUart->uart,USART_INT_RXBNE);


	USART_ClearIntFlag(pUart->uart,USART_INT_TXC);
}

void RS232_Para_Init(bsp_uart_str *pUart)
{
	pUart->uart = USART1;
	pUart->pRxBuf = RS232_RxBuff;
	pUart->pTxBuf = RS232_TxBuff;
	pUart->RxBufSize = UART_RX_BUF_SIZE;
	pUart->TxBufSize = UART_TX_BUF_SIZE;
	pUart->TxWrite = 0;
	pUart->TxRead = 0;
	pUart->RxWrite = 0;
	pUart->RxRead = 0;
	pUart->RxCount = 0;
	pUart->TxCount = 0;
    pUart->BaudRate = 9600;
	pUart->SendBefor = RS232_SendBefor;
	pUart->SendOver = RS232_SendOver;
	pUart->ReciveNew = RS232_ReciveNew;
}

void RS232_IO_Init(bsp_uart_str *pUart)
{
	GPIO_Config_T		gpioConfig;
	USART_Config_T 	uartConfig;

	RCM_EnableAPB2PeriphClock(RS232_UART_RCM);
	RCM_EnableAPB2PeriphClock(RS232_TR_IO_RCM);
    RCM_EnableAPB2PeriphClock(RS232_3_3CTR_IO_RCM);

	gpioConfig.pin  = RS232_3_3CTRL_PIN;
	gpioConfig.mode = GPIO_MODE_OUT_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(RS232_3_3CTRL_PORT,&gpioConfig);

	gpioConfig.pin  = RS232_TX_PIN;
	gpioConfig.mode = GPIO_MODE_AF_PP;//GPIO_MODE_AF_PP
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(RS232_TX_PORT,&gpioConfig);

	gpioConfig.pin  = RS232_RX_PIN;
	gpioConfig.mode = GPIO_MODE_IN_FLOATING;
	GPIO_Config(RS232_RX_PORT,&gpioConfig);

	RS232_3_3CTRL_EN();

	uartConfig.baudRate = pUart->BaudRate;
	uartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
	uartConfig.mode = USART_MODE_TX_RX;
	uartConfig.parity = USART_PARITY_NONE;
	uartConfig.stopBits = USART_STOP_BIT_1;
	uartConfig.wordLength = USART_WORD_LEN_8B;
	USART_Config(pUart->uart,&uartConfig);
	USART_Enable(pUart->uart);

  	NVIC_EnableIRQRequest(USART1_IRQn, 1, 1);
	USART_EnableInterrupt(pUart->uart,USART_INT_RXBNE);
	USART_ClearIntFlag(pUart->uart,USART_INT_TXC);
}

void LOG_RS232_Para_Init(bsp_uart_str *pUart)
{
	pUart->uart = USART3;
	pUart->pRxBuf = log_RS232_RxBuff;
	pUart->pTxBuf = log_RS232_TxBuff;
	pUart->RxBufSize = UART_RX_BUF_SIZE;
	pUart->TxBufSize = UART_TX_BUF_SIZE;
	pUart->TxWrite = 0;
	pUart->TxRead = 0;
	pUart->RxWrite = 0;
	pUart->RxRead = 0;
	pUart->RxCount = 0;
	pUart->TxCount = 0;
    pUart->BaudRate = 115200;
	pUart->SendBefor = log_RS232_SendBefor;
	pUart->SendOver = log_RS232_SendOver;
	pUart->ReciveNew = log_RS232_ReciveNew;
}

void log_RS232_IO_Init(bsp_uart_str *pUart)
{
	GPIO_Config_T	gpioConfig;
	USART_Config_T 	uartConfig;

	RCM_EnableAPB1PeriphClock(LOG_RS232_UART_RCM);
	RCM_EnableAPB2PeriphClock(LOG_RS232_TR_IO_RCM);

	GPIO_ConfigPinRemap(GPIO_FULL_REMAP_USART3);

	gpioConfig.pin  = LOG_RS232_TX_PIN;
	gpioConfig.mode = GPIO_MODE_AF_PP;
	gpioConfig.speed = GPIO_SPEED_50MHz;
	GPIO_Config(LOG_RS232_TX_PORT,&gpioConfig);

	gpioConfig.pin  = LOG_RS232_RX_PIN;
	gpioConfig.mode = GPIO_MODE_IN_FLOATING;
	GPIO_Config(LOG_RS232_RX_PORT,&gpioConfig);

	uartConfig.baudRate = pUart->BaudRate;
	uartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
	uartConfig.mode = USART_MODE_TX_RX;
	uartConfig.parity = USART_PARITY_NONE;
	uartConfig.stopBits = USART_STOP_BIT_1;
	uartConfig.wordLength = USART_WORD_LEN_8B;
	USART_Config(pUart->uart,&uartConfig);
	USART_Enable(pUart->uart);


  	NVIC_EnableIRQRequest(USART3_IRQn, 1, 2);
	USART_EnableInterrupt(pUart->uart,USART_INT_RXBNE);
	USART_ClearIntFlag(pUart->uart,USART_INT_TXC);
}

void bsp_uart_init(void)
{
	RS485_Para_Init(&g_tRs485_Uart);
	ISO_RS485_Para_Init(&g_tISORS485_Uart);
	RS232_Para_Init(&g_tRS232_Uart);
	LOG_RS232_Para_Init(&g_tlog_RS232_Uart);
	RS485_IO_Init(&g_tRs485_Uart);
	ISO_RS485_IO_Init(&g_tISORS485_Uart);
	RS232_IO_Init(&g_tRS232_Uart);
	log_RS232_IO_Init(&g_tlog_RS232_Uart);
}

void UARTSend(bsp_uart_str *pUart,uint8_t *Buff, uint16_t Len)
{
	uint16_t i;

	for(i = 0;i < Len; i++)
	{
			while (1)
			{
				__IO uint16_t usCount;

				__set_PRIMASK(1);
				usCount = pUart->TxCount;
				__set_PRIMASK(0);

				if (usCount < pUart->TxBufSize)
				{
					break;
				}
			}

			pUart->pTxBuf[pUart->TxWrite] = Buff[i];

			__set_PRIMASK(1);
			if (++pUart->TxWrite >= pUart->TxBufSize)
			{
				pUart->TxWrite = 0;
			}
			pUart->TxCount++;
			__set_PRIMASK(0);
	}
	USART_EnableInterrupt(pUart->uart,USART_INT_TXBE);
}

void Per_UARTSend(bsp_uart_str *pUart,uint8_t *Buff, uint16_t Len)
{

	if(pUart == 0)
	{
		return;
	}
	if(pUart->SendBefor != 0)
	{
     pUart->SendBefor();
	}
	UARTSend(pUart,Buff,Len);
}

void UartIRQ(bsp_uart_str *pUart)
{
	uint32_t temp;

	if (USART_ReadIntFlag(pUart->uart, USART_INT_OVRE) != RESET)
	{
		temp = pUart->uart->STS;
		temp = USART_RxData(pUart->uart);
	}

	if (USART_ReadIntFlag(pUart->uart, USART_INT_RXBNE) != RESET)
	{
		uint8_t ch;

		pUart->RxCount++;
		ch = (uint8_t)USART_RxData(pUart->uart);
		pUart->pRxBuf[pUart->RxWrite] = ch;

		pUart->RxWrite++;
		pUart->RxWrite = pUart->RxWrite % pUart->RxBufSize;

		if (pUart->ReciveNew)
		{
			pUart->ReciveNew();
		}

		USART_ClearIntFlag(pUart->uart,USART_INT_RXBNE);
	}

	if (USART_ReadIntFlag(pUart->uart, USART_INT_TXBE) != RESET)
	{
		if (pUart->TxCount == 0)
		{
			USART_DisableInterrupt(pUart->uart, USART_INT_TXBE);

			USART_EnableInterrupt(pUart->uart, USART_INT_TXC);
		}
		else
		{
			USART_TxData(pUart->uart,pUart->pTxBuf[pUart->TxRead]);
			if (++pUart->TxRead >= pUart->TxBufSize)
			{
				pUart->TxRead = 0;
			}
			pUart->TxCount--;
		}
	}

	else if (USART_ReadIntFlag(pUart->uart, USART_INT_TXC) != RESET)
	{
		if (pUart->TxCount == 0)
		{
			USART_DisableInterrupt(pUart->uart, USART_INT_TXC);
			if (pUart->SendOver)
			{
				pUart->SendOver();
			}
		}
		else
		{
			USART_TxData(pUart->uart,pUart->pTxBuf[pUart->TxRead]);
			if (++pUart->TxRead >= pUart->TxBufSize)
			{
				pUart->TxRead = 0;
			}
			pUart->TxCount--;
		}

		USART_ClearIntFlag(pUart->uart,USART_INT_TXC);
	}	
}

void UART4_IRQHandler(void)
{
	UartIRQ(&g_tRs485_Uart);
}
void USART2_IRQHandler(void)
{
	UartIRQ(&g_tISORS485_Uart);
}
void USART1_IRQHandler(void)
{
	UartIRQ(&g_tRS232_Uart);
}

void USART3_IRQHandler(void)
{
	UartIRQ(&g_tlog_RS232_Uart);
}

