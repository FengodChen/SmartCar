#include "common.h"
#include "include.h"
#include "BJTU_func.h"

#define KEY_AHEAD KEY_START
#define KEY_BACK KEY_B

/*
void PIT0_IRQHandler(void)
{
    led_turn(LED0);             //闪烁 LED0
    
    PIT_Flag_Clear(PIT0);       //清中断标志位
}
*/

void main() {
  bjtu_init_main();

  //OLED_ShowChar(2,2,'A',12,0);
  //OLED_Refresh_Gram();
    
  key_init(KEY_AHEAD);
  key_init(KEY_BACK);

  bjtu_set_wheel_freq_all(0);
  
  //gpio_set(PTE5, 0);
  //DELAY_US(2000);
  //gpio_set(PTE5, 1);
    
  while(1) {
    bjtu_refresh_battle_states();
    bjtu_refresh_wheel_now_speed();
    bjtu_print_battle_states();
    bjtu_print_speed_states();
    
    //steering_start_turn(1000);
    //uint16 i = 2000;

    if (key_check(KEY_AHEAD) == KEY_DOWN) {
      //bjtu_set_wheel_ahead_all();
      led_turn(LED0);
      //if (i > 500)
      //  i -= 500;
      steering_start_turn(STEERING_MIDDLE);
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