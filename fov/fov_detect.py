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
 
RES_DIR = 'res/'
RES_FILE = 'result.txt'
MAP_SCALE = 19

def get_name(impath, name_type): # ge the name or pos from image path
	ret_val_list = impath.split('/')[-1].split(SPLITTER)
	if name_type == 'name':
		return ret_val_list[0]
	else:
		return ret_val_list[-1].split('.')[0]

# detect the fov: edge_pt is the list made of end point of eyesight from one angle (step: 1")
def detect_fov(map_path, bearing):
	edge_pt = [] 
	return edge_pt

# return distance vector: [x_offset, y_offset]. The first parameter of the function is input
def dist_from_gps(vp_lat, vp_lng, t_lat, t_lng):
	dist_vec = [0, 0]
	return dist_vec

def draw_fov(map_path, edge_pt, dist_vec): # draw the result and return if the tp is within fov
	is_inside = True
	return is_inside

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
			'''
			# TODO:
			edge_pt = detect_fov(map_path, raw_data_list[i].vp_list[j].bearing) # detect the fov: edge_pt is the list made of end point of eyesight from one angle (step: 1")
			# return distance vector: [x_offset, y_offset]. The first parameter of the function is input
			dist_vec = dist_from_gps(raw_data_list[i].vp_list[j].lat, raw_data_list[i].vp_list[j].lng, \
										raw_data_list[i].t_lat, raw_data_list[i].t_lng) 
			is_inside = draw_fov(map_path, edge_pt, dist_vec) # draw the result and return if the tp is within fov
			if is_inside:
				num_inside += 1
			'''
		append_file(`i`+ ':' + `num_inside`, RES_DIR + RES_FILE) # write result to file
		# TO REMOVE
		break # this is temporary (for test)
	
	print "done"
