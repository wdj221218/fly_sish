/*
 * eeprom.c
 *
 *  Created on: 2024年4月30日
 *      Author: Dell
 */
#include "eeprom.h"

void bl24c02_init(void)
{
    bsp_iic_init();
}

uint8_t bl24c02_readonebyte(uint16_t addr)
{                 
    uint8_t temp=0;                                                                               
    bsp_iic_start();  
    bsp_iic_send_byte(ADD_WRITE);     
    bsp_iic_wait_ack(); 
    bsp_iic_send_byte(addr);   
    bsp_iic_wait_ack();     

    bsp_iic_start();           
    bsp_iic_send_byte(ADD_READ);            
    bsp_iic_wait_ack();  
    temp = bsp_iic_read_byte(0);         
    bsp_iic_stop();     
    return temp;
}

void bl24c02_writeonebyte(uint16_t addr,uint8_t data)
{                                                                                            
    bsp_iic_start();  
    bsp_iic_send_byte(ADD_WRITE);     
    bsp_iic_wait_ack();    
    bsp_iic_send_byte(addr);   
    bsp_iic_wait_ack();                                                        
    bsp_iic_send_byte(data);                             
    bsp_iic_wait_ack();                    
    bsp_iic_stop(); 
    app_wdt_feed();
    bsp_delay_ms(10);
}

void eeprom_write_u16(uint16_t start_addr,uint16_t *buff,uint16_t size)
{
    for (uint8_t i = 0; i < size; ++i)
    {
        bl24c02_writeonebyte(i * 2,(uint8_t)(*(buff + i) >> 8));
        bl24c02_writeonebyte(i * 2 + 1,(uint8_t)(*(buff + i)));
    }
}

void eeprom_read_u16(uint16_t start_addr,uint16_t *buff,uint16_t size)
{
    for (uint8_t i = 0; i < size; ++i)
    {
       *(buff + i) =  (uint16_t)bl24c02_readonebyte(i * 2) << 8 | bl24c02_readonebyte(i * 2) + 1;
    }
}

void param_write_eeprom(uint32_t start_addr,uint16_t *buff,uint16_t size)
{
    uint16_t size_temp = size;
    uint16_t crc_temp;   

    crc_temp = bsp_flash_crc(buff,size);
    eeprom_write_u16(start_addr,&size_temp,1);
    eeprom_write_u16(start_addr + EE_PARAM_SIZE - 2,&crc_temp,1);
    eeprom_write_u16(start_addr + 2,buff,size_temp);
}

uint8_t param_read_eeprom(uint32_t start_addr,uint16_t *buff)
{
    uint16_t data_size = 0;
    uint16_t check_crc = 0;
    uint16_t crc_temp;

    eeprom_read_u16(start_addr,&data_size,1);
    eeprom_read_u16(start_addr + EE_PARAM_SIZE - 2,&crc_temp,1);

    if((data_size == 0xFFFF) || (data_size > FLASH_PARAM_MAX_NUM))
    {
        return 1;
    }

    eeprom_read_u16(start_addr + 2,buff,data_size);

    check_crc = bsp_flash_crc(buff,data_size);

    if(check_crc != crc_temp)
    {
      return 1;
    }
    return 0;  
}
