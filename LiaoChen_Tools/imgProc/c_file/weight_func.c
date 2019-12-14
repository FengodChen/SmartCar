/*
Copyright(C) 2019 Liao Chen
@School Beijing Jiaotong University
@ID 17211401

MIT License
Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the
Software without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit
persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "weight_func.h"

float array1[ARRAY1_NUM][ARRAY1_H][ARRAY1_W] = ARRAY1;
float array2[ARRAY2_H][ARRAY2_W] = ARRAY2;

#define uint8 unsigned short
#define int64 long long int
#define IMG_H ARRAY1_H
#define IMG_W ARRAY1_W
#define CLASS_NUM ARRAY2_W

uint8 img2D[IMG_H][IMG_W];
double mesneArray[ARRAY1_NUM];
double ansArray[CLASS_NUM];

void weight_init(uint8* img) {
    int ptr = 0;
    for (int i = 0; i < IMG_H; ++i)
        for (int j = 0; i < IMG_W; ++j)
            img2D[i][j] = img[ptr++];
}

int64 weight_mul_sum(uint8 img2D[][IMG_W], float array1_n[][ARRAY1_W], uint8 H, uint8 W) {
    double sum = 0;
    for (int i = 0; i < H; ++i)
        for (int j = 0; j < W; ++j)
            sum += img2D[i][j] * array1_n[i][j];
    return sum;
}

void weight_get_mesneArray() {
    for (int i = 0; i < ARRAY1_NUM; ++i)
        mesneArray[i] = weight_mul_sum(img2D, array1[i], IMG_H, IMG_W);
}

void weight_get_finalArray() {
    double sum;
    for (int i = 0; i < ARRAY2_W; ++i) {
        sum = 0;
        for (int j = 0; j < ARRAY2_H; ++j) {
            sum += mesneArray[j] * array2[j][i];
        }
        ansArray[i] = sum;
    }
}

uint8 weight_get_ans() {
    weight_get_mesneArray();
    weight_get_finalArray();
    double max = -5000;
    uint8 max_ptr = 0;
    for (int i = 0; i < CLASS_NUM; ++i) {
        if (ansArray[i] > max) {
            max = ansArray[i];
            max_ptr = i;
        }
    }
    return max_ptr;
}

