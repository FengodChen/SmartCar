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
 * @File DIP_func.h DIP_func.c
 * @Date Oct 4, 2019
 *
 */

#ifndef DIP_FUNC_H
#define DIP_FUNC_H

#include "common.h"
#include "include.h"
#include "BJTU_func.h"

#define IMG_H CAMERA_H
#define IMG_W CAMERA_W

#define LOWER_THRESHOLD 5
#define NOT_TURN_THRESHOLD 15000

#define HEIGHT_WEIGHT 2
#define WIDTH_WEIGHT  1

typedef struct LR_weight {
  uint64 left;
  uint64 right;
} LR_weight;



/* �ײ㺯�������鲻Ҫ����ʹ�� */
void dip_clear_statistic(void);                         // ͳ����Ϣ����
void dip_clear_statistic_bool(void);                    // ͳ����Ϣboolֵ����
void dip_clear_weight(void);                            // Ȩ����Ϣ����
void dip_clear_weight_width(void);                      // Ȩ�أ���ȷ�����Ϣ����
void dip_clear_weight_height(void);                     // Ȩ�أ��߶ȷ�����Ϣ����

void dip_make_statistic(uint8* img_array);              // ͳ��ͼ����Ϣ
void dip_calculate_width_weight(void);                  // ͳ�ƿ�ȷ����ϵ�Ȩ��
void dip_calculate_height_weight(void);                 // ͳ�Ƹ߶ȷ����ϵ�Ȩ��

/* DIP���溯�� */
void dip_init_main(void);                               // DIP��ʼ��
void dip_process(uint8* img_array);                     // ����ͼ��
uint8 dip_get_turn_direction(void);                     // ͳ�Ʋ�������������ת����Ϣ
void dip_print_weight(void);                           // printf�������Ȩ����Ϣ

#endif