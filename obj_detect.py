'''
 Create: 	10/15/2015
 Modified: 	11/11/2015
 Author: 	Xiaochen Liu

 [How to use] 
 **Modify 'code/config.txt' to set the detection threshold and window**

 ### For single image
	- Include 'obj_detect.py', use the 'get_result' function inside.
	- Three parameters for 'get_result': [img_path], [logo_name] and [show_img]
	- Return value format: 	
		* RESULT: empty
		* RESULT: [left_pos]/[right_pos]/[img_cols]/[thres]

 ### For multiple images
	- In command line, input "$ python obj_detect.py [input_dir_path] [logo_name] [res_dir_path]"
	- All the result images will be stored in [res_dir_path]
'''

import time, os, copy, sys, random, threading
 
RESULT_HEADER = "RESULT: "
ERROR_MSG = "ERROR"

CONFIG_PATH = 'code/config.txt'
RES_DIR = 'res/'

THRESHOLD = 1.20 # threshold for detection

WSIZE_WIDTH = 120 # window size
WSIZE_HEIGHT = 36

BSIZE_WIDTH = 12
BSIZE_HEIGHT = 12

BSTRIDE_WIDTH = 6
BSTRIDE_HEIGHT = 6

CSIZE_WIDTH = 6
CSIZE_HEIGHT = 6

def get_result(image_path, logo_name, store_img=1, res_dir_path=RES_DIR):
	cmd = './code/detect/detect ' + `THRESHOLD` + ' ' + image_path + ' code/classifiers/' + logo_name + '.dat ' + `store_img` + ' ' + res_dir_path + \
	' ' + `WSIZE_WIDTH` + ' ' + `WSIZE_HEIGHT` + ' ' + `BSIZE_WIDTH` + ' ' + `BSIZE_HEIGHT` + \
	' ' + `BSTRIDE_WIDTH` + ' ' + `BSTRIDE_WIDTH` + ' ' + `CSIZE_WIDTH` + ' ' + `CSIZE_HEIGHT`
	print cmd # debug
	ret_val = os.popen(cmd).read().split('\n') # system call
	found = False
	for i in range(len(ret_val)):
		print ret_val[i]
		if ret_val[i].startswith(RESULT_HEADER): # if result starts with RESULT means success
			found = True
			return ret_val[i].split(RESULT_HEADER)[-1] # return result
			break
	if found == False:
		print 'error!'
		return ERROR_MSG # not found, return ERROR

# Go through the dir to process every image
def dir_walk(rootDir, indent, logo_name):
	print 'logo: ' + logo_name
	for dirName, subdirList, fileList in os.walk(rootDir):
		print indent + dirName
		for fname in fileList: # file 
			print indent + '\t'+ fname
			if '.jpg' in fname or '.png' in fname or '.JPG' in fname or '.PNG' in fname:
				detect_res = get_result(rootDir + fname, logo_name, 1, RES_DIR)

if __name__ == '__main__':
	if len(sys.argv) != 4:
		sys.exit('ERROR: wrong argc! Should be: $python obj_detect.py [input_dir] [logo_name] [output_dir]')
	if not os.path.exists(sys.argv[1]):
		sys.exit('ERROR: src path: ' + sys.argv[1] + ' was not found!')

	input_dir_path = sys.argv[1] 
	logo_name = sys.argv[2]
	RES_DIR = sys.argv[3]

	# check if the dir path is end with '/'
	if input_dir_path[-1] != '/':
		input_dir_path += '/'
	if RES_DIR[-1] != '/':
		RES_DIR += '/'

	if not os.path.exists(RES_DIR):
		os.makedirs(RES_DIR)

	# load the config data
	config_file = open(CONFIG_PATH, 'r')
	lines = config_file.readlines()
	for i in range(len(lines)):
		if logo_name in lines[i]: # if find the corresponding logo name
			THRESHOLD = float(lines[i].split(',')[1])
			WSIZE_WIDTH = int(lines[i].split(',')[2])
			WSIZE_HEIGHT = int(lines[i].split(',')[3])
			BSIZE_WIDTH = int(lines[i].split(',')[4])
			BSIZE_HEIGHT = int(lines[i].split(',')[5])
			BSTRIDE_WIDTH = int(lines[i].split(',')[6])
			BSTRIDE_HEIGHT = int(lines[i].split(',')[7])
			CSIZE_WIDTH = int(lines[i].split(',')[8])
			CSIZE_HEIGHT = int(lines[i].split(',')[9])
			dir_walk(input_dir_path, '', logo_name) # do detection
			break
	config_file.close()

	print "done"
