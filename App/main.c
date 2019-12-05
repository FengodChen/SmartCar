#include "common.h"
#include "include.h"
#include "BJTU_func.h"

#define KEY_AHEAD KEY_START
#define KEY_BACK KEY_B

void main() {
  bjtu_init_main();
    
  key_init(KEY_AHEAD);
  key_init(KEY_BACK);

  bjtu_set_wheel_freq_all(0);

  DELAY_MS(1000);

  //bjtu_main();
    
  while(1) {
    bjtu_refresh_battle_states();
    bjtu_refresh_wheel_now_speed();
    //bjtu_print_battle_states();
    //bjtu_print_speed_states();
    bjtu_main();
    DELAY_MS(1);

    if (key_check(KEY_AHEAD) == KEY_DOWN) { 
      led_turn(LED0);
      bjtu_set_steering_turn(TURN_LEFT, 50);
      bjtu_turn_steering();
      DELAY_MS(500);
    }
    else if (key_check(KEY_BACK) == KEY_DOWN) {
      led_turn(LED1);
      bjtu_set_steering_turn(TURN_RIGHT, 50);
      bjtu_turn_steering();
      DELAY_MS(500);
    }
    
  }
}