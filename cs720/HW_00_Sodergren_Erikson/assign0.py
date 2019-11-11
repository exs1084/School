# -*- coding: utf-8 -*-
"""
Created on Tue Jan 22 17:47:20 2019

@author: Erikson
"""

import matplotlib.pyplot as pyplot
import pandas

#question 1
speed = list(range(1, 81))
times = []

for x in speed:
    time = (20/x)*60
    times.append(time)
    
df = pandas.DataFrame(times, index = speed, columns=["minutes driven"])
pyplot.figure()
ax = df.plot(legend=False, title = "minutes to drive 20mi at various speeds")
ax.set_xlabel("speed(mph)")
ax.set_ylabel("minutes")

#question 2
gains = []

for x in range(5, 80, 5):
    gains.append(times[x-1] - times[x+4])
    
df2 = pandas.DataFrame(gains, index = speed[4:79:5], columns=["minutes driven"])
pyplot.figure()
ax2 = df2.plot(legend=False, title = "reduction in driving time as speed increases")
ax2.set_xlabel("speed(mph)")
ax2.set_ylabel("time saved by driving 5mph faster(minutes)")

#question 3
cogA = 73
cogB = 51
cogC = 31
backCogs = [19, 23, 33, 41, 53, 63, 71]

ratiosA = []
ratiosB = []
ratiosC = []
for x in backCogs:
    ratiosA.append(cogA/x)
    ratiosB.append(cogB/x)
    ratiosC.append(cogC/x)
    
values = [ratiosA, ratiosB, ratiosC]
df3 = pandas.DataFrame({"cogA": ratiosA, "cogB": ratiosB, "cogC": ratiosC, "backC": backCogs})
pyplot.figure()
ax3 = df3.plot(x = "backC", y = ["cogA", "cogB", "cogC"], title = "gear ratios of different front cogs with back cogs")
ax3.set_xlabel("teeth on back cog")
ax3.set_ylabel("gear ratio")