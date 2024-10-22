#include "debug_log.h"

#define LOG_HEAD LOG_ID_MID
log_ctx_s g_log_ctx;
log_ctrl_s g_log_ctrl[] =
{
	{LOG_ID_SYS_MARK, NULL},
	{LOG_ID_BSP_MARK, NULL},
	{LOG_ID_MID_MARK, NULL},
	{LOG_ID_APP_MARK, NULL},
};

extern bsp_uart_str g_tlog_RS232_Uart;

static const unsigned int crc8Table[256] =
{
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15, 0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65, 0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5, 0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85, 0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2, 0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2, 0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32, 0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42, 0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C, 0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC, 0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C, 0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x19, 0x1E, 0x17, 0x10, 0x05, 0x02, 0x0B, 0x0C, 0x21, 0x26, 0x2F, 0x28, 0x3D, 0x3A, 0x33, 0x34,
    0x4E, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5C, 0x5B, 0x76, 0x71, 0x78, 0x7F, 0x6A, 0x6D, 0x64, 0x63,
    0x3E, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2C, 0x2B, 0x06, 0x01, 0x08, 0x0F, 0x1A, 0x1D, 0x14, 0x13,
    0xAE, 0xA9, 0xA0, 0xA7, 0xB2, 0xB5, 0xBC, 0xBB, 0x96, 0x91, 0x98, 0x9F, 0x8A, 0x8D, 0x84, 0x83,
    0xDE, 0xD9, 0xD0, 0xD7, 0xC2, 0xC5, 0xCC, 0xCB, 0xE6, 0xE1, 0xE8, 0xEF, 0xFA, 0xFD, 0xF4, 0xF3
};


unsigned char calcCRC(unsigned char *data,unsigned int len)//计算CRC
{
    unsigned char crc8 = 0;

    while (len --)
    {
        crc8 = crc8 ^ (*data++);
        crc8 = crc8Table[crc8];
    }

    return crc8;
}

u8 get_check_sum(u8*data, u8 size)
{
    return calcCRC(data, size);
}

void log_uart_tp_cb(u8 *data, u16 size)
{
	if(14 != size)
	{
		return;
	}

	if(get_check_sum(data,13) != data[13])
	{
		return;
	}

	bsp_uart_log_s *req = (bsp_uart_log_s*)data;
	WDBG(LOG_ID_SUB_MID_LOG,"id:0x%x\n", req->id);
	switch(req->id)
	{
		case 0x180701f4:
		{
            g_log_ctrl[3].mark = req->cmd[0] | 7;
            g_log_ctrl[2].mark = req->cmd[1] | 7;
            g_log_ctrl[1].mark = req->cmd[2] | 7;
            g_log_ctrl[0].mark = req->cmd[3] | 7;
		}
		break;

		case 0x180801f4:
		{
			WDBG(LOG_ID_SUB_MID_LOG," sub id:%d\n", req->cmd[0]);

			switch(req->cmd[0])
			{
				case 0:

				break;

				case 1:

				break;

				case 2:

				break;

				case 3:

				break;
				case 4:

				break;

				case 5:

				break;
			}
		}
		break;
	}
}

uint32_t dev_dbg_get_mark(uint16_t log_id, uint16_t log_sub_id) 
{
    if(log_id >= LOG_ID_MAX) 
    {
        return 0;
    }
    uint32_t mark = (g_log_ctrl[log_id].mark & log_sub_id)?1:0;
    return mark;
}

u32 get_can_msg(u32 canid, u8* data,u8 size, ctrl_msg_s*ctrl)
{
    if(data== NULL | size > 8 | ctrl == NULL)
    {
        return 0;
    }
    ctrl->data[0] = (size + 4)<<4;
    u8*temp = (u8*)&canid;
    for(u8 i =0; i< 4; i++)
    {
        ctrl->data[1 + i] = temp[i];
    }

    for(u8 i =0; i< size; i++)
    {
        ctrl->data[5 + i] = data[i];
    }
    ctrl->data[13] = get_check_sum(ctrl->data, 13);
    return 0;
}

void wdbg_printf(uint8_t log_id, uint16_t sub_id, const char * format, ... ) 
{
    if(dev_dbg_get_mark(log_id, sub_id) == 0) 
    {
        return;
    } 
    va_list args;
    uint32_t len;
    va_start(args, format);
    len = vsnprintf ((char*)g_log_ctx.cache_buf, LOG_TEMP_SIZE, format, args);
    va_end (args);

    if(len <= 0)
    {
    	return;
    }

	u8* temp = g_log_ctx.cache_buf;
	while(len--)
	{
		if(g_log_ctx.all_size < LOG_CACHE_SIZE)
		{
			g_log_ctx.send_buf[g_log_ctx.w_idx] = *temp;
			g_log_ctx.w_idx = (g_log_ctx.w_idx + 1 == LOG_CACHE_SIZE)?0:(g_log_ctx.w_idx + 1);
			g_log_ctx.all_size++;
		}
		temp++;
	}
}

void wdbg_dump(uint8_t log_id, uint16_t sub_id, uint8_t*data, uint16_t size) 
{
    for(uint16_t i= 0; i< size;i++)
    {
        wdbg_printf(log_id,sub_id, "%02x ", data[i]);
        if((i+1)%16 == 0)
        {
            wdbg_printf(log_id,sub_id, "\n");
        }
    }
    wdbg_printf(log_id,sub_id, "\n");
}

void log_send(u8 *buff,u8 size)
{
	Per_UARTSend(&g_tlog_RS232_Uart,buff,size);
}

void mid_log_print_server(void)
{
	if(g_log_ctx.all_size <= 0)
	{
		return;
	}

	u8 data[8];
	u8 cnt =0;
	ctrl_msg_s ctrl;
	for(u8 i =0; i < 8; i++)
	{
		if(g_log_ctx.all_size> 0)
		{
			data[i] = g_log_ctx.send_buf[g_log_ctx.r_idx];
			g_log_ctx.r_idx = (g_log_ctx.r_idx + 1 == LOG_CACHE_SIZE)?0:(g_log_ctx.r_idx + 1);
			cnt++;
			g_log_ctx.all_size--;
		}
		else
		{
			break;
		}
	}

	get_can_msg(0x1806ff01, data, cnt, &ctrl);
	log_send((u8*)&ctrl, 14);//发送日志

}

void mid_log_uart_init(void)
{

}





