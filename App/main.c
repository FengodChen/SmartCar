#include "common.h"
#include "include.h"
#include "BJTU_func.h"



void main() {
  bjtu_init_main();
  bjtu_set_wheel_freq_all(25);
  bjtu_set_wheel_expect_speed_all(600);

  DELAY_MS(50); 

  while(1) {
    bjtu_main();
    bjtu_print_speed_states();
    DELAY_MS(1);
  }
}