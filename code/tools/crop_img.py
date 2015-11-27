#!/usr/bin/python

import os
import PIL
from PIL import Image
import sys 
from random import randint
import time

WIDTH = 180
HEIGHT = 54
PIECE = 1 # how many images each street view generates 
WIDTH_MAX = 250

UPPER = 0.15 # upper 15% will be ignored
LOWER = 0.33 # lower 1/3 will be ignored

def trim_img(img_path, width_in, height_in, piece, dstDir):
    try:
        f = Image.open(img_path)
    except:
        print 'ERROR: cannot open: ' + img_path
        return
    else:
        xsize, ysize_origin = f.size
        ysize = int(ysize_origin * (1.0 - UPPER - LOWER))
    	# print `xsize` + ' ' + `ysize_origin` + ' ' + `ysize` + ' ' + `int(ysize_origin * UPPER)`
        for i in range(piece): # for each piece...
            width = width_in + randint(0, WIDTH_MAX - width_in) # generate a random larger scale between origin window and max size
            height = height_in * width / width_in
            xstart = randint(0, xsize - width)
            ystart = randint(int(ysize_origin * UPPER), int(ysize_origin * UPPER) + ysize - height)
            box = (xstart, ystart, xstart + width, ystart + height)
            # print 'Crop size: ' + `width` + ' ' + `height` + ' -- ' + img_path
            f.crop(box).resize((width_in, height_in), PIL.Image.ANTIALIAS).save(dstDir + '/' + `int(round(time.time() * 10000))` + '.jpg')

# Set the directory you want to start from
def dir_walk(rootDir, indent, dstDir):
    for dirName, subdirList, fileList in os.walk(rootDir):
        print indent + dirName
        img_num = 0
        for fname in fileList: # file 
            if '.jpg' in fname or '.JPG' in fname:
            	img_num += 1
                trim_img(dirName + '/' + fname, WIDTH, HEIGHT, PIECE, dstDir)
	print 'Image number: ' + `img_num`

if __name__ == '__main__':
    if len(sys.argv) == 2:
        print '[rootDir] [dstDir] [width] [height] [piece] [WIDTH_MAX]' 
    elif len(sys.argv) == 7:
        rootDir = sys.argv[1]
        dstDir = sys.argv[2]
        WIDTH = int(sys.argv[3])
        HEIGHT = int(sys.argv[4])
        PIECE = int(sys.argv[5])
        WIDTH_MAX = int(sys.argv[6])
        # create dest dir
        if not os.path.exists(dstDir):
            os.makedirs(dstDir)

        dir_walk(rootDir, '', dstDir) # walk through 
    else:
        print 'ERROR: wrong argc!'
        
    print "done"
