#include "mid_key.h"
key_fifo_s    s_tkey_fifo;
key_param_s   s_tkey_param[kid_max] = 
{
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
    {0,     FILTER_CNT,    0,      0,      LONG_CNT,   0,      REPEAT_CNT},
}

void mid_key_code_put(key_code_enum key_code)
{
    if(key_code > key_code_max)
    {
        return;
    }

    s_tkey_fifo.key_buff[s_tkey_fifo.w_idx] = key_code;
    ++s_tkey_fifo.w_idx %= KEY_FIFO_SIZE;
}

key_code_enum mid_key_code_get(void)
{
    key_code_enum ret;
    if(s_tkey_fifo.r_idx == s_tkey_fifo.w_idx)
    {
        return key_none;
    }

    ret = s_tkey_fifo.key_buff[s_tkey_fifo.r_idx];
    ++s_tkey_fifo.r_idx %= KEY_FIFO_SIZE;
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
        if(s_tkey_param[id].fliter_cnt < s_tkey_param[id].filter_time)
        {
            s_tkey_param[id].fliter_cnt = s_tkey_param[id].filter_time;
        }
        else if(s_tkey_param[id].fliter_cnt < 2 * s_tkey_param[id].filter_time)
        {
            s_tkey_param[id].fliter_cnt++;
        }
        else
        {
            if(0 == s_tkey_param[id].state)
            {
                s_tkey_param[id].state = 1;
                mid_key_code_put(id * 3 + 1);
            }
            
            if(s_tkey_param[id].long_time > 0)
            {
                if(s_tkey_param[id].long_cnt < s_tkey_param[id].long_time)
                {
                    if(++s_tkey_param[id].long_cnt >= s_tkey_param[id].long_time)
                    {
                        mid_key_code_put(id * 3 + 3);
                    }
                }
                else
                {
                    if(++s_tkey_param[id].repeat_long_cnt >= s_tkey_param[id].repeat_long_period)
                    {
                        s_tkey_param[id].repeat_long_cnt = 0;
                        mid_key_code_put(id * 3 + 3);
                    }
                }
            }
        }
    }
    else
    {
        if(s_tkey_param[id].fliter_cnt > s_tkey_param[id].filter_time)
        {
            s_tkey_param[id].fliter_cnt = s_tkey_param[id].filter_time;
        }
        else if(0 != s_tkey_param[id].fliter_cnt)
        {
            s_tkey_param[id].fliter_cnt--;
        }
        else
        {
            if(1 == s_tkey_param[id].state)
            {
                s_tkey_param[id].state = 0;
                mid_key_code_put(id * 3 + 2);
            }
        }
        s_tkey_param[id].repeat_long_cnt = 0;
        s_tkey_param[id].long_cnt = 0;
    }
}

void mid_key_scan(void)
{
    for(u8 i = 0; i < kid_max; i++)
    {
        mid_key_code_detect(i);
    }
}


