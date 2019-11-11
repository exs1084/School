'''
Justin Lad
BDA: HW8
3/31/2019

City, State --> Lat, Long 
'''
import pandas as pd
from geopy.geocoders import Nominatim
import csv
import math


def haversine(lon1, lat1, lon2, lat2):
    """
    Thank you stackoverflow
    https://stackoverflow.com/questions/4913349/haversine-formula-in-python-bearing-and-distance-between-two-gps-points
    Calculate the great circle distance between two points 
    on the earth (specified in decimal degrees)
    """
    # convert decimal degrees to radians 
    lon1, lat1, lon2, lat2 = map(radians, [lon1, lat1, lon2, lat2])

    # haversine formula 
    dlon = lon2 - lon1 
    dlat = lat2 - lat1 
    a = sin(dlat/2)**2 + cos(lat1) * cos(lat2) * sin(dlon/2)**2
    c = 2 * asin(sqrt(a)) 
    r = 6371 # Radius of earth in kilometers. Use 3956 for miles
    return c * r


def convert_data(file_name):
	'''
	converts csv file containing City,Country to a csv file
	containing lat,long
	@param file_name: csv file with City,Country to convert to lat,long
	'''
	df = pd.read_csv(file_name)
	geo_api = Nominatim(user_agent="open_sesame")
	csv_writer = csv.writer(open('lat_long_pts.csv','w',newline=''))
	csv_writer.writerow(['Lat','Long'])
	for ind,place in df.iterrows():
		location = geo_api.geocode(place['City']+place['Country'])
		if location == None:
			#print(f'couldnt find location {place["City"] + place["Country"] } at index {ind}')
			#manually write this single unfound location from https://www.latlong.net/
			csv_writer.writerow(['4.880680','114.922270']) 
			continue
		csv_writer.writerow([location.latitude,location.longitude])
		

file_name = 'CS_720_City_Country.csv'
convert_data(file_name)