'''
 Create: 	10/15/2015
 Modified: 	11/11/2015
 Author: 	Xiaochen Liu

 [How to use]
 ### For single image
	- Include 'obj_detect.py', use the 'get_result' function inside.
	- Three parameters for 'get_result': [img_path], [logo_name] and [show_img]
	- Return value format: 	
		* RESULT: empty
		* RESULT: [left_pos]/[right_pos]/[img_cols]/[thres]

 ### For multiple images
	- In command line, input "$ python obj_detect.py [input_dir_path] [logo_name] [res_dir_path] [threshold=1]"
	- All the result images will be stored in [res_dir_path]
'''

import time, os, copy, sys, random,threading
 
THRESHOLD = 1.0 # threshold for detection

RESULT_HEADER = "RESULT: "
ERROR_MSG = "ERROR"

RES_DIR = 'res/'

WSIZE_WIDTH = 120 # window size
WSIZE_HEIGHT = 36

BSIZE_WIDTH = 12
BSIZE_HEIGHT = 12

BSTRIDE_WIDTH = 6
BSTRIDE_HEIGHT = 6

CSIZE_WIDTH = 6
CSIZE_HEIGHT = 6

def get_result(image_path, logo_name, store_img=1, res_dir_path=RES_DIR):
	cmd = './detect/detect ' + `THRESHOLD` + ' ' + image_path + ' descriptors/' + logo_name + '.dat ' + `store_img` + ' ' + res_dir_path + \
	' ' + `WSIZE_WIDTH` + ' ' + `WSIZE_HEIGHT` + ' ' + `BSIZE_WIDTH` + ' ' + `BSIZE_HEIGHT` + \
	' ' + `BSTRIDE_WIDTH` + ' ' + `BSTRIDE_WIDTH` + ' ' + `CSIZE_WIDTH` + ' ' + `CSIZE_HEIGHT`

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
				detect_res = get_result(rootDir + fname, logo_name)

if __name__ == '__main__':
	if len(sys.argv) != 5 and len(sys.argv) != 4:
		sys.exit('ERROR: wrong argc!')
	if not os.path.exists(sys.argv[1]):
		sys.exit('ERROR: src path: ' + sys.argv[1] + ' was not found!')

	input_dir_path = sys.argv[1] 
	logo_name = sys.argv[2]
	THRESHOLD = sys.argv[3]

	if len(sys.argv) == 5:
		RES_DIR = sys.argv[4]

	if not os.path.exists(RES_DIR):
		os.makedirs(RES_DIR)

	dir_walk(input_dir_path, '', logo_name) # walk through 
	
	print "done"
