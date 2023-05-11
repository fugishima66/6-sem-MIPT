import csv
import sys
import matplotlib.pyplot as plt

from matplotlib.widgets import Slider
from math import ceil

if len(sys.argv) != 2:
    print("Error: CSV file must be specified")
    exit()

meshResult = []
rightSolution = []

fileName = sys.argv[1]
with open(fileName) as csvFile:
    reader = csv.reader(csvFile, delimiter=' ')
    
    lineCount = 0
    for row in reader:
        floatRow = [float(val) if val != '' else val for val in row]
        if lineCount == 0:
            x0, x1, hx, nx = floatRow[0], floatRow[1], floatRow[2], floatRow[3]
            t0, t1, ht, nt = floatRow[4], floatRow[5], floatRow[6], floatRow[7]

            nx = int(nx)
            nt = int(nt)
        elif lineCount < 1 + nt:
            meshResult.append(floatRow[0:nx])
        else:
            rightSolution.append(floatRow[0:nx])

        lineCount += 1

x = [i * hx for i in range(0, nx)]

plt.figure(figsize=(11.7,8.3))
plt.title("Сравнение аналитического и численного решений")
plt.grid(which='both')
plt.grid(which='minor', alpha=0.2)
plt.grid(which='major', alpha=0.5)
plt.minorticks_on()
plt.autoscale()
plt.xlabel("$x$", fontsize=10)
plt.ylabel("$u(x)$", fontsize=10)
lineCalc, = plt.plot(x, meshResult[0], 'r', label='численное решение')
lineTrue, = plt.plot(x, rightSolution[0], 'g--', label='аналитическое решение')
plt.legend()
plt.subplots_adjust(bottom = 0.25)

# Ползунок для времени
axtime = plt.axes([0.25, 0.1, 0.65, 0.03])
timeSlider = Slider(
  ax = axtime,
  label = 'Время (в отсчетах)',
  valmin = 0,
  valmax = nt - 1,
  valstep = 1,
  valinit = 0,
)

def update(val):
	lineCalc.set_ydata(meshResult[val])
	lineTrue.set_ydata(rightSolution[val])

timeSlider.on_changed(update)
plt.show()
