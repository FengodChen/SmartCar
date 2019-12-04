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

#define STEERING_PIT(X) PIT0##X

static wheel_states left_wheel;
static wheel_states right_wheel;

static battle_states bs;

static uint8 imgbuff[CAMERA_SIZE];
static uint8 img[CAMERA_H*CAMERA_W];
int imgCount = 0;

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
  set_vector_handler(STEERING_PIT(_VECTORn), steering_end_turn);
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

/* ----------------- Initialize Function ----------------- */

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
  ftm_pwm_init(FTM0, FTM_CH3,10*1000,0); 
  ftm_pwm_init(FTM0, FTM_CH4,10*1000,0); 
  gpio_init(PTA5, GPO, left_wheel.ahead_or_back);
  gpio_init(PTA8, GPO, right_wheel.ahead_or_back);
}

void bjtu_init_encoder(void) {
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
}

void bjtu_init_oled(void) {
  OLED_Init();
}

void bjtu_init_steering(void) {
  gpio_init(PTE5,GPO,1);
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
  set_vector_handler(PORTA_VECTORn , PORTA_IRQHandler);   //设置 PORTA 的中断服务函数为 PORTA_IRQHandler
  set_vector_handler(DMA0_VECTORn , DMA0_IRQHandler);     //设置 DMA0 的中断服务函数为 PORTA_IRQHandler
}

void bjtu_init_main(void) {
  bjtu_init_adc();
  bjtu_init_uart();
  bjtu_init_wheel();
  bjtu_init_encoder();
  bjtu_init_oled();
  bjtu_init_steering();
  bjtu_init_led();
  bjtu_init_camera();
  dip_init_main();
}

/* ----------------- Set Wheel Speed Function ----------------- */

void bjtu_set_wheel_freq_left(uint8 freq) {
  if (freq > 100)
    freq = 100;
  left_wheel.freq = freq;
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(freq, left_wheel.ahead_or_back));
}
void bjtu_set_wheel_freq_right(uint8 freq) {
  if (freq > 100)
    freq = 100;
  right_wheel.freq = freq;
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(freq, right_wheel.ahead_or_back));
}

void bjtu_set_wheel_freq_all(uint8 freq) {
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

void bjtu_set_wheel_expect_speed_left(int8 speed) {
  left_wheel.expect_speed = speed;
}

void bjtu_set_wheel_expect_speed_right(int8 speed) {
  right_wheel.expect_speed = speed;
}

void bjtu_set_wheel_expect_speed_all(int8 speed) {
  bjtu_set_wheel_expect_speed_left(speed);
  bjtu_set_wheel_expect_speed_right(speed);
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
  ++imgCount;
}

/* ----------------- Printf Function ----------------- */

void bjtu_print_battle_states(void) {
  printf("ADC采样结果为:%d，相应电压值为%dmV\n",bs.var, bs.vol);
}

void bjtu_print_speed_states(void) {
  printf("左轮速度是%d， 右轮速度是%d\n", left_wheel.now_speed, right_wheel.now_speed);
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
  dip_print_weight();
}
