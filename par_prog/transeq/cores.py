# -*- coding: utf-8 -*-
import sys
import os
import subprocess
import re

import numpy as np
import matplotlib.pyplot as plt

if len(sys.argv) != 2:
    print("Error: executable file must be specified")
    exit()

# Extract cpp programm from command line arguments
cppProgram = sys.argv[1]

startCore = 1
endCore = 4
nCores = endCore - startCore + 1
timeArray = np.zeros(nCores)

for n in range(startCore, endCore + 1):
	proc = subprocess.Popen(["mpirun", "-np", str(n), cppProgram, "1000", "2000"], stdout=subprocess.PIPE)
	output, error = proc.communicate()
	regResult = re.findall("\\d+\\.\\d+", str(output))
	timeArray[n - startCore] = float(regResult[0]);

# Plot (time, nCore)
nCoreArray = [i for i in range(startCore, endCore + 1)]

plt.figure(figsize=(11.7,8.3))
plt.title("Зависимость времени выполнения от колличества ядер")
plt.grid(which='both')
plt.grid(which='minor', alpha=0.2)
plt.grid(which='major', alpha=0.5)
plt.minorticks_on()
plt.autoscale()
plt.xlabel("$n$, число ядер", fontsize=10)
plt.ylabel("$T$, время выполнения", fontsize=10)
plt.plot(nCoreArray, timeArray, 'bo-')
plt.savefig("core_time.png")
plt.show()