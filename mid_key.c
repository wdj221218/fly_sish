#include "mid_key.h"

key_param_s   s_tkey_param[kid_max] = 
{
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
}

u8 mid_get_key_value(keyid_e id)
{
    if(id > kid_k4)
    {
        return;
    }

    switch (id)
    {
        case kid_k1:
            return 0;
        break;

        case kid_k2:
            return 0;
        break;        
        
        case kid_k3:
            return 0;
        break; 

        case kid_k4:
            return 0;
        break;
        
        default:
            return 0;
        break;
    }
}

u8 mid_get_key_state(keyid_e id) 
{
    u8 cnt;
    u8 save;

    if(id > kid_k4)
    {
        return;
    }

    for(u8 i = 0; i < kid_max;i++)
    {
        if(1 == mid_get_key_value(i))
        {
            cnt++;
            save = i;
        }
    }

    if((1 == cnt) && (id == save))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

u8 mid_key_code_detect(keyid_e id)
{
    if(id > kid_k4)
    {
        return;
    }

    if(1 == mid_get_key_state(id))
    {
        s_tkey_param[id]
    }
    else
    {
        
    }

}




