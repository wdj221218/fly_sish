/*
 * app_temp_hold.h
 *
 *  Created on: 2024年3月25日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_TEMP_HOLD_H_
#define APP_APP_APP_INC_APP_TEMP_HOLD_H_

#include "bsp.h"
#include "mid.h"
#include "app.h"

#define HOLD_READY        1
#define HOLD_UNREADY      0

typedef enum
{
    HoldTemp_Init = 0,
    HoldTemp_Heat,
    HoldTemp_Ref,
    HoldTemp_Hold,
}Hold_Temp_List;

void Hold_Temp_State_Machine(uint16_t target_temp,uint16_t now_temp);

#endif /* APP_APP_APP_INC_APP_TEMP_HOLD_H_ */
