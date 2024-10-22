/*
 * app_self_cir.c
 *
 *  Created on: 2024年6月17日
 *      Author: Dell
 */
#include "app_self_cir_mode.h"
extern unit_run_state_s g_tunit_run_state;
void self_cir_mode_run(void)
{
    app_pump_run_cir(DataBase_Get(sys_bms_temp_min),DataBase_Get(sys_bms_temp_max));
    g_tunit_run_state.unit_mode_state = unit_mode_cir;
}

void auto_init(void)
{
    app_pump_run_auto_init();
    g_tunit_run_state.unit_run_state = unit_run_run;
}

uint8_t self_cir_mode_exit()
{
    return 1;
}

