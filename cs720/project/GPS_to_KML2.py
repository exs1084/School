# -*- coding: utf-8 -*-
"""
Created on Wed Apr  3 11:59:54 2019

@author: exs1084
"""
import math
import numpy as np
from sklearn.cluster import dbscan
from sklearn.cluster import DBSCAN

from sklearn.neighbors import KDTree

def calculate_angle(prev_coord,cur_coord):
    '''
    calculates bearing angle between two long/lat coordinates
    https://www.igismap.com/formula-to-find-bearing-or-heading-angle-between-two-points-latitude-longitude/
    '''
    long1,lat1 = prev_coord[0], prev_coord[1]
    long2,lat2 = cur_coord[0], cur_coord[1]
    deltaLong = long2 - long1
    
    x = math.sin(deltaLong) * math.cos(lat2)
    y = math.cos(lat1) * math.sin(lat2) - math.sin(lat1) * math.cos(lat2) * math.cos(deltaLong)    
    
    bearing = math.atan2(x,y)
    return math.degrees(bearing)


def kmlHeader():
    ret = ""
    ret += """<?xml version="1.0" encoding="UTF-8"?>\n
    <kml xmlns="http://www.opengis.net/kml/2.2">\n
    <Document>\n
    <Style id="yellowPoly">
        <LineStyle>
            <color>Af00ffff</color>
            <width>6</width>
        </LineStyle>
        <PolyStyle>
            <color>7f00ff00</color>
        </PolyStyle>
    </Style>\n"""
    return ret
def kmlTail():
    ret = ""
    ret += """
        </Document>
        </kml>"""
    return ret

def writeDataToKML(data, outFile):
    for x in data:
        outFile.write("%s,%s\n"%(x[0],x[1]))
    return


def cluster_pins(cur_class,minPts):
    '''
    does things
    
    thank you https://geoffboeing.com/2014/08/clustering-to-reduce-spatial-data-set-size/
    '''
            
    cur_class_np = np.array(cur_class)
    to_cluster = cur_class_np[:,[0,1]]
    
    #try this again with normaler weights see if it works
            
    db = DBSCAN(eps=1/75000.0, min_samples=minPts, algorithm='ball_tree', metric='haversine')
    
    a = db.fit(np.radians(to_cluster))
    b = db.fit_predict(np.radians(to_cluster))
    
    clusters = {}
    point_labels = a.labels_
    m = a.core_sample_indices_
    
    num_clusters = len(set(point_labels))
    
    #'''
    for index, point in enumerate(point_labels):
        if point in clusters:
            clusters[point].append(cur_class[index])
        else:
            clusters[point] = [cur_class[index]]
    
    cur_class_COMS = compute_COM(clusters)
    
    #print(clusters)
    return cur_class_COMS


def compute_COM(clusters):
    '''
    computes center of mass

    @param clusters: dictionary of clusters
    @return list of COMs for each cluster
    '''

    COM_list = []
    for key in clusters:
        cur_COM = []
        cluster = np.array(clusters[key])
        for coord in range(3):
            cur_COM.append(np.mean(cluster[:,coord]))
        COM_list.append(cur_COM)
        
    return COM_list

def get_distance(pt_a, pt_b):
    '''
    compute euclidean distance

    @param pt_a: note, index 0 is the pointID, index 1 is x-cord, etc
    @param pt_b: note, index 0 is the pointID, index 1 is x-cord, etc
    @return L2 Norm between 2 pts
    '''
    dx = (float(pt_a[0]) - float(pt_b[0]))**2
    dy = (pt_a[1] - pt_b[1])**2
    return (dx + dy) ** (0.5)

def haversine(lon1, lat1, lon2, lat2):
    """
    Thank you stackoverflow
    https://stackoverflow.com/questions/4913349/haversine-formula-in-python-bearing-and-distance-between-two-gps-points
    Calculate the great circle distance between two points 
    on the earth (specified in decimal degrees)
    """
    # convert decimal degrees to radians 
    lon1, lat1, lon2, lat2 = map(math.radians, [lon1, lat1, lon2, lat2])

    # haversine formula 
    dlon = lon2 - lon1 
    dlat = lat2 - lat1 
    a = math.sin(dlat/2)**2 + math.cos(lat1) * math.cos(lat2) * math.sin(dlon/2)**2
    c = 2 * math.asin(math.sqrt(a)) 
    r = 6371 # Radius of earth in kilometers. Use 3956 for miles
    return c * r


