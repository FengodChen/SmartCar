/* Copyright Notice:
 * 
 * Copyright (c) 2019, Liao Chen
 * @School Beijing Jiaotong University
 * @ID 17211401
 * 
 * This header file used to BJTU Smart Car.
 * It based on MK60_DZ10 program.
 * Use IAR Embedded Workbench IDE 8.32.1
 * 
 * @File BJTU_func.h BJTU_func.c
 * @Date Oct 3, 2019
 *
 */

#include "BJTU_func.h"

#define MAIN_PIT(X)     PIT0##X
#define STEERING_PIT(X) PIT1##X

static wheel_states left_wheel;
static wheel_states right_wheel;

static uint32 time_ms;
static uint32 time_needed;
static uint8 time_arrived;

static battle_states bs;

static uint8 is_running;

static uint8 imgbuff[CAMERA_SIZE];
static uint8 img[CAMERA_H*CAMERA_W];

const uint32 steering_turn_total = ((STEERING_RIGHT_MAX>STEERING_LEFT_MAX)?STEERING_RIGHT_MAX:STEERING_LEFT_MAX) - STEERING_MIDDLE;
static uint16 steering_turn;
static turn_order steering_turn_order;

static int64 delta_speed_left;
static int64 delta_speed_right;

static func_list *main_func_head;

/* ----------------- Main Function ----------------- */

static void bjtu_main_camera() {
  bjtu_refresh_camera();
  bjtu_oled_show_camera();
}

static void bjtu_main_dip() {
  dip_process(img);
}

static void bjtu_main_steering() {
  //bjtu_key_func();
  steering_turn_order = dip_get_turn_direction();
  bjtu_set_steering_turn(steering_turn_order.direction, steering_turn_order.turn_percent);
  bjtu_turn_steering();
}

static void bjtu_main_speed() {
  bjtu_refresh_wheel_now_speed();
  bjtu_print_speed_states();
  if (left_wheel.now_speed > SPEED_LOWER_THRESHOLD && right_wheel.now_speed > SPEED_LOWER_THRESHOLD && !is_running) {
      is_running = 1;
  }
  if ((left_wheel.now_speed < SPEED_LOWER_THRESHOLD || right_wheel.now_speed < SPEED_LOWER_THRESHOLD) && is_running) {
    bjtu_set_wheel_freq_all(0);
    return;
  }
  if (is_running){
    delta_speed_left = left_wheel.now_speed - left_wheel.expect_speed;
    delta_speed_right = right_wheel.now_speed - right_wheel.expect_speed;
    if (delta_speed_left > SPEED_TOLERANCE) {
      left_wheel.freq -= 1;
      if (left_wheel.freq < 0)
        left_wheel.freq = 0;
      bjtu_set_wheel_freq_left(left_wheel.freq);
    } else if (delta_speed_left < -SPEED_TOLERANCE) {
      left_wheel.freq += 1;
      if (left_wheel.freq > 100)
        left_wheel.freq = 100;
      bjtu_set_wheel_freq_left(left_wheel.freq);
    }
    if (delta_speed_right > SPEED_TOLERANCE) {
      right_wheel.freq -= 1;
      if (right_wheel.freq < 0)
        right_wheel.freq = 0;
      bjtu_set_wheel_freq_right(right_wheel.freq);
    } else if (delta_speed_right < -SPEED_TOLERANCE) {
      right_wheel.freq += 1;
      if (right_wheel.freq > 100)
        right_wheel.freq = 100;
      bjtu_set_wheel_freq_right(right_wheel.freq);
    }
  }
}

void bjtu_main() {
  if (time_arrived) {
    time_arrived = 0;
    time_ms = 0;
    time_needed = main_func_head->need_time_ms;
    main_func_head->func();
    main_func_head = main_func_head->next;
  }
}

/* ----------------- !Inner Function! ----------------- */

uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol) {
  if (gpio_out_vol == 0)
    return freq;
  else
    return 100-freq;
}

void steering_end_turn(void) {
  gpio_set(PTE5, 1);
  PIT_Flag_Clear(STEERING_PIT());
}

void steering_start_turn(uint16 time_us) {
  gpio_set(PTE5, 0);
  pit_init_us(STEERING_PIT(), time_us);
  enable_irq (STEERING_PIT(_IRQn));
}

void PORTA_IRQHandler(void) {
  uint8 n;
  uint32 flag;
  while (!PORTA_ISFR);
  flag = PORTA_ISFR;
  PORTA_ISFR  = ~0;
  n = 29;
  if(flag & (1 << n)) {
      camera_vsync();
  }
}

void DMA0_IRQHandler(void) {
    camera_dma();
}

void malloc_func_list(func_list **now) {
  (*now) = (func_list*)malloc(sizeof(func_list));
}

void timer(void) {
  ++time_ms;
  if (time_ms == time_needed)
    time_arrived = 1;
  PIT_Flag_Clear(MAIN_PIT());
  pit_init_ms(MAIN_PIT(), 1);
  enable_irq (MAIN_PIT(_IRQn));
}

