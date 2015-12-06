class Vp_data: # vp data struct
	lat = 0.0
	lng = 0.0
	bearing = 0.0
	def __init__(self, lat_, lng_, bearing_):
		self.lat = lat_
		self.lng = lng_
		self.bearing = bearing_

class Raw_data: # triangulation result with list of vp
	t_lat = 0.0
	t_lng = 0.0
	vp_list = []
	vp_list_len = 0

	def __init__(self, t_lat_, t_lng_):
		self.t_lat = t_lat_
		self.t_lng = t_lng_

	def add(self, vp_data):
		self.vp_list_len += 1
		self.vp_list.append(vp_data)