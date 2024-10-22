/*
 * eeprom.h
 *
 *  Created on: 2024年4月30日
 *      Author: Dell
 */

#ifndef COMMON_MID_INC_EEPROM_H_
#define COMMON_MID_INC_EEPROM_H_

#include "bsp.h"
#include "mid.h"


void bl24c02_init(void);
uint8_t bl24c02_readonebyte(uint16_t addr);
void bl24c02_writeonebyte(uint16_t addr,uint8_t data);

void param_write_eeprom(uint32_t start_addr,uint16_t *buff,uint16_t size);
uint8_t param_read_eeprom(uint32_t start_addr,uint16_t *buff);


#endif /* COMMON_MID_INC_EEPROM_H_ */
