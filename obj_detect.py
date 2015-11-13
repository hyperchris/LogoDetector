'''
 Create: 	10/15/2015
 Author: 	Xiaochen Liu
 
 Function: 	Given image path and logo name, the "get_result" will return 
 		the x-coordinate of the two boudaries of detected logo and 
 		the image width as well as the threshold and the round #
 
 Usage: 	Put this python file in the same path with the "code" folder. 
 			Then import the python file and call "get_result" function: 
 				func_result = get_result(image_path, show_img)

 What's the return value format: 	
//          * RESULT: empty
//          * RESULT: [left_pos]/[right_pos]/[img_cols]/[thres]

if you want to detect all imgs in a dir, use: $python obj_detect.py img_dir/
'''

import time, os, copy, sys, random,threading
 
OS = 'OSX' # change this to 'Linux' if you're running the code on Linux
THRESHOLD = 1.0 # threshold for detection

RESULT_HEADER = "RESULT: "
ERROR_MSG = "ERROR"

def get_result(image_path, logo_name, show_img):
	print 'logo: ' + logo_name
	cmd = './code/' + OS + '/detect ' + `THRESHOLD` + ' ' + image_path + ' descriptors/' + logo_name + '.dat ' + `show_img`
	ret_val = os.popen(cmd).read().split('\n') # system call
	
	found = False
	for i in range(len(ret_val)):
		print ret_val[i]
		if ret_val[i].startswith(RESULT_HEADER):
			found = True
			return ret_val[i].split(RESULT_HEADER)[-1]
			break
	
	if found == False:
		print 'error!'
		return ERROR_MSG

# Set the directory you want to start from
def dir_walk(rootDir, indent):
	logo_name = rootDir.split('/')[-2]
	out_file = open(logo_name + '_res.txt','w')
	right_ans = 0
	wrong_ans = 0
	for dirName, subdirList, fileList in os.walk(rootDir):
		print indent + dirName
		for fname in fileList: # file 
			print indent + '\t'+ fname
			if '.jpg' in fname or '.png' in fname or '.JPG' in fname or '.PNG' in fname:
				detect_res = get_result(rootDir + fname, logo_name, 0)
	out_file.close()
	print 'ANSWER: Right:' + `right_ans` + ' Wrong:' + `wrong_ans`

if __name__ == '__main__':
	# should go through all imgs here...
	
	if len(sys.argv) != 2:
		sys.exit('ERROR: wrong argc!')
	if not os.path.exists(sys.argv[1]):
		sys.exit('ERROR: ' + sys.argv[1] + ' was not found!')

	dir_walk(sys.argv[1], '') # walk through 
	
	print "done"
