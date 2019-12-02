#include "BJTU_func.h"

static uint8 wheel_freq_left;
static uint8 wheel_freq_right;
static uint8 gpio_out_vol;

uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol) {
  if (gpio_out_vol == 0)
    return freq;
  else
    return 100-freq;
}

void bjtu_init_adc() {
  adc_init(ADC1_SE16);
}
void bjtu_init_uart() {
  uart_init(UART0,115200);
}

void bjtu_init_wheel() {
  gpio_out_vol = GPIO_GO_AHEAD;
  wheel_freq_left = wheel_freq_right = 0;
  ftm_pwm_init(FTM0, FTM_CH3,10*1000,0); 
  ftm_pwm_init(FTM0, FTM_CH4,10*1000,0); 
  gpio_init(PTA5, GPO, gpio_out_vol);
  gpio_init(PTA8, GPO, gpio_out_vol);
}

void bjtu_init_encoder() {
  ftm_quad_init(FTM1);
  ftm_quad_init(FTM2);
}

void bjtu_set_wheel_freq_all(uint8 freq) {
  wheel_freq_right = wheel_freq_left = freq;
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(freq, gpio_out_vol));
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(freq, gpio_out_vol));
}

void bjtu_set_wheel_freq_left(uint8 freq) {
  wheel_freq_left = freq;
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(freq, gpio_out_vol));
}
void bjtu_set_wheel_freq_right(uint8 freq) {
  wheel_freq_right = freq;
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(freq, gpio_out_vol));
}

void bjtu_set_wheel_back() {
  gpio_out_vol = GPIO_GO_AHEAD;
  gpio_set(PTA5, gpio_out_vol);
  gpio_set(PTA8, gpio_out_vol);
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(wheel_freq_left, gpio_out_vol));
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(wheel_freq_right, gpio_out_vol));
}

void bjtu_set_wheel_ahead() {
  gpio_out_vol = GPIO_GO_BACK;
  gpio_set(PTA5, gpio_out_vol);
  gpio_set(PTA8, gpio_out_vol);
  ftm_pwm_duty(FTM0, FTM_CH3, fixed_freq(wheel_freq_left, gpio_out_vol));
  ftm_pwm_duty(FTM0, FTM_CH4, fixed_freq(wheel_freq_right, gpio_out_vol));
}

battle_states bjtu_get_battle_states() {
  battle_states tmp;
  tmp.var = adc_once(ADC1_SE16, ADC_8bit);
  tmp.vol = (3300*tmp.var)/((1<<8)-1);
  return tmp;
}

speed_states bjtu_get_speed_states() {
  speed_states tmp;
  tmp.left_wheel = ftm_quad_get(FTM1);
  tmp.right_wheel = -ftm_quad_get(FTM2);
  ftm_quad_clean(FTM1);
  ftm_quad_clean(FTM2);
  return tmp;
}

void bjtu_print_battle_states(battle_states bs) {
  printf("ADC采样结果为:%d，相应电压值为%dmV\n",bs.var, bs.vol);
}

void bjtu_print_speed_states(speed_states ss) {
  printf("左轮速度是%d， 右轮速度是%d\n", ss.left_wheel, ss.right_wheel);
}

