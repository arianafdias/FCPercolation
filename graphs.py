import matplotlib.pyplot as plt
import numpy as np

with open('Statistic.txt') as file:
    lst = []
    for line in file:
        lst.append([float(x) for x in line.split()])

col1 = [ x[0] for x in lst] # l
col2 = [ x[1] for x in lst] # p
col3 = [ x[2] for x in lst] # frac
col4 = [ x[3] for x in lst] # tmin medio
col5 = [ x[4] for x in lst] # tmedio

#for i in range(len(col3)):
#	col3[i] = 1- col3[i]

frac16 = []
tmin16 = []
tmed16 = []
frac32 = []
tmin32 = []
tmed32 = []
frac64 = []
tmin64 = []
tmed64 = []
frac128 = []
tmin128 = []
tmed128 = []
frac256 = []
tmin256 = []
tmed256 = []
p = []

for i in range(len(col1)):
	if col1[i] == 16:
		frac16.append(col3[i])
		tmin16.append(col4[i])
		tmed16.append(col5[i])
		p.append(col2[i])
	if col1[i] == 32:
		frac32.append(col3[i])
		tmin32.append(col4[i])
		tmed32.append(col5[i])
	if col1[i] == 64:
		frac64.append(col3[i])
		tmin64.append(col4[i])
		tmed64.append(col5[i])
	if col1[i] == 128:
		frac128.append(col3[i])
		tmin128.append(col4[i])
		tmed128.append(col5[i])
	if col1[i] == 256:
		frac256.append(col3[i])
		tmin256.append(col4[i])
		tmed256.append(col5[i])

plt.plot(p, frac256, 'o', label = 'L = 256')
plt.plot(p, frac128, 'o', label = 'L = 128')
plt.plot(p, frac64, 'o', label = 'L = 64')
plt.plot(p, frac32, 'o', label = 'L = 32')
plt.plot(p, frac16, 'o', label = 'L = 16')
plt.xlabel('p, probabilidade de ocupação', fontsize = 20)
plt.ylabel('probabilidade de haver agregado percolativo', fontsize = 20)
plt.xticks(fontsize = 20)
plt.yticks(fontsize = 20)
plt.legend(fontsize = 20)
plt.show()
