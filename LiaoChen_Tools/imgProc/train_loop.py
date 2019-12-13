#! /usr/bin/python3

import imgFunc

# [Config]
classList = ['L', 'R', 'GO', 'STOP']
imgPath = "./log_file/log.txt"
loadPath = "./net_data/best.tkl"
savePath = "./net_data/better.tkl"

# [Main Function]
ga_main = imgFunc.GA_Main(10, 10, imgPath, classList)
ga_main.load(loadPath)
minLoss = min(ga_main.getLoss())

while (True):
    ga_main.train(50, 0.00001)
    if (minLoss > min(ga_main.getLoss())):
        ga_main.save(savePath)
        loadPath = savePath
        minLoss = min(ga_main.getLoss())
        print("Great! Saved Net!")
    else:
        ga_main.load(loadPath)