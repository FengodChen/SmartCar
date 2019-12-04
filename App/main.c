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
    
  while(1) {
    bjtu_refresh_battle_states();
    bjtu_refresh_wheel_now_speed();
    //bjtu_print_battle_states();
    //bjtu_print_speed_states();
    
    DELAY_MS(1);
    //steering_start_turn(1000);
    //uint16 i = 2000;

    if (key_check(KEY_AHEAD) == KEY_DOWN) {    
      bjtu_refresh_camera();
      bjtu_debug_1();
      bjtu_oled_show_camera();
      led_turn(LED0);
      DELAY_MS(500);
    }
    else if (key_check(KEY_BACK) == KEY_DOWN) {
      //bjtu_set_wheel_back_all();
      led_turn(LED1);
      //i += 500;
      steering_start_turn(800);
      DELAY_MS(500);
    }
    
  }
}