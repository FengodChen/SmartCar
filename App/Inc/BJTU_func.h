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

#ifndef BJTU_FUNC_H_
#define BJTU_FUNC_H_

#include "common.h"
#include "include.h"
#include "stdlib.h"
#include "DIP_func.h"
#include "weight_func.h"

#define WHEEL_GO_AHEAD  (0)
#define WHEEL_GO_BACK   (1)

#define STEERING_MIDDLE         (1000)
#define STEERING_LEFT_MAX       (1200)
#define STEERING_RIGHT_MAX      (800)

#define KEY_S3 KEY_START
#define KEY_S2 KEY_B

#define SPEED_LOWER_THRESHOLD (25)
#define SPEED_TOLERANCE (10)

// 电池信息结构体
typedef struct battle_states {
  uint8 var;
  uint8 vol;
} battle_states;

// 后车车轮状态结构体
typedef struct wheel_states {
  uint8 ahead_or_back;          // 向前转或向后转
  int8 freq;                   // 电机供电占空比
  int64 expect_speed;           // 期望的速度
  int64 now_speed;              // 目前的速度
} wheel_states;

// 函数链表
typedef struct func_list {
  struct func_list *next;
  void (*func)();
  uint32 need_time_ms;
} func_list;

// 舵机转向
enum {
  TURN_LEFT,
  TURN_RIGHT,
  TURN_AHEAD,
  WHEEL_STOP
};

/* 底层函数，建议不要轻易使用 */

uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol);                // 得到修正过的占空比
void steering_end_turn(void);                                   // 中断函数，将舵机电平重新归零
void steering_start_turn(uint16 time_us);                       // 将舵机电平置一并开始中断
void PORTA_IRQHandler(void);
void DMA0_IRQHandler(void);
void malloc_func_list(func_list **now);                        // 为函数列表申请空间
void timer(void);                                              // 计时器


/* 主要函数 */
void bjtu_init_main(void);                                              // 初始化全部所需要的硬件

void bjtu_main_camera();                                     // 相机主函数
void bjtu_main_dip();                                        // 图像处理主函数
void bjtu_main_steering();                                   // 舵机转动主函数
void bjtu_main_speed();                                      // 控制速度函数
void bjtu_main();                                            // 中断用主函数

/* 用于开始时的初始化 */
void bjtu_init_key(void);                                       // 初始化按键
void bjtu_init_led(void);                                       // 初始化四个LED
void bjtu_init_adc(void);                                       // 初始化ADC
void bjtu_init_uart(void);                                      // 初始化UART
void bjtu_init_wheel(void);                                     // 初始化后轮轮胎（电机频率等）
void bjtu_init_encoder(void);                                   // 初始化编码器
void bjtu_init_oled(void);                                      // 初始化OLED
void bjtu_init_steering(void);                                  // 初始化舵机
void bjtu_init_camera(void);                                    // 初始化摄像头
void bjtu_init_func_list(void);                                 // 初始化函数列表
void bjtu_init_timer(void);                                     // 初始化计时器

/* 反馈按键 */
void bjtu_key_func(void);                                       // 各种按键的功能函数

/* 控制后车车轮的占空比以及正反转 */
void bjtu_set_wheel_freq_all(int8 freq);                       // 设置后轮两个轮胎转动占空比
void bjtu_set_wheel_freq_left(int8 freq);                      // 设置后轮左轮转动占空比
void bjtu_set_wheel_freq_right(int8 freq);                     // 设置后轮右轮转动占空比

void bjtu_set_wheel_back_all(void);                             // 设置后轮两个轮胎向后转
void bjtu_set_wheel_back_left(void);                            // 设置后轮左轮向后转
void bjtu_set_wheel_back_right(void);                           // 设置后轮右轮向后转

void bjtu_set_wheel_ahead_all(void);                            // 设置后轮两个轮胎向前转
void bjtu_set_wheel_ahead_left(void);                           // 设置后轮左轮向前转
void bjtu_set_wheel_ahead_right(void);                          // 设置后轮右轮向前转

void bjtu_set_wheel_expect_speed_all(int32 speed);               // 设置后轮两个轮胎期望速度
void bjtu_set_wheel_expect_speed_left(int32 speed);              // 设置后轮左轮期望速度
void bjtu_set_wheel_expect_speed_right(int32 speed);             // 设置后轮右轮期望速度

/* 控制舵机方向 */
void bjtu_set_steering_turn(int8 direction, int8 turn_percent);          // 设置舵机绝对转向位置 @args （舵机转向，偏转位置百分比）
void bjtu_turn_steering(void);                                          // 根据设置好的舵机绝对转向位置进行转向

/* 刷新各种信息 */ 
void bjtu_refresh_battle_states(void);                          // 得到电池信息并返回
void bjtu_refresh_wheel_now_speed(void);                        // 得到后轮速度信息并返回
void bjtu_refresh_camera(void);                                 // 得到相机获取的图像

/* 使用printf输出各种信息 */
void bjtu_print_battle_states(void);                            // 通过printf输出电池信息
void bjtu_print_speed_states(void);                             // 通过printf输出后轮速度
void bjtu_print_image(void);                                    // 通过printf输出图像

/* OLED显示 */
void bjtu_oled_show_camera(void);                               // OLED显示已经从相机获取的数列的图像

/* 相应Key键 */
void bjtu_key_func(void);                                       // 相应Key键

/* 用于各种Debug */
void bjtu_debug_1(void);

#endif