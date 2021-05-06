import matplotlib.pyplot as plt
import numpy as np
with open("fuzzyout.dat") as f:
    data = f.read()

data = data.split('\n')
data = [x for x in data if x != '']
input = [float(row.split('\t')[0]) for row in data]
ref = [float(row.split('\t')[1]) for row in data]
output = [float(row.split('\t')[2]) for row in data]
x = list(range(0, len(ref)))
fig = plt.figure()

ax1 = fig.add_subplot(111)

ax1.set_title("Temperature control")    
ax1.set_xlabel('time')
ax1.set_ylabel('output')

ax1.plot(x,input, c='g', label='input')
ax1.plot(x,output, c='b', label='output')
ax1.plot(x,ref, c='r', label='Reference')

leg = ax1.legend()

plt.show()