/*
 * app_uart.c
 *
 *  Created on: 2024年3月12日
 *      Author: Dell
 */
#include "app_uart.h"

extern bsp_uart_str g_tRs485_Uart;
extern bsp_uart_str g_tISORS485_Uart;
extern bsp_uart_str g_tRS232_Uart;

uint8_t uart_temp[2048];

void MODS_Slave_Handle(uint8_t *buff,uint16_t len)
{
	MODS_Data_Get();
	modbus_cmd_analyze(buff,len);
	MODS_Data_Push();
}

void MODS_Master_Handle(uint8_t *buff,uint16_t len)
{
	modbus_cmd_analyze_master(buff,len);
}

void PC_Handle(uint8_t *buff,uint16_t len)
{
	pc_cmd_analyze(buff,len);
}

void RS485_UART_Handle(bsp_uart_str *pUart)
{
	uint16_t crc;
	uint16_t crc_temp;
	uint16_t len_temp;

	if(bsp_timer_check(TIMEOUT_RS485) == 0)
	{
		return;
	}

	if(pUart->RxCount < 4)
	{
		pUart->RxCount = 0;
		pUart->RxWrite = 0;
		return;
	}

	if(pUart->RxCount > UART_RX_BUF_SIZE)
	{
		pUart->RxCount = UART_RX_BUF_SIZE;
	}

	__set_PRIMASK(1);

	BufferCopy(uart_temp,pUart->pRxBuf,pUart->RxCount);
	len_temp = pUart->RxCount;
	pUart->RxCount = 0;
	pUart->RxWrite = 0;
	
	__set_PRIMASK(0);

	if(uart_temp[0] == SlaveAddr)
	{
		crc_temp = uart_temp[len_temp - 2] << 8 | uart_temp[len_temp - 1];
		crc = CRC16_Modbus(uart_temp,len_temp - 2);

		if(crc_temp != crc)
		{
			return;
		}

		MODS_Master_Handle(uart_temp,len_temp);
	}
}

void ISORS485_UART_Handle(bsp_uart_str *pUart)
{
	uint16_t crc;
	uint16_t crc_temp;
	uint16_t len_temp;

	if(bsp_timer_check(TIMEOUT_ISO_RS485) == 0)
	{
		return;
	}

	if(pUart->RxCount < 4)
	{
		pUart->RxCount = 0;
		pUart->RxWrite = 0;
		return;
	}

	if(pUart->RxCount > UART_RX_BUF_SIZE)
	{
		pUart->RxCount = UART_RX_BUF_SIZE;
	}

	__set_PRIMASK(1);

	BufferCopy(uart_temp,pUart->pRxBuf,pUart->RxCount);
	len_temp = pUart->RxCount;
	pUart->RxCount = 0;
	pUart->RxWrite = 0;

	__set_PRIMASK(0);

	if(uart_temp[0] == DataBase_Get(sys_rs485_add))
	{
		crc_temp = uart_temp[len_temp - 1] | uart_temp[len_temp - 2] << 8;
		crc = CRC16_Modbus(uart_temp,len_temp - 2);

		if(crc_temp != crc)
		{
			return;
		}

		MODS_Slave_Handle(uart_temp,len_temp);
	}
}

void RS232_UART_Handle(bsp_uart_str *pUart)
{
	uint16_t crc;
	uint16_t crc_temp;
	uint16_t len_temp;

	if(bsp_timer_check(TIMEOUT_RS232) == 0)
	{
		return;
	}

	if(pUart->RxCount < 4)
	{
		pUart->RxCount = 0;
		pUart->RxWrite = 0;
		return;
	}

	if(pUart->RxCount > UART_RX_BUF_SIZE)
	{
		pUart->RxCount = UART_RX_BUF_SIZE;
	}

	__set_PRIMASK(1);

	BufferCopy(uart_temp,pUart->pRxBuf,pUart->RxCount);
	len_temp = pUart->RxCount;
	pUart->RxCount = 0;
	pUart->RxWrite = 0;
	__set_PRIMASK(0);

	if(uart_temp[0] == HOST_PROTOCOL_HEAD)
	{
		crc_temp = uart_temp[len_temp - 3] | uart_temp[len_temp - 2] << 8;
		crc = CRC16_Modbus(uart_temp,len_temp - 3);

		if(crc_temp != crc)
		{
			return;
		}
		PC_Handle(uart_temp,len_temp);
	}
}

void log_RS232_UART_Handle(bsp_uart_str *pUart)
{
	uint16_t crc;
	uint16_t crc_temp;
	uint16_t len_temp;

	if(bsp_timer_check(LOG_TIMEOUT_RS232) == 0)
	{
		return;
	}

	if(pUart->RxCount < 4)
	{
		pUart->RxCount = 0;
		pUart->RxWrite = 0;
		return;
	}

	if(pUart->RxCount > UART_RX_BUF_SIZE)
	{
		pUart->RxCount = UART_RX_BUF_SIZE;
	}

	__set_PRIMASK(1);

	BufferCopy(uart_temp,pUart->pRxBuf,pUart->RxCount);
	len_temp = pUart->RxCount;
	pUart->RxCount = 0;
	pUart->RxWrite = 0;
	__set_PRIMASK(0);

	log_uart_tp_cb(uart_temp,len_temp);
}


void app_uart_handle(void)
{
	RS485_UART_Handle(&g_tRs485_Uart);
	ISORS485_UART_Handle(&g_tISORS485_Uart);
	RS232_UART_Handle(&g_tRS232_Uart);
}





