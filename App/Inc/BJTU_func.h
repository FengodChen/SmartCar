/* Copyright Notice:
 * 
 * Copyright (c) 2019, Liao Chen
 * @School Beijing Jiaotong University
 * @ID 17211401
 * 
 * This header file used to BJTU Smart Car.
 * It based on MK60 program.
 * 
 * @File BJTU_fun.h BJTU_func.c
 * @Date Oct 1, 2019
 *
 */

#ifndef BJTU_FUNC_H_
#define BJTU_FUNC_H_

#include "common.h"
#include "include.h"

#define GPIO_GO_AHEAD 0
#define GPIO_GO_BACK 1

// 轮胎电机数据全局变量
//static uint8 gpio_out_vol;
//  static uint8 wheel_freq;

// 电池信息结构体
typedef struct battle_states {
  uint8 var;
  uint8 vol;
} battle_states;

typedef struct speed_states {
  int16 left_wheel;
  int16 right_wheel;
} speed_states;

uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol);       // 得到修正过的占空比

void bjtu_init_adc();                                   // 初始化ADC
void bjtu_init_uart();                                  // 初始化UART
void bjtu_init_wheel();                                 // 初始化后轮轮胎（电机频率等）
void bjtu_init_encoder();                               // 初始化编码器

void bjtu_set_wheel_freq_all(uint8 freq);               // 设置后轮两个轮胎转动占空比
void bjtu_set_wheel_freq_left(uint8 freq);              // 设置后轮左轮转动占空比
void bjtu_set_wheel_freq_right(uint8 freq);             // 设置后轮右轮转动占空比
void bjtu_set_wheel_back();                             // 设置后轮轮胎向后转
void bjtu_set_wheel_ahead();                            // 设置后轮轮胎向前转

battle_states bjtu_get_battle_states();                 // 得到电池信息并返回
speed_states bjtu_get_speed_states();                   // 得到后轮速度信息并返回

void bjtu_print_battle_states(battle_states);           // 通过printf输出电池信息
void bjtu_print_speed_states(speed_states);             // 通过printf输出后轮速度

#endif