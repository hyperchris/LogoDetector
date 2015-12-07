import time, os, copy, sys, random, threading
from common import *

AoV = 60 # angle of view

CPP_DIR = 'detect_fov/'
CPP_NAME = 'detect_fov'
RESULT_TRUE = 'TRUE'

# detect the fov: edge_pt is the list made of end point of eyesight from one angle (step: 1")
def detect_fov(map_path, bearing, dist_vec):
	# the bearing may need to be modified to consider the offset
	edge_pt = [] 
	# command: ./program_name [map path] [bearing_angle] [angle_of_view] [dist_x] [dist_y]
	cmd = './' + CPP_DIR + CPP_NAME + ' ' + map_path + ' ' + `bearing` + ' ' + `AoV` + ' ' + `dist_vec[0]` + ' ' + `dist_vec[1]`
	print 'detect_fov command: ' + cmd # debug
	ret_val = os.popen(cmd).read().split('\n') # system call
	for i in range(len(ret_val)):
		print '*\t' + ret_val[i]
		if ret_val[i].startswith(RESULT_TRUE): # if result starts with RESULT means success
			return True
	return False