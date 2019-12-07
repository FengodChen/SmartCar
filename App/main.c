#include "common.h"
#include "include.h"
#include "BJTU_func.h"



void main() {
  bjtu_init_main();
  bjtu_set_wheel_freq_all(25);
  bjtu_set_wheel_expect_speed_all(150);

  DELAY_MS(100); 
  
  //uint8 i = 0;

  while(1) {
    bjtu_main();
   /*
    if (i == 0) {
      bjtu_print_speed_states();
      //dip_print_weight();
    }
    i += 32;
*/
    //DELAY_MS(1);
  }
}