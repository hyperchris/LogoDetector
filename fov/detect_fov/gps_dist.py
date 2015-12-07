import utm
import math

# point in x-y plane
class point():
	def __init__(self, x, y):
		self.x = float(x)
		self.y = float(y)

def get_distance(origin_lat, origin_lng, dest_lat, dest_lng):	
	xy = utm.from_latlon(origin_lat, origin_lng) # convert origin from lat-lng to xy
	originXY = point(float(xy[0]), float(xy[1]))
	
	xy = utm.from_latlon(dest_lat, dest_lng) # convert destination from lat-lng to xy
	destinationXY = point(float(xy[0]), float(xy[1]))

	distance = math.sqrt((originXY.x - destinationXY.x) ** 2 + (originXY.y - destinationXY.y) ** 2)
	return [destinationXY.x - originXY.x, destinationXY.y - originXY.y] # return the distance vector