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
from copy import copy
import numpy as np
import matplotlib.pyplot as plt
import random
import pickle
import time
import multiprocessing

IMG_H = 60
IMG_W = 80

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
        self.data.append(intData)
    
    def clear(self):
        self.name = None
        self.data = []


class ImgOperate:
    def __init__(self, filePath):
        self.imgList= []
        self.imgNum = 0
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
                self.imgNum += 1
                print("Loading, number is: ", self.imgNum)
            elif (isImgData):
                imgBuff.addStrData(f_line)
            else:
                pass
        f.close()
    
    def showImg(self, index):
        if (index < len(self.imgList)):
            plt.imshow((self.imgList[index].data)*255,cmap='gray')
            plt.title(self.imgList[index].name)
            plt.show()
        else:
            print("[E]: Index Out of List")

class ClassOperate:
    def __init__(self, classList):
        self.classList = classList
        self.classNum = len(classList)
    
    def getOneHot(self, className):
        classOneHot = np.zeros(self.classNum)
        if (className in self.classList):
            classOneHot[self.classList.index(className)] = 1
        return classOneHot


class GA_DNA:
    def __init__(self, branchNum, classNum, minR = -1, maxR = 1):
        if (minR >= maxR):
            print("[W]: minR >= maxR, set minR = -1 and maxR = 1")
            minR = -1
            maxR = 1
        self.minR = minR
        self.maxR = maxR
        self.branchNum = branchNum
        self.classNum = classNum
        self.array1 = None
        self.array2 = None
    
    def getRandomDNA(self):
        self.array1 = []
        for i in range(self.branchNum):
            randList = np.random.rand(IMG_H, IMG_W)
            randList = randList * (self.maxR - self.minR) + self.minR
            self.array1.append(randList.copy())
        self.array1 = np.array(self.array1)
        self.array2 = np.random.rand(self.branchNum, self.classNum)
    
    def copyDNA(self, ga_dna):
        if (self.branchNum == ga_dna.branchNum and self.classNum == ga_dna.classNum):
            self.array1 = ga_dna.array1.copy()
            self.array2 = ga_dna.array2.copy()
        else:
            print("[E]: DNA Not Match")
    
    def partnership(self, value1, value2, mutationRate, selfGeneticRate, averageRate):
        # mutation
        if (random.random() <= mutationRate):
            return random.random() * (self.maxR - self.minR) + self.minR
        # partnership
        else:
            # only one
            if (random.random() > averageRate):
                if (random.random() < selfGeneticRate):
                    return value1
                else:
                    return value2
            # average
            else:
                return ((value1+value2)/2)*(1+(random.random()-0.5)/5)
    
    def reproduce(self, ga_dna, mutationRate = 0.3, selfGeneticRate = 0.8, averageRate = 0.2):
        if (self.branchNum == ga_dna.branchNum and self.classNum == ga_dna.classNum):
            ga_dna_child = GA_DNA(self.branchNum, self.classNum, self.minR, self.maxR)
            ga_dna_child.array1 = np.zeros((self.branchNum, IMG_H, IMG_W))
            ga_dna_child.array2 = np.zeros((self.branchNum, self.classNum))
            # array1
            for i in range(self.branchNum):
                for j in range(IMG_H):
                    for k in range(IMG_W):
                        ga_dna_child.array1[i,j,k] = self.partnership(self.array1[i,j,k], ga_dna.array1[i,j,k], mutationRate, selfGeneticRate, averageRate)
            # array2
            for i in range(self.branchNum):
                for j in range(self.classNum):
                    ga_dna_child.array2[i,j] = self.partnership(self.array2[i,j], ga_dna.array2[i,j], mutationRate, selfGeneticRate, averageRate)
            
            return ga_dna_child
        else:
            print("[E]: DNA Not Match")
    
    '''
    def reproduce_array1_threads(self, childArray1, selfArray1, otherArray1, i, j, k, mutationRate, selfGeneticRate, averageRate):
        childArray1[i,j,k] = self.partnership(selfArray1[i,j,k], otherArray1[i,j,k], mutationRate, selfGeneticRate, averageRate)
        self.sum_tmp += 1
        #print(self.sum_tmp)
    '''
    def reproduce_array1_threads(self, selfArray1, otherArray1, i, mutationRate, selfGeneticRate, averageRate):
        #tmp_array = np.zeros(IMG_H, IMG_W)
        #tmp_array = [([0]*IMG_W) for i in range(IMG_H)]
        tmp_array = []
        for i in range(IMG_H):
            tmp_array.append([0]*IMG_W)
        for j in range(IMG_H):
            for k in range(IMG_W):
                tmp_array[j,k] = self.partnership(selfArray1[i,j,k], otherArray1[i,j,k], mutationRate, selfGeneticRate, averageRate)
        return (tmp_array, i)

    def reproduce_array2_threads(self, childArray2, selfArray2, otherArray2, i, j, mutationRate, selfGeneticRate, averageRate):
        childArray2[i,j] = self.partnership(selfArray2[i,j], otherArray2[i,j], mutationRate, selfGeneticRate, averageRate)

    def reproduce_threads(self, ga_dna, mutationRate = 0.3, selfGeneticRate = 0.8, averageRate = 0.2):
        if (self.branchNum == ga_dna.branchNum and self.classNum == ga_dna.classNum):
            ga_dna_child = GA_DNA(self.branchNum, self.classNum, self.minR, self.maxR)
            ga_dna_child.array1 = np.zeros((self.branchNum, IMG_H, IMG_W))
            ga_dna_child.array2 = np.zeros((self.branchNum, self.classNum))
            # array1
            pool = multiprocessing.Pool()
            resList = []
            for i in range(self.branchNum):
                res = pool.apply_async(self.reproduce_array1_threads,args=(self.array1, ga_dna.array1, i, mutationRate, selfGeneticRate, averageRate))
                resList.append(res)
            pool.close()
            pool.join()
            for res in resList:
                (array, ptr) = res.get()
                #ga_dna_child[ptr] = np.array(array)
                print(np.array(array))

            # array2
            for i in range(self.branchNum):
                for j in range(self.classNum):
                    ga_dna_child.array2[i,j] = self.partnership(self.array2[i,j], ga_dna.array2[i,j], mutationRate, selfGeneticRate, averageRate)
            
            return ga_dna_child
        else:
            print("[E]: DNA Not Match")

