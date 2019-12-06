#include "common.h"
#include "include.h"
#include "BJTU_func.h"



void main() {
  bjtu_init_main();
  bjtu_set_wheel_freq_all(25);

  DELAY_MS(1000); 

  while(1) {
    bjtu_main();
    DELAY_MS(1);
  }
}