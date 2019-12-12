from copy import copy
import numpy as np
import matplotlib.pyplot as plt

class ImgData:
    def __init__(self):
        self.name = None
        self.data = []

    def setName(self, name):
        self.name = name

    def addStrData(self, strData):
        intDataLen = len(strData) - 1
        intData = [0] * intDataLen
        for ptr in range(intDataLen):
            intData[ptr] = int(strData[ptr])
            if (intData[ptr] == 1):
                intData[ptr] = 255
        self.data.append(intData)
    
    def clear(self):
        self.name = None
        self.data = []


class ImgOperate:
    def __init__(self, filePath):
        self.imgList= []
        imgBuff = ImgData()
        isImgData = False
        f = open(filePath, 'r')
        for f_line in f:
            if (f_line[0] == '<' and f_line[-2] == '>' and not isImgData):
                isImgData = True
                imgBuff.setName(f_line[1:-2])
                continue
            elif (f_line[0:2] == '</' and f_line[-2] == '>' and isImgData):
                isImgData = False
                imgBuff.data = np.array(imgBuff.data)
                self.imgList.append(copy(imgBuff))
                imgBuff.clear()
            elif (isImgData):
                imgBuff.addStrData(f_line)
            else:
                pass
    
    def showImg(self, index):
        if (index < len(self.imgList)):
            plt.imshow(self.imgList[index].data,cmap='gray')
            plt.title(self.imgList[index].name)
            plt.show()
        else:
            print("[E]: Index Out of List")
