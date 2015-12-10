import time, os, sys
from common import *

# for parser: vp file
VP_HEADER_LEFT = '=== play with the ' # the header line of one place in t_file
VP_HEADER_RIGHT = 'th landmark ==='
VP_NUM_HEADER_LEFT = 'get '
VP_NUM_HEADER_RIGHT = ' detected photo'
VP_FILE_HEADER = './data/subway_with_merge/photo/' # skip this line for every vp
# for parser: t file
T_NUM_SPLITTER = '\t' # num \t gps
T_GPS_SPLITTER = ',' # lat,lng

# parse two files. Output Raw_data[] as 'raw_data_list'
def parse(vp_file, t_file):
	raw_data_list = []
	vp_data = open(vp_file,'r') # view point
	t_data = open(t_file,'r') # triagulation results

	t_line = t_data.readline()
	vp_line = vp_data.readline()
	while t_line != '': # scan result of locations
		print ' '
		print 'reading line: ' + t_line.split('\n')[0]
		raw_data = Raw_data(-1, -1)
		t_line_part = t_line.split(T_NUM_SPLITTER)
		if t_line_part[-1] != '\n': # if this location has detection result
			t_lat = float(t_line_part[-1].split(T_GPS_SPLITTER)[0])
			t_lng = float(t_line_part[-1].split(T_GPS_SPLITTER)[-1].split('\n')[0])
			raw_data = Raw_data(t_lat, t_lng) # create a new raw_data object
			print 'new raw_data: ' + `raw_data.t_lat` + ' ' + `raw_data.t_lng`
		t_place = int(t_line_part[0])
		print 'place #: ' + `t_place`
		if not VP_HEADER_LEFT in vp_line:
			sys.exit('vp header not match t_line! ' + vp_line)
		vp_place = int(vp_line.split(VP_HEADER_LEFT)[-1].split(VP_HEADER_RIGHT)[0])
		if vp_place != t_place: # check if vp place matches t place
			sys.exit('vp place doesnt match t place!: ' + `vp_place` + ' ' + `t_place`)
		vp_line = vp_data.readline()
		vp_num = int(vp_line.split(VP_NUM_HEADER_LEFT)[-1].split(VP_NUM_HEADER_RIGHT)[0])
		print 'vp #: ' + `vp_num`
		vp_count = 0
		while vp_count < vp_num:
			vp_line = vp_data.readline() # read the file name line
			vp_file_part = vp_line.split(VP_FILE_HEADER)[-1].split('_') # output format: [pl#, vp#, lat, lng, bearing.jpg]
			lat = float(vp_file_part[2])
			lng = float(vp_file_part[3])
			bearing = float(vp_file_part[4].split('.')[0])
			vp_line = vp_data.readline() # read the detection result line
			if not 'empty' in vp_line:
				# print `vp_count` + ': ' + vp_file_part[1] + ' ' + `lat` + ' ' + `lng` + ' ' + `bearing`
				raw_data.vp_dict[`vp_count`] = Vp_data(lat, lng, bearing) # store the data
				vp_count += 1
		# for i in range(len(raw_data.vp_list)):
			# print `len(raw_data_list)` + ': ' + `raw_data.vp_list[i][0]` + ' ' + `raw_data.vp_list[i][1]` 
		raw_data_list.append(raw_data) # add one more subway..
		print 'vp_list len: ' + `len(raw_data.vp_dict)`
		t_line = t_data.readline()
		while vp_line != '' and not VP_HEADER_LEFT in vp_line: # pass the rest empty results
			vp_line = vp_data.readline()

	return raw_data_list
