#include "common.h"
#include "include.h"
#include "BJTU_func.h"

enum {GO_AHEAD, GO_BACK};
#define KEY_AHEAD KEY_START
#define KEY_BACK KEY_STOP

void main() {
  battle_states bs;
  speed_states ss;
  
  bjtu_init_adc();
  bjtu_init_uart();
  bjtu_init_wheel();
  bjtu_init_encoder();
    
  key_init(KEY_AHEAD);
  key_init(KEY_BACK);
    
  bjtu_set_wheel_freq_left(20);
  bjtu_set_wheel_freq_right(20);
    
  while(1) {
    bs = bjtu_get_battle_states();
    ss = bjtu_get_speed_states();
    
    bjtu_print_battle_states(bs);
    bjtu_print_speed_states(ss);
      
      
    //printf("Hello, world!\n");
        
    if (key_check(KEY_AHEAD) == KEY_DOWN)
      bjtu_set_wheel_ahead();
    else if (key_check(KEY_BACK) == KEY_DOWN)
      bjtu_set_wheel_back();
    DELAY_MS(100);
    //gpio_set(PTE5, 0);
    //DELAY_MS(1);
    //gpio_set(PTE5, 1);
  }
}