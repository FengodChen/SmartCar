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

#include "DIP_func.h"

const unsigned int img_width_middle = IMG_W / 2;
//const unsigned int img_height_middle = IMG_H / 2;

unsigned int img_w_statistic[IMG_W];             // 图像宽度方向上的统计
unsigned int img_h_statistic_L[IMG_H];           // 图像高度方向（左边）上的统计
unsigned int img_h_statistic_R[IMG_H];           // 图像高度方向（右边）上的统计

uint8 img_w_statistic_bool[IMG_W];        // 图像宽度方向上超过阈值的统计
uint8 img_h_statistic_bool_L[IMG_H];      // 图像高度方向（左边）上的超过阈值统计
uint8 img_h_statistic_bool_R[IMG_H];      // 图像高度方向（右边）上的超过阈值统计

static int dip_i, dip_j;
static int dip_h, dip_w;
static uint64 dip_long_ptr;

LR_weight width_weight;
LR_weight height_weight;

static uint64 total_left_weight;
static uint64 total_right_weight;

void dip_init_main(void) {
  dip_clear_statistic();
  dip_clear_statistic_bool();
  dip_clear_weight();
}

void dip_clear_statistic(void) {
  for (dip_i = 0; dip_i < IMG_W; ++dip_i)
    img_w_statistic[dip_i] = 0;
  for (dip_i = 0; dip_i < IMG_H; ++dip_i)
    img_h_statistic_L[dip_i] = img_h_statistic_R[dip_i] = 0;
}

void dip_clear_statistic_bool(void) {
  for (dip_i = 0; dip_i < IMG_W; ++dip_i)
    img_w_statistic_bool[dip_i] = 0;
  for (dip_i = 0; dip_i < IMG_H; ++dip_i)
    img_h_statistic_bool_L[dip_i] = img_h_statistic_bool_R[dip_i] = 0;
}

void dip_clear_weight_width(void) {
  width_weight.left = width_weight.right = 0;
}

void dip_clear_weight_height(void) {
  height_weight.left = height_weight.right = 0;
}

void dip_clear_weight(void) {
  width_weight.left = width_weight.right = 0;
  height_weight.left = height_weight.right = 0;
}

void dip_make_statistic(uint8* img_array) {
  dip_long_ptr = 0;
  dip_clear_statistic();
  for (dip_h = 0; dip_h < IMG_H; ++dip_h) {
    for (dip_w = 0; dip_w < IMG_W; ++dip_w) {
      if (img_array[dip_long_ptr++] == 0) {
        img_w_statistic[dip_w] += 1;
        if (dip_w < img_width_middle)
          img_h_statistic_L[dip_h] += 1;
        else
          img_h_statistic_R[dip_h] += 1;
      }
    }
  }
  for (dip_i = 0; dip_i < IMG_W; ++dip_i) {
    if (img_w_statistic[dip_i] >= LOWER_THRESHOLD)
      img_w_statistic_bool[dip_i] = 1;
    else
      img_w_statistic_bool[dip_i] = 0;
  }
  for (dip_i = 0; dip_i < IMG_H; ++dip_i) {
    if (img_h_statistic_L[dip_i] >= LOWER_THRESHOLD)
      img_h_statistic_bool_L[dip_i] = 1;
    else
      img_h_statistic_bool_L[dip_i] = 0;
    if (img_h_statistic_R[dip_i] >= LOWER_THRESHOLD)
      img_h_statistic_bool_R[dip_i] = 1;
    else
      img_h_statistic_bool_R[dip_i] = 0;
  }
}

void dip_calculate_width_weight(void) {
  dip_clear_weight_width();
  dip_j = 1;
  for (dip_i = 0; dip_i < img_width_middle; ++dip_i, ++dip_j)
    width_weight.left += dip_j * img_w_statistic[dip_i];
  dip_j = img_width_middle;
  for (dip_i = img_width_middle; dip_i < IMG_W; ++dip_i, --dip_j)
    width_weight.right += dip_j * img_w_statistic[dip_i];
}

void dip_calculate_height_weight(void) {
  dip_clear_weight_height();
  for (dip_i = 0, dip_j = 0; dip_i < IMG_H; ++dip_i, ++dip_j) {
    height_weight.left += dip_j * img_h_statistic_L[dip_i];
    height_weight.right += dip_j * img_h_statistic_R[dip_i];
  }
}

void dip_process(uint8* img_array) {
  dip_make_statistic(img_array);
  dip_calculate_width_weight();
  dip_calculate_height_weight();
}

uint8 dip_get_turn_direction(void) {  
  total_left_weight = WIDTH_WEIGHT*width_weight.left + HEIGHT_WEIGHT*height_weight.left;
  total_right_weight = WIDTH_WEIGHT*width_weight.right + HEIGHT_WEIGHT*height_weight.right;
  if (total_left_weight > total_right_weight && total_left_weight - total_right_weight > NOT_TURN_THRESHOLD)
    return TURN_RIGHT;
  else if (total_left_weight < total_right_weight && total_right_weight - total_left_weight > NOT_TURN_THRESHOLD)
    return TURN_LEFT;
  else
    return TURN_AHEAD;
}

void dip_print_weight(void) {
  printf("\n=====Weight States=====\n");
  printf("width_weight.left = %lld, ", width_weight.left);
  printf("width_weight.right = %lld\n", width_weight.right);
  printf("height_weight.left = %lld, ", height_weight.left);
  printf("height_weight.right = %lld\n", height_weight.right);
  printf("左边危险权重是%lld，", total_left_weight);
  printf("右边危险权重是%lld\n", total_right_weight);
}
