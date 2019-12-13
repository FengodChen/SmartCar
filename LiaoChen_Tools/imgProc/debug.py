import imgFunc

imgPath = "./log_file/log.txt"
classList = ['L', 'R', 'GO', 'STOP']
ga_main = imgFunc.GA_Main(imgPath, classList)
ga_main.train(50, 0.05)

'''
imgOperate = imgFunc.ImgOperate(imgPath)
ga = imgFunc.GA(10, 10, imgOperate, classList)
ga.create()

def seeAns(index):
    (dna1, dna2, loss1, loss2) = ga.getBestDNAs()
    print("True Ans: {}". format(imgOperate.imgList[index].name))
    print(ga.runAns(imgOperate.imgList[index].data, dna1))
    print(ga.runAns(imgOperate.imgList[index].data, dna2))
    print(classList)

def train(trainNum, mutationRate = 0.3, selfGeneticRate = 0.8, averageRate = 0.2):
    for i in range(trainNum):
        print("Runing {}/{}".format(i+1, trainNum))
        (dna1, dna2, loss1, loss2) = ga.getBestDNAs()
        if (loss1 < loss2):
            ga.reproduce(dna1, dna2, mutationRate, selfGeneticRate, averageRate)
        else:
            ga.reproduce(dna2, dna1, mutationRate, selfGeneticRate, averageRate)

'''
