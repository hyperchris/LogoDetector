import sys, os

# sample data line: 000_005_34.011770_-118.282736_82.157411.jpg 1920,0,0
# sample line: '[path] cols,left,right'

def get_data(line):
	return line.split(',')[-2] + ',' + line.split(',')[-1]

if __name__ == '__main__':
	src = sys.argv[1]
	dst = sys.argv[2]

	# read in file
	din = open(src, 'r')
	dout = open(dst, 'w')

	last_line = ''
	# go through the lines, copy to new file
	dline = din.readline()
	while dline != '':
		print 'line: ' + dline
		if last_line != '' and get_data(dline) != get_data(last_line):
			dout.writelines(dline)
		last_line = dline
		dline = din.readline()
		# ignore those that have same 'left' & 'right' as last line's
	# close the file
	din.close()
	dout.close()