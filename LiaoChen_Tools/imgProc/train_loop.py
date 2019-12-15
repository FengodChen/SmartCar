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

import imgFunc
import config

# [Config]
classList = config.classList
imgPath = config.imgLogPath
loadPath = config.weightLoadPath
savePath = config.weightSavePath
branchNum = config.array1_branchNum
DNANum = config.dnaNum
mutationRate = config.mutationRate

# [Main Function]
ga_main = imgFunc.GA_Main(branchNum, DNANum, imgPath, classList)
ga_main.load(loadPath)
minLoss = min(ga_main.getLoss())
#minLoss = 1000

while (True):
    ga_main.train(50, mutationRate)
    if (minLoss > min(ga_main.getLoss())):
        ga_main.save(savePath)
        loadPath = savePath
        minLoss = min(ga_main.getLoss())
        print("Great! Saved Net!")
    else:
        ga_main.load(loadPath)