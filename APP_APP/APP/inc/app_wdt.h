/*
 * app_wdt.h
 *
 *  Created on: 2024年5月21日
 *      Author: Dell
 */

#ifndef APP_APP_APP_INC_APP_WDT_H_
#define APP_APP_APP_INC_APP_WDT_H_

#include "apm32f10x_wwdt.h"
#include "apm32f10x_rcm.h"
#include "apm32f10x_misc.h"
#include "stdio.h"

void app_wdt_init(void);
void app_wdt_feed(void);
void app_wdt_disable(void);


#endif /* APP_APP_APP_INC_APP_WDT_H_ */
