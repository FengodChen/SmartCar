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



/* 底层函数，建议不要轻易使用 */
void dip_clear_statistic(void);                         // 统计信息归零
void dip_clear_statistic_bool(void);                    // 统计信息bool值归零
void dip_clear_weight(void);                            // 权重信息归零
void dip_clear_weight_width(void);                      // 权重（宽度方向）信息归零
void dip_clear_weight_height(void);                     // 权重（高度方向）信息归零

void dip_make_statistic(uint8* img_array);              // 统计图像信息
void dip_calculate_width_weight(void);                  // 统计宽度方向上的权重
void dip_calculate_height_weight(void);                 // 统计高度方向上的权重

/* DIP表面函数 */
void dip_init_main(void);                               // DIP初始化
void dip_process(uint8* img_array);                     // 处理图像
uint8 dip_get_turn_direction(void);                     // 统计并返回向左向右转的信息
void dip_print_weight(void);                           // printf输出所有权重信息

#endif