/* ----------------- Initialize Function ----------------- */

void bjtu_init_func_list(void) {
  func_list *tmp_dip, *tmp_steering, *tmp_speed;
  
  malloc_func_list(&main_func_head);
  malloc_func_list(&tmp_dip);
  malloc_func_list(&tmp_steering);
  malloc_func_list(&tmp_speed);
  
  main_func_head->func = bjtu_main_camera;
  main_func_head->need_time_ms = 5;
  main_func_head->next = tmp_dip;
  
  tmp_dip->func = bjtu_main_dip;
  tmp_dip->need_time_ms = 5;
  tmp_dip->next = tmp_steering;
  
  tmp_steering->func = bjtu_main_steering;
  tmp_steering->need_time_ms = 5;
  tmp_steering->next = tmp_speed;
  
  tmp_speed->func = bjtu_main_speed;
  tmp_speed->need_time_ms = 5;
  tmp_speed->next = main_func_head;
}

void bjtu_init_key(void) {
  key_init(KEY_S2);
  key_init(KEY_S3);
}

void bjtu_init_led(void) {
  led_init(LED0);
  led_init(LED1);
  led_init(LED2);
  led_init(LED3);
}

void bjtu_init_adc(void) {
  adc_init(ADC1_SE16);
}
void bjtu_init_uart(void) {
  uart_init(UART0,115200);
}

void bjtu_init_wheel() {
  left_wheel.ahead_or_back = WHEEL_GO_AHEAD;
  right_wheel.ahead_or_back = WHEEL_GO_AHEAD;
  left_wheel.freq = right_wheel.freq = 0;
  left_wheel.expect_speed = right_wheel.expect_speed = 0;
  ftm_pwm_init(FTM0, FTM_CH3,10*1000,0); 
  ftm_pwm_init(FTM0, FTM_CH4,10*1000,0); 
  gpio_init(PTA5, GPO, left_wheel.ahead_or_back);
  gpio_init(PTA8, GPO, right_wheel.ahead_or_back);
  is_running = 0;
}

void bjtu_init_encoder(void) {
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
}

void bjtu_init_oled(void) {
  OLED_Init();
}

void bjtu_init_steering(void) {
  set_vector_handler(STEERING_PIT(_VECTORn), steering_end_turn);
  gpio_init(PTE5,GPO,1);
  steering_turn = STEERING_MIDDLE;
  steering_start_turn(STEERING_MIDDLE);
  DELAY_MS(20);
  steering_start_turn(STEERING_MIDDLE);
  DELAY_MS(20);
  steering_start_turn(STEERING_MIDDLE);
  DELAY_MS(20);
  steering_start_turn(STEERING_MIDDLE);
  DELAY_MS(20);
}

void bjtu_init_camera(void) {
  camera_init(imgbuff);
  set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);
  set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);
}

void bjtu_init_timer(void) {
  time_ms = 0;
  time_needed = 50;
  time_arrived = 0;
  set_vector_handler(MAIN_PIT(_VECTORn), timer);
  pit_init_ms(MAIN_PIT(), 1);
  enable_irq (MAIN_PIT(_IRQn));
}

void bjtu_init_main(void) {
  bjtu_init_key();
  bjtu_init_adc();
  bjtu_init_uart();
  bjtu_init_wheel();
  bjtu_init_encoder();
  bjtu_init_oled();
  bjtu_init_steering();
  bjtu_init_led();
  bjtu_init_camera();
  bjtu_init_func_list();
  bjtu_init_timer();
  dip_init_main();
}

/* ----------------- Key Function ----------------- */
int8 i = 0;
void bjtu_key_func(void) {
  if (key_check(KEY_S2) == KEY_DOWN) { 
    led_turn(LED0);
    if (i > 80)
      i = 80;
    bjtu_set_steering_turn(TURN_LEFT, i);
    bjtu_turn_steering();
    //DELAY_MS(2);
    i+=2;
  } else if (key_check(KEY_S3) == KEY_DOWN) {
    if (i < 0)
      i = 0;
    led_turn(LED1);
    bjtu_set_steering_turn(TURN_LEFT, i);
    bjtu_turn_steering();
    //DELAY_MS();
    i-=2;
  }
}

/* ----------------- Set Wheel Speed Function ----------------- */

void bjtu_set_wheel_freq_left(int8 freq) {
  if (freq > 100)
    freq = 100;
  else if (freq < 0)
    freq = 0;
  left_wheel.freq = freq;
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(freq, left_wheel.ahead_or_back));
}
void bjtu_set_wheel_freq_right(int8 freq) {
  if (freq > 100)
    freq = 100;
  else if (freq < 0)
    freq = 0;
  right_wheel.freq = freq;
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(freq, right_wheel.ahead_or_back));
}

void bjtu_set_wheel_freq_all(int8 freq) {
  bjtu_set_wheel_freq_left(freq);
  bjtu_set_wheel_freq_right(freq);
}

