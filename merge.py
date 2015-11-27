'''
 Create: 	10/15/2015
 Modified: 	11/27/2015
 Author: 	Xiaochen Liu

 [How to use]
 ### In your python code:
  - Use **merge_image(src_dir, res_dir)** to do processing. Result will be stored in res_dir

 ### In command line:
  - Input "$ python merge.py [src_dir] [res_dir]"
  - All the result images will be stored in [res_dir]
'''

import time, os, copy, sys, random, threading
 
RES_DIR = 'res/'
SPLITTER = '_h' # used for splitting image name into image_info and pos_info

pos_list = ['20_p5', '0_p5', '-20_p5', '20_p-5', '0_p-5', '-20_p-5']
offset_list = [(1280,0), (640,0), (0,0), (1280,320), (640,320), (0,320)]

RES_WID = 1920
RES_HEI = 960

# Call this function in your program
def merge_image():
	if not os.path.exists(RES_DIR):
		os.makedirs(RES_DIR)
		
	dir_walk(input_dir, '') # walk through 


def get_name(impath, name_type): # ge the name or pos from image path
	ret_val_list = impath.split('/')[-1].split(SPLITTER)
	if name_type == 'name':
		return ret_val_list[0]
	else:
		return ret_val_list[-1].split('.')[0]

def call_merge(impath): # merge the six images into one
	pos = get_name(impath, 'pos')
	if pos in pos_list: # never seen this location before
		index = pos_list.index(pos)
		res_path = RES_DIR + get_name(impath, 'name') + '.jpg' # path of result image
		cmd = './code/merge/merge_support ' + impath + ' ' + \
		`offset_list[index][0]` + ' ' + `offset_list[index][1]` + ' ' + res_path
		ret_val = os.popen(cmd).read()
		# print ret_val

# Go through the dir to process every image
def dir_walk(rootDir, indent):
	print 'processing .....'
	for dirName, subdirList, fileList in os.walk(rootDir):
        # print indent + dirName
		img_num = 0
		for fname in fileList: # file 
			if '.jpg' in fname or '.png' in fname or '.JPG' in fname or '.PNG' in fname:
				img_num += 1
				if 0 == img_num % 100:
					print '..' + `img_num` + '..'
				call_merge(rootDir + fname)

if __name__ == '__main__':
	if len(sys.argv) != 3:
		sys.exit('ERROR: wrong argc!')
	if not os.path.exists(sys.argv[1]):
		sys.exit('ERROR: src path: ' + sys.argv[1] + ' was not found!')

	input_dir = sys.argv[1] 
	RES_DIR = sys.argv[2]

	if not os.path.exists(RES_DIR):
		os.makedirs(RES_DIR)
		
	dir_walk(input_dir, '') # walk through 
	
	print "done"
