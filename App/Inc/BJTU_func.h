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

// ��̥�������ȫ�ֱ���
//static uint8 gpio_out_vol;
//  static uint8 wheel_freq;

// �����Ϣ�ṹ��
typedef struct battle_states {
  uint8 var;
  uint8 vol;
} battle_states;

typedef struct speed_states {
  int16 left_wheel;
  int16 right_wheel;
} speed_states;

uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol);       // �õ���������ռ�ձ�

void bjtu_init_adc();                                   // ��ʼ��ADC
void bjtu_init_uart();                                  // ��ʼ��UART
void bjtu_init_wheel();                                 // ��ʼ��������̥�����Ƶ�ʵȣ�
void bjtu_init_encoder();                               // ��ʼ��������

void bjtu_set_wheel_freq_all(uint8 freq);               // ���ú���������̥ת��ռ�ձ�
void bjtu_set_wheel_freq_left(uint8 freq);              // ���ú�������ת��ռ�ձ�
void bjtu_set_wheel_freq_right(uint8 freq);             // ���ú�������ת��ռ�ձ�
void bjtu_set_wheel_back();                             // ���ú�����̥���ת
void bjtu_set_wheel_ahead();                            // ���ú�����̥��ǰת

battle_states bjtu_get_battle_states();                 // �õ������Ϣ������
speed_states bjtu_get_speed_states();                   // �õ������ٶ���Ϣ������

void bjtu_print_battle_states(battle_states);           // ͨ��printf��������Ϣ
void bjtu_print_speed_states(speed_states);             // ͨ��printf��������ٶ�

#endif