/* ----------------- Set Wheel Back or Ahead Function ----------------- */

void bjtu_set_wheel_back_left(void) {
  left_wheel.ahead_or_back = WHEEL_GO_BACK;
  gpio_set(PTA5, left_wheel.ahead_or_back);
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(left_wheel.freq, left_wheel.ahead_or_back));
}

void bjtu_set_wheel_back_right(void) {
  right_wheel.ahead_or_back = WHEEL_GO_BACK;
  gpio_set(PTA8, right_wheel.ahead_or_back);
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(right_wheel.freq, right_wheel.ahead_or_back));
}

void bjtu_set_wheel_back_all(void) {
  bjtu_set_wheel_back_left();
  bjtu_set_wheel_back_right();
}

void bjtu_set_wheel_ahead_left(void) {
  left_wheel.ahead_or_back = WHEEL_GO_AHEAD;
  gpio_set(PTA5, left_wheel.ahead_or_back);
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(left_wheel.freq, left_wheel.ahead_or_back));
}

void bjtu_set_wheel_ahead_right(void) {
  right_wheel.ahead_or_back = WHEEL_GO_AHEAD;
  gpio_set(PTA8, right_wheel.ahead_or_back);
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(right_wheel.freq, right_wheel.ahead_or_back));
}

void bjtu_set_wheel_ahead_all(void) {
  bjtu_set_wheel_ahead_left();
  bjtu_set_wheel_ahead_right();
}

/* ----------------- Set Wheel Speed Function ----------------- */

void bjtu_set_wheel_expect_speed_left(int32 speed) {
  left_wheel.expect_speed = speed;
}

void bjtu_set_wheel_expect_speed_right(int32 speed) {
  right_wheel.expect_speed = speed;
}

void bjtu_set_wheel_expect_speed_all(int32 speed) {
  bjtu_set_wheel_expect_speed_left(speed);
  bjtu_set_wheel_expect_speed_right(speed);
}

/* -----------------  Steering Function ----------------- */

void bjtu_set_steering_turn(int8 direction, int8 turn_percent) {
  if (turn_percent > 80)
    turn_percent = 80;
  else if (turn_percent < 1)
    turn_percent = 1;
  switch (direction) {
  case TURN_LEFT:
    steering_turn = STEERING_MIDDLE + (steering_turn_total * turn_percent)/100;
    break;
  case TURN_RIGHT:
    steering_turn = STEERING_MIDDLE - (steering_turn_total * turn_percent)/100;
    break;
  case TURN_AHEAD:
    steering_turn = STEERING_MIDDLE;
    break;
  case WHEEL_STOP:
    bjtu_set_wheel_freq_all(0);
    break;
  default:
    break;
  }
}
  
void bjtu_turn_steering(void) {
  steering_start_turn(steering_turn);
}

/* ----------------- Refresh States Function ----------------- */

void bjtu_refresh_battle_states(void) {
  bs.var = adc_once(ADC1_SE16, ADC_8bit);
  bs.vol = (3300*bs.var)/((1<<8)-1);
}

void bjtu_refresh_wheel_now_speed(void) {
  left_wheel.now_speed = ftm_quad_get(FTM1);
  right_wheel.now_speed = -ftm_quad_get(FTM2);
  ftm_quad_clean(FTM1);
  ftm_quad_clean(FTM2);
}

void bjtu_refresh_camera(void) {
  camera_get_img();
  img_extract(img, imgbuff, CAMERA_SIZE);
  //++imgCount;
}

/* ----------------- Printf Function ----------------- */

void bjtu_print_battle_states(void) {
  printf("\n=====Battle States=====\n");
  printf("ADC采样结果为:%d，相应电压值为%dmV\n",bs.var, bs.vol);
}

void bjtu_print_speed_states(void) {
  printf("\n=====Speed States=====\n");
  printf("左轮速度是%lld， 右轮速度是%lld\n", left_wheel.now_speed, right_wheel.now_speed);
}

void bjtu_print_image(void) {
  int half = IMG_W/2 - 1;
  printf("\n=====Image States=====\n");
  for (int i = 0; i < IMG_H; ++i) {
    for (int j = 0; j < IMG_W; ++j) {
      if (img[i*IMG_W + j] == 0)
        printf("0");
      else
        printf("1");
      if (j == half)
        printf("-");
    }
    printf("!\n");
  }
}

/* ----------------- OLED Function ----------------- */
const Site_t site = {0, 0};
const Size_t img_size = {CAMERA_W, CAMERA_H};
const Size_t oled_show_size = {CAMERA_W, CAMERA_H};

void bjtu_oled_show_camera(void) {
  OLED_Img_gray_Z(site, oled_show_size, img, img_size);
}

/* ----------------- Debug Function ----------------- */
void bjtu_debug_1(void) {
  dip_process(img);
  dip_get_turn_direction();
  bjtu_print_image();
  dip_print_weight();
}

