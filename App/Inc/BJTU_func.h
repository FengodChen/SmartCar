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

#define WHEEL_GO_AHEAD 0
#define WHEEL_GO_BACK 1

// 电池信息结构体
typedef struct battle_states {
  uint8 var;
  uint8 vol;
} battle_states;

// 后车车轮状态结构体
typedef struct wheel_states {
  uint8 ahead_or_back;          // 向前转或向后转
  uint8 freq;                   // 电机供电占空比
  int16 expect_speed;           // 期望的速度
  int16 now_speed;              // 目前的速度
} wheel_states;

/* 底层函数，建议不要使用 */
uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol);            // 得到修正过的占空比
void steering_end_turn(void);                               // 中断函数，将舵机电平重新归零
void steering_start_turn(uint16 time_us);                   // 将舵机电平置一并开始中断

/* 用于开始时的初始化 */
void bjtu_init_adc(void);                                   // 初始化ADC
void bjtu_init_uart(void);                                  // 初始化UART
void bjtu_init_wheel(void);                                 // 初始化后轮轮胎（电机频率等）
void bjtu_init_encoder(void);                               // 初始化编码器
void bjtu_init_oled(void);                                  // 初始化OLED
void bjtu_init_steering(void);                              // 初始化舵机
void bjtu_init_main(void);                                  // 初始化全部所需要的硬件

/* 设置后车车轮的占空比以及正反转 */
void bjtu_set_wheel_freq_all(uint8 freq);                       // 设置后轮两个轮胎转动占空比
void bjtu_set_wheel_freq_left(uint8 freq);                      // 设置后轮左轮转动占空比
void bjtu_set_wheel_freq_right(uint8 freq);                     // 设置后轮右轮转动占空比

void bjtu_set_wheel_back_all(void);                             // 设置后轮两个轮胎向后转
void bjtu_set_wheel_back_left(void);                            // 设置后轮左轮向后转
void bjtu_set_wheel_back_right(void);                           // 设置后轮右轮向后转

void bjtu_set_wheel_ahead_all(void);                            // 设置后轮两个轮胎向前转
void bjtu_set_wheel_ahead_left(void);                           // 设置后轮左轮向前转
void bjtu_set_wheel_ahead_right(void);                          // 设置后轮右轮向前转

void bjtu_set_wheel_expect_speed_all(int8 speed);               // 设置后轮两个轮胎期望速度
void bjtu_set_wheel_expect_speed_left(int8 speed);              // 设置后轮左轮期望速度
void bjtu_set_wheel_expect_speed_right(int8 speed);             // 设置后轮右轮期望速度

/* 刷新各种信息 */ 
void bjtu_refresh_battle_states(void);                         // 得到电池信息并返回
void bjtu_refresh_wheel_now_speed(void);                       // 得到后轮速度信息并返回

/* 使用printf输出各种信息 */
void bjtu_print_battle_states(void);                            // 通过printf输出电池信息
void bjtu_print_speed_states(void);                             // 通过printf输出后轮速度

#endif