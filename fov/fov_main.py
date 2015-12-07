'''
 Create: 	12/1/2015
 Author: 	Xiaochen Liu

 [How to use]
 ### In command line:
  - Input "$ python fov_detect.py [view_point_file] [triangulation_file] [res_dir]"
  - Note: [view_point_file] and [triangulation_file] can be found in 'code/sample_data/'
  - Fov detection result and all result images will be stored in [res_dir]
'''
import os, sys, time
import parse
from common import *
import download_map
import gps_dist
import detect_fov
 
RES_DIR = 'res/'
RES_FILE = 'result.txt'
MAP_SCALE = 19

# append message to file
def append_file (msg, file_path):
	f = open(file_path, 'a')
	f.write(msg + '\n')
	f.close
	# return

if __name__ == '__main__':
	if len(sys.argv) != 4:
		sys.exit('ERROR: wrong argc! Should be: python fov_detect.py [view_point_file] [triangulation_file] [res_dir]')
	if not os.path.exists(sys.argv[1]):
		sys.exit('ERROR: src path: ' + sys.argv[1] + ' was not found!')
	if not os.path.exists(sys.argv[2]):
		sys.exit('ERROR: src path: ' + sys.argv[2] + ' was not found!')
	# how to draw view points (connect the 360 points)

	vp_file = sys.argv[1] 
	t_file = sys.argv[2]

	RES_DIR = sys.argv[3]
	if RES_DIR[-1] != '/':
		RES_DIR += '/'
	if not os.path.exists(RES_DIR):
		os.makedirs(RES_DIR)
	
	raw_data_list = parse.parse(vp_file, t_file) # parse to get the triangulation result with all viewpoint (vp) in GPS and bearing
	
	for i in range(len(raw_data_list)): # for each t_point
		num_inside = 0
		for j in range(raw_data_list[i].vp_list_len): # for each vp of the t_point
			map_path = download_map.download_map(raw_data_list[i].vp_list[j], MAP_SCALE, RES_DIR, i, j) # download the map image for this vp
			# Function to get the relative distance from vp to t_p
			# Input: vp_lat,lng; t_lat,lng;  
			# Return distance vector: [x_offset, y_offset]
			dist_vec = gps_dist.get_distance(raw_data_list[i].vp_list[j].lat, raw_data_list[i].vp_list[j].lng, \
										raw_data_list[i].t_lat, raw_data_list[i].t_lng) 
			print 'dist vec: ' + `dist_vec[0]` + ' ' + `dist_vec[1]`
			# function of detect_fov:
			#	1. find the edge points
			#	2. draw edge points, origin, and t_p
			# 	3. return if the t_p is in the FoV 
			# 	4. (TBD) draw the sight_line from vp and a circle around the t_p
			# Input: [map_path], [vp_point_bearing] [distance_vector]
			is_inside = detect_fov.detect_fov(map_path, raw_data_list[i].vp_list[j].bearing, dist_vec) 
			if is_inside:
				num_inside += 1
		append_file(`i`+ ':' + `num_inside`, RES_DIR + RES_FILE) # write result to file
		# TO REMOVE
		break # this is temporary (for test)
	
	print "done"
