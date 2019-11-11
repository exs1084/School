'''
Justin Lad
BDA: HW8
3/31/2019

Dendrogram Creation Program
'''

from scipy.cluster import hierarchy
from scipy.spatial.distance import pdist
import pandas as pd
import numpy as np
from math import sin, cos, asin, sqrt, radians, inf
from matplotlib import pyplot as plt
from scipy.cluster.hierarchy import dendrogram

EARTH_RADIUS = 6371#average radius in km


def hav(theta):#haversine subFunc
    return pow(sin(theta/2),2)

def haversine(loc1, loc2):#in [lat, long]
	'''
	function to compute haversine distance in dendrogram
	'''
    lat1 = radians(loc1[0])
    lat2 = radians(loc2[0])
    lon1 = radians(loc1[1])
    lon2 = radians(loc2[1])
    temp =  hav(abs(lat2-lat1))+cos(lat1)*cos(lat2)*hav(abs(lon2-lon1))
    return EARTH_RADIUS * 2 * asin(min(1,sqrt(temp)))


def create_dendrogram(data):
	'''
	Creates a dendrogram starting with 50 clusters, using haversine distance 
	@param data: nx2 numpy array of lat/long coordinates
	'''
	pdists = pdist(data,metric=haversine)
	Z = hierarchy.linkage(pdists, method='average')
	dn = hierarchy.dendrogram(Z,p=50,truncate_mode='lastp',count_sort='ascending',color_threshold=10.0)
	plt.xlabel('Cluster Group ID')
	plt.title('Dendrogram for Top 50 Cities (Average Linkage)')
	plt.ylabel('Haversine Distance (km)')
	plt.show()



def main():
	df = pd.read_csv('lat_long_pts.csv')
	data = np.array(df)
	create_dendrogram(data)

main()