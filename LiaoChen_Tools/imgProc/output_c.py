#! /usr/bin/python3

'''
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
'''

import config
import imgFunc
import numpy as np

# [Config]
classList = config.classList
imgPath = config.imgLogPath
loadPath = config.weightLoadPath
savePath = config.weightSavePath

headPath = config.cHeadPath
srcPath = config.cSrcPath
headName = config.cHeadName
licenseStr = config.licenseStr
dataType = config.dataType

# [Main]
ga_main = imgFunc.GA_Main(10, 10, imgPath, classList)
ga_main.load(loadPath)
DNA = None
(DNA1, DNA2, loss1, loss2) = ga_main.ga.getBestDNAs_threads()
if (loss1 < loss2):
    DNA = DNA1
else:
    DNA = DNA2

(array1_num, array1_h, array1_w) = np.shape(DNA.array1)
(array2_h, array2_w) = np.shape(DNA.array2)

cFile = open(srcPath, 'w')
hFile = open(headPath, 'w')

# [.h]
hFile.write("/*\n{}\n*/\n\n".format(licenseStr))
hFile.write("#ifndef {}\n".format(headName))
hFile.write("#define {}\n".format(headName))
hFile.write('\n')
hFile.write('#include "include.h"\n')
hFile.write('#include "BJTU_func.h"\n')
hFile.write('\n')
hFile.write('#define ARRAY1_NUM ({})\n'.format(array1_num))
hFile.write('#define ARRAY1_H ({})\n'.format(array1_h))
hFile.write('#define ARRAY1_W ({})\n'.format(array1_w))
hFile.write('\n')
hFile.write('#define ARRAY2_H ({})\n'.format(array2_h))
hFile.write('#define ARRAY2_W ({})\n'.format(array2_w))
hFile.write('\n')
hFile.write('#define ARRAY1 \\\n')
hFile.write("{")
for i in range(array1_num):
    hFile.write("{")
    for j in range(array1_h):
        hFile.write("{")
        for k in range(array1_w):
            inner = DNA.array1[i, j, k]
            if (inner < 0):
                hFile.write("{}".format(inner))
            else:
                hFile.write("+{}".format(inner))
            if (k < array1_w-1):
                hFile.write(", ")
        if (j < array1_h-1):
            hFile.write("},\\\n")
        else:
            hFile.write("}\\\n")
    if (i < array1_num-1):
        hFile.write("},\\\n")
    else:
        hFile.write("}\\\n")
hFile.write("}\n")
hFile.write("\n")
hFile.write('#define ARRAY2 \\\n')
hFile.write("{")
for i in range(array2_h):
    hFile.write("{")
    for j in range(array2_w):
        inner = DNA.array2[i, j]
        if (inner < 0):
            hFile.write("{}".format(inner))
        else:
            hFile.write("+{}".format(inner))
        if (j < array2_w-1):
            hFile.write(", ")
    if (i < array2_h-1):
        hFile.write("},\\\n")
    else:
        hFile.write("}\\\n")
hFile.write("}\n")
hFile.write("\n")
hFile.write('double weight_mul_sum(const uint8 *img, const float array1_n[][ARRAY1_W], const uint8 H, const uint8 W);\n')
hFile.write('void weight_get_mesneArray(const uint8 *img);\n')
hFile.write('void weight_get_finalArray();\n')
hFile.write('uint8 weight_get_ans(const uint8 *img);\n')
hFile.write("\n")
hFile.write("#endif\n")

# [.c]
cFile.write("/*\n{}\n*/\n\n".format(licenseStr))
cFile.write('\n')
cFile.write('\n')
cFile.write('#include "weight_func.h"\n')
cFile.write('\n')
cFile.write('const float array1[ARRAY1_NUM][ARRAY1_H][ARRAY1_W] = ARRAY1;\n')
cFile.write('const float array2[ARRAY2_H][ARRAY2_W] = ARRAY2;\n')
cFile.write('\n')
cFile.write('#define CLASS_NUM ARRAY2_W\n')
cFile.write('\n')
cFile.write('double mesneArray[ARRAY1_NUM];\n')
cFile.write('double ansArray[CLASS_NUM];\n')
cFile.write('\n')
cFile.write('double weight_mul_sum(const uint8 *img, const float array1_n[][ARRAY1_W], const uint8 H, const uint8 W) {\n')
cFile.write('    double sum = 0;\n')
cFile.write('    uint32 ptr = 0;\n')
cFile.write('    for (int i = 0; i < H; ++i)\n')
cFile.write('        for (int j = 0; j < W; ++j)\n')
cFile.write('            sum += img[ptr++] * array1_n[i][j];\n')
cFile.write('    return sum;\n')
cFile.write('}\n')
cFile.write('\n')
cFile.write('void weight_get_mesneArray(const uint8 *img) {\n')
cFile.write('    for (int i = 0; i < ARRAY1_NUM; ++i)\n')
cFile.write('        mesneArray[i] = weight_mul_sum(img, array1[i], IMG_H, IMG_W);\n')
cFile.write('}\n')
cFile.write('\n')
cFile.write('void weight_get_finalArray() {\n')
cFile.write('    double sum;\n')
cFile.write('    for (int i = 0; i < ARRAY2_W; ++i) {\n')
cFile.write('        sum = 0;\n')
cFile.write('        for (int j = 0; j < ARRAY2_H; ++j) {\n')
cFile.write('            sum += mesneArray[j] * array2[j][i];\n')
cFile.write('        }\n')
cFile.write('        ansArray[i] = sum;\n')
cFile.write('    }\n')
cFile.write('}\n')
cFile.write('\n')
cFile.write('uint8 weight_get_ans(const uint8 *img) {\n')
cFile.write('    weight_get_mesneArray(img);\n')
cFile.write('    weight_get_finalArray();\n')
cFile.write('    double max = -5000;\n')
cFile.write('    uint8 max_ptr = 0;\n')
cFile.write('    for (int i = 0; i < CLASS_NUM; ++i) {\n')
cFile.write('        if (ansArray[i] > max) {\n')
cFile.write('            max = ansArray[i];\n')
cFile.write('            max_ptr = i;\n')
cFile.write('        }\n')
cFile.write('    }\n')
cFile.write('    return max_ptr;\n')
cFile.write('}\n')
cFile.write('\n')

cFile.close()
hFile.close()