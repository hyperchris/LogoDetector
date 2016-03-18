import numpy as np

import matplotlib.pyplot as plt

data = np.loadtxt('abs_diff.txt')

sorted_data = np.sort(data)

yvals=np.arange(len(sorted_data))/float(len(sorted_data))

plt.plot(sorted_data,yvals,lw=2)

plt.show()