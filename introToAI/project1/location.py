# -*- coding: utf-8 -*-
"""
Created on Sun Feb 24 13:22:06 2019

@author: Erikson
"""

import math

class location:
    xPix = 0
    yPix = 0
    x = 0.0
    y = 0.0
    z = 0.0
    rate = 1.0
    heur = math.inf
    bestDist = math.inf
    bestParent = None#update with x, y of parent
    usedGoal = 1#which brench of the search this has been checked in.
                #needed to efficiently clear out other fields during a new branch
    queuedForSeason = False
    queuedWaterLevel = 0

