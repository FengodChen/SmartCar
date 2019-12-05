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
#include "DIP_func.h"

#define WHEEL_GO_AHEAD  (0)
#define WHEEL_GO_BACK   (1)

#define STEERING_MIDDLE         (1000)
#define STEERING_LEFT_MAX       (1200)
#define STEERING_RIGHT_MAX      (800)

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

// ���ת��
enum {
  TURN_LEFT,
  TURN_RIGHT
};

/* �ײ㺯�������鲻Ҫ����ʹ�� */
uint8 fixed_freq(uint8 freq, uint8 gpio_out_vol);                // �õ���������ռ�ձ�
void steering_end_turn(void);                                   // �жϺ������������ƽ���¹���
void steering_start_turn(uint16 time_us);                       // �������ƽ��һ����ʼ�ж�
void PORTA_IRQHandler(void);
void DMA0_IRQHandler(void);

/* ���ڿ�ʼʱ�ĳ�ʼ�� */
void bjtu_init_led(void);                                       // ��ʼ���ĸ�LED
void bjtu_init_adc(void);                                       // ��ʼ��ADC
void bjtu_init_uart(void);                                      // ��ʼ��UART
void bjtu_init_wheel(void);                                     // ��ʼ��������̥�����Ƶ�ʵȣ�
void bjtu_init_encoder(void);                                   // ��ʼ��������
void bjtu_init_oled(void);                                      // ��ʼ��OLED
void bjtu_init_steering(void);                                  // ��ʼ�����
void bjtu_init_camera(void);                                    // ��ʼ������ͷ
void bjtu_init_main(void);                                      // ��ʼ��ȫ������Ҫ��Ӳ��

/* ���ƺ󳵳��ֵ�ռ�ձ��Լ�����ת */
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

/* ���ƶ������ */
void bjtu_set_steering_turn(uint8 direction, uint8 turn_percent);       // ���ö������ת��λ�� @args �����ת��ƫתλ�ðٷֱȣ�
void bjtu_turn_steering(void);                                          // �������úõĶ������ת��λ�ý���ת��

/* ˢ�¸�����Ϣ */ 
void bjtu_refresh_battle_states(void);                          // �õ������Ϣ������
void bjtu_refresh_wheel_now_speed(void);                        // �õ������ٶ���Ϣ������
void bjtu_refresh_camera(void);                                 // �õ������ȡ��ͼ��

/* ʹ��printf���������Ϣ */
void bjtu_print_battle_states(void);                            // ͨ��printf��������Ϣ
void bjtu_print_speed_states(void);                             // ͨ��printf��������ٶ�
void bjtu_print_image(void);                                    // ͨ��printf���ͼ��

/* OLED��ʾ */
void bjtu_oled_show_camera(void);                               // OLED��ʾ�Ѿ��������ȡ�����е�ͼ��

/* ���ڸ���Debug */
void bjtu_debug_1(void);

#endif