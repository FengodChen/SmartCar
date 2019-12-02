#include "common.h"
#include "include.h"
#include "BJTU_func.h"

enum {GO_AHEAD, GO_BACK};

#define KEY_AHEAD KEY_START
#define KEY_BACK KEY_STOP

static battle_states bs;
static speed_states ss;

void main() {
  bjtu_init_main();
    
  key_init(KEY_AHEAD);
  key_init(KEY_BACK);
    
  bjtu_set_wheel_freq_left(20);
  bjtu_set_wheel_freq_right(40);
    
  while(1) {
    bs = bjtu_get_battle_states();
    ss = bjtu_get_speed_states();
    
    bjtu_print_battle_states(bs);
    bjtu_print_speed_states(ss);

    //printf("Hello, world!\n");
        
    if (key_check(KEY_AHEAD) == KEY_DOWN)
      bjtu_set_wheel_ahead_all();
    else if (key_check(KEY_BACK) == KEY_DOWN)
      bjtu_set_wheel_back_all();
    DELAY_MS(100);
  }
}