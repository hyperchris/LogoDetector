import numpy as np
import matplotlib.pyplot as plt
import pylab as pl
import matplotlib.patches as mpatches

num_bins = 300

# change following lines for different data
X_LIM = 90
FONT_SIZE = 17
X_LABLE = 'error(m)'
Y_LABLE = '# of subway'
TITLE = 'Distribution of errors (all results)'

DIR = 'data/'
color_list = ['r', 'g', 'b']
err_color_list = ['r--', 'g--', 'b--']

file_list = ['seed_2_eye.txt', 'zoomed_cropped.txt']
legend_list = ['Google', 'Our esult']

for i in range(len(file_list)):
	data = np.loadtxt(DIR + file_list[i])
	print legend_list[i] + '-data size: ' + `len(data)`
	max_err = 0 # find the max error in data
	for j in range(len(data)):
		if data[j] > max_err:
			max_err = data[j]
	print legend_list[i] + '-max error: ' + `max_err`
	counts, bin_edges = np.histogram(data, bins=num_bins, normed=False)
	cdf = np.cumsum(counts)
	plt.plot(bin_edges[1:], cdf, linewidth=2, color=color_list[i]) # plot the cdf
	plt.plot([max_err, X_LIM], [len(data), len(data)], color=color_list[i], linewidth=2) # plot the line to the end (para: [x_start, end], [y_start, end], color)
	plt.plot([max_err, max_err], [0, len(data)], err_color_list[i], linewidth=1) # show the biggest error

# print len(plot_list)
plt.legend(legend_list, loc=4)
ltext = plt.gca().get_legend().get_texts()
for i in range(len(file_list)):
	plt.setp(ltext[i], color=color_list[i])
# plt.legend(plot_list)# make legend
plt.title(TITLE)
plt.xlim([0, X_LIM])
plt.xlabel(X_LABLE, fontsize=FONT_SIZE)
plt.ylabel(Y_LABLE, fontsize=FONT_SIZE)
plt.xticks(fontsize=FONT_SIZE)
plt.yticks(fontsize=FONT_SIZE)

plt.show()