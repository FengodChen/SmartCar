#! /usr/bin/python3

# [Config]
filePath = './in.c'
outPath = './out.py'
fpName = 'cFile'
spaceNum = 0

# [Main]
fr = open(filePath, 'r')
fw = open(outPath, 'w')

for line in fr:
    fw.write("{}{}.write('{}\\n')\n".format(" "*spaceNum, fpName, line[:-1]))

fr.close()
fw.close()