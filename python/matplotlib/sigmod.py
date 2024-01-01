import matplotlib.pyplot as plt
import numpy as np

def sigmod(x):
    return 1 / (1 + np.exp(-x))

xpoints = np.arange(-20, 20, 1)
ypoints = sigmod(xpoints)

plt.plot(xpoints, ypoints)
plt.show()