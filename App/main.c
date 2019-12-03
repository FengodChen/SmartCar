#include "common.h"
#include "include.h"
#include "BJTU_func.h"

#define KEY_AHEAD KEY_START
#define KEY_BACK KEY_A


void main() {
  bjtu_init_main();
    
  key_init(KEY_AHEAD);
  key_init(KEY_BACK);
    
  bjtu_set_wheel_freq_left(15);
  bjtu_set_wheel_freq_right(15);
    
  while(1) {
    bjtu_refresh_battle_states();
    bjtu_refresh_wheel_now_speed();
    bjtu_print_battle_states();
    bjtu_print_speed_states();
    
    /*
    if (key_check(KEY_AHEAD) == KEY_DOWN)
      bjtu_set_wheel_ahead_all();
    else if (key_check(KEY_BACK) == KEY_DOWN)
      bjtu_set_wheel_back_all();
    DELAY_MS(1);
    */
  }
}