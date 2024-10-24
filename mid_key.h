#ifndef MID_KEY_H
#define MID_KEY_H

#define KEY_FIFO_SIZE   10  

#define TASK_PERIOD     2

#define FILTER_TIME     10
#define FILTER_CNT      FILTER_TIME/TASK_PERIOD

#define LONG_TIME       2000
#define LONG_CNT        LONG_TIME/TASK_PERIOD

#define REPEAT_TIME     100
#define REPEAT_CNT      REPEAT_TIME/TASK_PERIOD

typedef enum
{
	kid_k1 = 0,
	kid_k2,
	kid_k3,
	kid_k4,
    kid_max,
}keyid_e;

typedef enum
{
	key_none = 0,	

	k_1_down,				
	k_1_up,				    
	k_1_long,				

	k_2_down,				
	k_2_up,				    
	k_2_long,				

	k_3_down,				
	k_3_up,				    
	k_3_long,				

	k_4_down,				
	k_4_up,				    
	k_4_long,				
}key_code_enum;

typedef struct 
{
    u8  key_buff[KEY_FIFO_SIZE];
    u8  w_idx;
    u8  r_idx;
}key_fifo_s;

typedef struct 
{
    u8  fliter_cnt;
    u8  filter_time
    u8  state;
    u16 long_cnt;
    u16 long_time;
    u16 repeat_long_cnt;
    u16 repeat_long_period;
}key_param_s;

#endif