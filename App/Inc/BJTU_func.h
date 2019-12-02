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

// �����Ϣ�ṹ��
typedef struct battle_states {
  uint8 var;
  uint8 vol;
} battle_states;

// �ٶ���Ϣ�ṹ��
typedef struct speed_states {
  int16 left_wheel;
  int16 right_wheel;
} speed_states;

// �󳵳���״̬�ṹ��
typedef struct wheel_states {
  uint8 ahead_or_back;
  uint8 freq;
} wheel_states;

/* �ײ㺯�������鲻Ҫʹ�� */
uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol);            // �õ���������ռ�ձ�

/* ���ڿ�ʼʱ�ĳ�ʼ�� */
void bjtu_init_adc(void);                                   // ��ʼ��ADC
void bjtu_init_uart(void);                                  // ��ʼ��UART
void bjtu_init_wheel(void);                                 // ��ʼ��������̥�����Ƶ�ʵȣ�
void bjtu_init_encoder(void);                               // ��ʼ��������
void bjtu_init_main(void);                                  // ��ʼ��ȫ������Ҫ��Ӳ��

/* ���ú󳵳��ֵ�ռ�ձ��Լ�����ת */
void bjtu_set_wheel_freq_all(uint8 freq);                       // ���ú���������̥ת��ռ�ձ�
void bjtu_set_wheel_freq_left(uint8 freq);                      // ���ú�������ת��ռ�ձ�
void bjtu_set_wheel_freq_right(uint8 freq);                     // ���ú�������ת��ռ�ձ�
void bjtu_set_wheel_back_all(void);                             // ���ú���������̥���ת
void bjtu_set_wheel_back_left(void);                            // ���ú����������ת
void bjtu_set_wheel_back_right(void);                           // ���ú����������ת
void bjtu_set_wheel_ahead_all(void);                            // ���ú���������̥��ǰת
void bjtu_set_wheel_ahead_left(void);                           // ���ú���������ǰת
void bjtu_set_wheel_ahead_right(void);                          // ���ú���������ǰת

/* ��ȡ������Ϣ */
battle_states bjtu_get_battle_states(void);                 // �õ������Ϣ������
speed_states bjtu_get_speed_states(void);                   // �õ������ٶ���Ϣ������

/* ʹ��printf���������Ϣ */
void bjtu_print_battle_states(battle_states);           // ͨ��printf��������Ϣ
void bjtu_print_speed_states(speed_states);             // ͨ��printf��������ٶ�

#endif