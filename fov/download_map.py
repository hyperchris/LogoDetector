import time, os, sys
from common import *
import urllib2

# sample google map api: http://maps.google.com/maps/api/staticmap?center=34.0200862,-118.2892835&zoom=19&size=900x900&maptype=roadmap

SLEEP_TIME = 1

PREFIX = 'http://maps.google.com/maps/api/staticmap?center='
MIDDLE = '&zoom='
SUFFIX = '&size=900x900&maptype=roadmap'

# download the map image for this vp
def download_map(vp_data, scale, res_dir, tp_num, vp_num): 
	# data struct of vp_data is Vp_data from common
	imgurl = PREFIX + `vp_data.lat` + ',' + `vp_data.lng` + MIDDLE + `scale` + SUFFIX
	response = urllib2.urlopen(imgurl) # download map
	img = response.read()
	map_path = res_dir + 'res_' + `tp_num` + '_vp_' + `vp_num` + '.png'
	print 'store img to: ' + map_path
	imout = open(map_path, 'w')
	imout.write(img)
	imout.close()
	time.sleep(SLEEP_TIME) # sleep for 100 ms

	return map_path