class GA:
    def __init__(self, branchNum, DNANum, imgOperate, classList):
        self.branchNum = branchNum
        self.classList = classList
        self.classNum = len(classList)
        self.DNANum = DNANum
        self.imgOperate = imgOperate
        self.classOperate = ClassOperate(classList)
        self.DNAs_1 = None
        self.DNAs_2 = None

        self.perClassNum = {}
        for className in classList:
            self.perClassNum[className] = 0
        for imgData in imgOperate.imgList:
            imgName = imgData.name
            if (imgName in self.perClassNum):
                self.perClassNum[imgName] += 1
        
        self.loss = None
    
    def create(self):
        dna = GA_DNA(self.branchNum, self.classNum)
        self.DNAs_1 = []
        self.DNAs_2 = []
        for i in range(self.DNANum):
            dna.getRandomDNA()
            self.DNAs_1.append(copy(dna))
        for i in range(self.DNANum):
            dna.getRandomDNA()
            self.DNAs_2.append(copy(dna))
    
    def load(self, ga):
        if (self.branchNum == ga.branchNum and self.classNum == ga.branchNum):
            self.DNANum = ga.DNANum
            self.DNAs_1 = copy(ga.DNAs_1)
            self.DNAs_2 = copy(ga.DNAs_2)
        else:
            print("[E]: GA Not Match")

    def runAns(self, img, dna):
        p1 = []
        tmp = img*dna.array1
        for m in tmp:
            p1.append(m.sum())
        p1 = np.array(p1)
        ans = np.matmul(p1, dna.array2)
        # softmax
        ans /= 1000
        ans = np.exp(ans)
        ans_sum = ans.sum()
        ans /= ans_sum
        return ans
    
    def getBestDNAs(self):
        loss = []
        for DNA in self.DNAs_1:
            loss_sum = 0
            for imgData in self.imgOperate.imgList:
                ans = self.runAns(imgData.data, DNA)
                ans = (ans * self.classOperate.getOneHot(imgData.name)).sum()
                loss_sum += (float(-np.log(ans))/self.perClassNum[imgData.name])
            #loss_sum /= self.imgOperate.imgNum
            loss.append(copy(loss_sum))
        loss1 = min(loss)
        bestDNA1 = copy(self.DNAs_1[loss.index(loss1)])

        loss = []
        for DNA in self.DNAs_2:
            loss_sum = 0
            for imgData in self.imgOperate.imgList:
                ans = self.runAns(imgData.data, DNA)
                ans = (ans * self.classOperate.getOneHot(imgData.name)).sum()
                loss_sum += (float(-np.log(ans))/self.perClassNum[imgData.name])
            #loss_sum /= self.imgOperate.imgNum
            loss.append(copy(loss_sum))
        loss2 = min(loss)
        bestDNA2 = copy(self.DNAs_2[loss.index(loss2)])

        return (bestDNA1, bestDNA2, loss1, loss2)
    
    def getLoss_threads(self, DNA, ptr):
        loss_sum = 0
        for imgData in self.imgOperate.imgList:
            ans = self.runAns(imgData.data, DNA)
            ans = (ans * self.classOperate.getOneHot(imgData.name)).sum()
            loss_sum += (float(-np.log(ans))/self.perClassNum[imgData.name])
        return (ptr, loss_sum)
    
    def getBestDNAs_threads(self):
        ptr_1 = 0
        ptr_2 = 0

        pool = multiprocessing.Pool()

        resList_1 = []
        resList_2 = []
        for DNA in self.DNAs_1:
            res = pool.apply_async(self.getLoss_threads, args=(DNA, ptr_1))
            resList_1.append(res)
            ptr_1 += 1
        for DNA in self.DNAs_2:
            res = pool.apply_async(self.getLoss_threads, args=(DNA, ptr_2))
            resList_2.append(res)
            ptr_2 += 1
        pool.close()
        pool.join()

        loss = np.zeros(self.DNANum)

        for res in resList_1:
            (ptr, loss_sum) = res.get()
            loss[ptr] = loss_sum
        loss1 = min(loss)
        bestDNA1 = copy(self.DNAs_1[loss.argmin()])

        for res in resList_2:
            (ptr, loss_sum) = res.get()
            loss[ptr] = loss_sum
        loss2 = min(loss)
        bestDNA2 = copy(self.DNAs_2[loss.argmin()])

        return (bestDNA1, bestDNA2, loss1, loss2)
    
    def reproduce(self, DNA1, DNA2, mutationRate = 0.3, selfGeneticRate = 0.8, averageRate = 0.2):
        self.DNAs_1 = []
        self.DNAs_2 = []
        for i in range(self.DNANum):
            self.DNAs_1.append(DNA1.reproduce(DNA2, mutationRate, selfGeneticRate, averageRate))
            self.DNAs_2.append(DNA1.reproduce(DNA2, mutationRate, selfGeneticRate, averageRate))

    def reproduce_threads(self, DNA1, DNA2, mutationRate = 0.3, selfGeneticRate = 0.8, averageRate = 0.2):
        self.DNAs_1 = []
        self.DNAs_2 = []
        for i in range(self.DNANum):
            self.DNAs_1.append(DNA1.reproduce_threads(DNA2, mutationRate, selfGeneticRate, averageRate))
            self.DNAs_2.append(DNA1.reproduce_threads(DNA2, mutationRate, selfGeneticRate, averageRate))