def writePins(pins, outFile):
    '''
    l = [1,2,3,4,5]
    for i, val in enumerate(l):
        if val %2 == 0:
            del l[i]
    print(l)
    '''
    
    stops = []
    left = []
    right = []
    for x in pins:
        if x[2] == 0:
            stops.append(x)
        if x[2] == 1:
            left.append(x)
        if x[2] == 2:
            right.append(x)
            
    clustered_right = cluster_pins(right,4)
    clustered_left = cluster_pins(left,4)
    clustered_stop = cluster_pins(stops,1)
    print(f'number of stops: {len(clustered_stop)}')
    
    left_array = np.array(clustered_left)
    left_array[:,2] = left_array[:,2]*100 #scale the class so that you always get closest neighbor of desired class
    right_array = np.array(clustered_right)
    right_array[:,2] = right_array[:,2]*100 #scale the class so that you always get closest neighbor of desired class
    stop_array = np.array(clustered_stop)
    
    inp = np.concatenate((left_array,right_array, stop_array),axis=0)
    
    #TODO: add speed as dimension of pins list (can eliminate turns on road, not due to stopping)
    #iterate through clustered_class and remove any with speed > 10 MPHish
    # now remove stops near turns
    
    
    kd_tree = KDTree(inp)
    kept_stop_signs = []
    for index, stop in enumerate(clustered_stop):
        cur_loc = np.array(stop)
        
        #print(f'current stop location: {cur_loc}')
        cur_stop_coords = cur_loc[0:2]
        
        left_index = cur_loc.copy()
        left_index[2] = 100 #scale the class so that you always get closest neighbor of desired class
        right_index = cur_loc.copy()
        right_index[2] = 200
        
        right_dist, close_right_index = kd_tree.query(right_index.reshape(1,-1)) #index of the closest right point
        left_dist, close_left_index = kd_tree.query(left_index.reshape(1,-1)) #index of the closest right point
        
        closest_left_coords = inp[close_left_index[0]][0]
        closest_right_coords = inp[close_right_index[0]][0]
        
        closest_left = haversine(cur_stop_coords[0],cur_stop_coords[1],closest_left_coords[0],closest_left_coords[1])
        closest_right = haversine(cur_stop_coords[0],cur_stop_coords[1],closest_right_coords[0],closest_right_coords[1])
        
        if closest_left > 0.2 and closest_right > 0.2: #about a tenth of a mile
            kept_stop_signs.append(clustered_stop[index])
            
    
    print(f'number of stops: {len(clustered_stop)}')
    #for x in stops:
    for x in kept_stop_signs :
        outFile.write("""<Placemark>
                  <description>StopLight</description>\n""")
        outFile.write("""<Point><coordinates>%s, %s, 0.0</coordinates></Point>\n"""%(x[0],x[1]))
        outFile.write("""</Placemark>""")
        
    #for x in left:
    for x in clustered_left:
        outFile.write("""<Placemark>
                  <description>Left Turn</description>
                  <Style id="normalPlacemark"><IconStyle>
                      <color>ff0000ff</color>
                      <Icon><href>http://maps.google.com/mapfiles/kml/paddle/1.png</href></Icon>
                      </IconStyle></Style>\n""")
        outFile.write("""<Point><coordinates>%s, %s, 0.0</coordinates></Point>\n"""%(x[0],x[1]))
        outFile.write("""</Placemark>""")
        
    #for x in right:
    for x in clustered_right:
        outFile.write("""<Placemark>
                  <description>Right Turn</description>
                  <Style id="normalPlacemark"><IconStyle>
                      <color>ff00ff00</color>
                      <Icon><href>http://maps.google.com/mapfiles/kml/paddle/1.png</href></Icon>
                      </IconStyle></Style>\n""")
        outFile.write("""<Point><coordinates>%s, %s, 0.0</coordinates></Point>\n"""%(x[0],x[1]))
        outFile.write("""</Placemark>""")
        
        
def line1Good(line):#makes sure the GPRMC line is present
    if len(line) is not 13:
        return False
    if line[0] != "$GPRMC":
        return False
    if line[3] is '' or line[4] is '' or line[5] is '' or line[6] is '':
        return False
    return True
    
def line2Good(line):#makes sure the GPGGA line is present
    if len(line) is not 15:
        return False
    if line[0] != "$GPGGA":
        return False
    if line[2] is '' or line[3] is '' or line[4] is '' or line[5] is '':
        return False
    return True
    
