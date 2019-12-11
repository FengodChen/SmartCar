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
#include "math.h"

#define IMG_H CAMERA_H          //60
#define IMG_W CAMERA_W          //80

#define LOWER_THRESHOLD 5
#define NOT_TURN_THRESHOLD 10000
#define NOT_TURN_THRESHOLD_MIN 9999

#define HEIGHT_WEIGHT 1
#define WIDTH_WEIGHT  1

typedef struct LR_weight {
  uint64 left;
  uint64 right;
} LR_weight;

typedef struct turn_order {
  uint8 direction;
  uint8 turn_percent;
} turn_order;

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
//int32 dip_get_turn_direction(void);                     // 统计并返回向左向右转的信息
turn_order dip_get_turn_direction(void);

void dip_print_weight(void);                           // printf输出所有权重信息

#endif