class GA_Main:
    def __init__(self, branchNum, DNANum, imgPath, classList):
        self.imgOperate = ImgOperate(imgPath)
        self.classList = classList
        self.ga = GA(branchNum, DNANum, self.imgOperate, classList)
        self.ga.create()

    def seeAns(self, index):
        (dna1, dna2, loss1, loss2) = self.ga.getBestDNAs_threads()
        print("True Ans: {}". format(self.imgOperate.imgList[index].name))
        print(self.ga.runAns(self.imgOperate.imgList[index].data, dna1))
        print(self.ga.runAns(self.imgOperate.imgList[index].data, dna2))
        print(self.classList)
    
    def train(self, trainNum, mutationRate = 0.05, selfGeneticRate = 0.8, averageRate = 0.2):
        for i in range(trainNum):
            ts = time.time()
            print("Runing {}/{}".format(i+1, trainNum))
            (dna1, dna2, loss1, loss2) = self.ga.getBestDNAs_threads()
            print("\tloss1 = {}, loss2 = {}".format(loss1, loss2))
            tm = time.time()
            print("\t[GetBestDNA]Used time: {}".format(tm-ts))
            if (loss1 < loss2):
                self.ga.reproduce(dna1, dna2, mutationRate, selfGeneticRate, averageRate)
            else:
                self.ga.reproduce(dna2, dna1, mutationRate, selfGeneticRate, averageRate)
            te = time.time()
            print("\t[Reproduce]Used time: {}".format(te-tm))
            print("\t[All]Used time: {}".format(te-ts))
    
    def save(self, filePath):
        classData = pickle.dumps(self)
        saveFile = open(filePath, 'wb')
        saveFile.write(classData)
        saveFile.close()
    
    def load(self, filePath):
        loadFile = open(filePath, 'rb')
        tmpSelf = copy(self)
        tmpSelf = pickle.loads(loadFile.read())
        self.ga.branchNum = tmpSelf.ga.branchNum
        self.ga.DNANum = tmpSelf.ga.DNANum
        self.ga.DNAs_1 = tmpSelf.ga.DNAs_1
        self.ga.DNAs_2 = tmpSelf.ga.DNAs_2
        loadFile.close()
    
    def getLoss(self):
        (dna1, dna2, loss1, loss2) = self.ga.getBestDNAs()
        return (loss1, loss2)
