#include "common.h"
#include "include.h"
#include "BJTU_func.h"



void main() {
  bjtu_init_main();
  bjtu_set_wheel_freq_all(15);
  bjtu_set_wheel_expect_speed_all(250);

  DELAY_MS(100); 
  
  while(1) {
    bjtu_main(); 
  }
}