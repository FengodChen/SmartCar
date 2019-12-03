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

// �󳵳���״̬�ṹ��
typedef struct wheel_states {
  uint8 ahead_or_back;          // ��ǰת�����ת
  uint8 freq;                   // �������ռ�ձ�
  int16 expect_speed;           // �������ٶ�
  int16 now_speed;              // Ŀǰ���ٶ�
} wheel_states;

/* �ײ㺯�������鲻Ҫʹ�� */
uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol);            // �õ���������ռ�ձ�
void steering_end_turn(void);                               // �жϺ������������ƽ���¹���
void steering_start_turn(uint16 time_us);                   // �������ƽ��һ����ʼ�ж�

/* ���ڿ�ʼʱ�ĳ�ʼ�� */
void bjtu_init_adc(void);                                   // ��ʼ��ADC
void bjtu_init_uart(void);                                  // ��ʼ��UART
void bjtu_init_wheel(void);                                 // ��ʼ��������̥�����Ƶ�ʵȣ�
void bjtu_init_encoder(void);                               // ��ʼ��������
void bjtu_init_oled(void);                                  // ��ʼ��OLED
void bjtu_init_steering(void);                              // ��ʼ�����
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

void bjtu_set_wheel_expect_speed_all(int8 speed);               // ���ú���������̥�����ٶ�
void bjtu_set_wheel_expect_speed_left(int8 speed);              // ���ú������������ٶ�
void bjtu_set_wheel_expect_speed_right(int8 speed);             // ���ú������������ٶ�

/* ˢ�¸�����Ϣ */ 
void bjtu_refresh_battle_states(void);                         // �õ������Ϣ������
void bjtu_refresh_wheel_now_speed(void);                       // �õ������ٶ���Ϣ������

/* ʹ��printf���������Ϣ */
void bjtu_print_battle_states(void);                            // ͨ��printf��������Ϣ
void bjtu_print_speed_states(void);                             // ͨ��printf��������ٶ�

#endif