import matplotlib.pyplot as plt

with open('Statistic.txt') as file:
    lst = []
    for line in file:
        lst.append([float(x) for x in line.split()])

col1 = [ x[0] for x in lst] # l
col2 = [ x[1] for x in lst] # p
col3 = [ x[2] for x in lst] # frac
col4 = [ x[3] for x in lst] # tmin medio
col5 = [ x[4] for x in lst] # tmedio

plt.plot(col2, col3, 'o')
plt.show()
