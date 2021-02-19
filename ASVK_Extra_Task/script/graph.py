import matplotlib.pyplot as plt
import numpy as np 
import sys


if __name__ == "__main__":
	with open(sys.argv[1], "r") as f:
		y = np.array(list(map(np.float32, f.readlines()))) 

	x = np.arange(0, y.shape[0], 1, dtype = np.float32)

	plt.plot(x, y)
	plt.show()