def degreeToDec(string, direction):#converts lat/lon from degree-min-sec format to decimal format
    degrees=0
    minutes=0
    if direction == 'N' or direction == 'S':
        degrees = float(string[0:2])
        minutes = float(string[2:])
    else:
        degrees = float(string[0:3])
        minutes = float(string[3:])
    decimal = degrees + minutes/60
    if direction == 'S' or direction == 'W':
        return decimal * -1
    else:
        return decimal
    
def reformatTime(time):
    hour = time[0:2]
    mins = time[2:4]
    secs = time[4:]
    return float(hour) + float(mins)/60 + float(secs)/3600


def GPSToKML(inFilenames, outFilename):
    outFile = open(outFilename, "w")#open file
    outFile.write(kmlHeader())
    pins = list()#fill with [lon, lat, class]. 0=stop, 1=left, 2=right
    
    for x in inFilenames:
        inFilename = x
        inFile = open(inFilename, "r")#open file
        data = list()
        last_bearing = 0    
        recentStop = False#var to catch stops
        timeStopped = 0
        rawline1 = inFile.readline()
        while rawline1:
            line1 = rawline1.split(',')
            if line1Good(line1):
                rawline2 = inFile.readline()
                if not rawline2:
                    break
                line2 = rawline2.split(',')
                if line2Good(line2):
                    lat = (degreeToDec(line1[3], line1[4]) + degreeToDec(line2[2], line2[3]))/2
                    lon = (degreeToDec(line1[5], line1[6]) + degreeToDec(line2[4], line2[5]))/2
                    time = reformatTime(line1[1])
                    speed = float(line1[7])
                    if speed < 2:#2 knots = 2.3mph
                        if(recentStop == False):
                            timeStopped = time#record when it first stopped
                        recentStop = True
                    if speed > 5:#5 knots = 5.8mph
                        if recentStop:
                            recentStop = False
                            if time - timeStopped < .1:#stopped for less than 6 minutes
                                pins.append([lon, lat, 0, speed])
                        #compare previous datapoint, calculate bearing, add if greater than .2 degree
                        if len(data) > 0:
                            bearing = calculate_angle(data[-1],[lon,lat])
                            delta_bearing = abs(abs(last_bearing)-abs(bearing))
                            #print(delta_bearing) #testing purposes
                            if delta_bearing  > .2:
                                data.append([lon, lat, bearing, time])
                                last_bearing = bearing
                            
                        #if data is empty, nothing to compare bearing to, so add it
                        else:
                            data.append([lon, lat, 0, time])
                        
                        if len(data) > 25:#.2s difference in time each record, 5 seconds is at least 25 records
                            for x in range(len(data)-2, len(data)-25, -1):
                                angle = bearing - data[x][2]
                                timeDiff = time - data[x][3]
                                if(timeDiff) > 5/60:
                                    break
                                if(angle) > 45:
                                    pins.append([data[x][0], data[x][1], 2, speed])#right turn
                                    break
                                if(angle) < -45:
                                    pins.append([data[x][0], data[x][1], 1, speed])#left turn
                                    break
                        
                    rawline1 = inFile.readline()
                else:
                    line1 = line2
            else:
                rawline1 = inFile.readline()
        
        #write to a kml file after here?
        outFile.write("""<Placemark><styleUrl>#yellowPoly</styleUrl>
    <LineString>
    <Description>.</Description>
        <extrude>1</extrude>
        <tesselate>1</tesselate>
        <coordinates>\n""")
        writeDataToKML(data, outFile)#project only states needing the pins, not the path anymore...drop this?
        outFile.write("""    \t</coordinates>
            </LineString>
            </Placemark>\n""")
        inFile.close()
    
    
    #prune the pins here
    writePins(pins, outFile)
    outFile.write(kmlTail())
    outFile.close()
    return "pickle"





def main():
    #note: '2019_03_19__1310_26.txt' throws an error
    #inFiles = ["2019_03_05__RIT_to_Home.txt", "2019_03_03__RIT_to_Home.txt",'2019_03_04__Parked_at_RIT.TXT','2019_03_04__RIT_to_HOME.TXT']
    inFiles = ['2019_03_03__RIT_to_HOME.txt']
    outFile = "output.kml"
    GPSToKML(inFiles, outFile)
    
if __name__ == "__main__":
    main()