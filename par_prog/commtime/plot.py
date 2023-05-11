# -*- coding: utf-8 -*-
import csv
import sys
import matplotlib.pyplot as plt

from matplotlib.widgets import Slider
from math import ceil

if len(sys.argv) != 2:
    print("Error: CSV file must be specified")
    exit()

valueArray = []
timeArray = []

for i in range (1, 20):
    valueArray.append(i)

fileName = sys.argv[1]

with open(fileName) as csvFile:
    reader = csv.reader(csvFile, delimiter=' ')
    for row in reader:
        floatRow = [float(val) if val != '' else val for val in row]
        for i in range(0, 19):
            timeArray.append(floatRow[i])

plt.figure(figsize=(11.7,8.3))
plt.title("Зависимость времени пересылки от передаваемого значения")
plt.grid(which='both')
plt.grid(which='minor', alpha=0.2)
plt.grid(which='major', alpha=0.5)
plt.minorticks_on()
plt.autoscale()
plt.xlabel("$n$, пересылаемое значение, 2^n", fontsize=10)
plt.ylabel("$T$, время пересылки", fontsize=10)
plt.plot(valueArray, timeArray, 'bo-')
plt.savefig("comm_time.png")
plt.show()