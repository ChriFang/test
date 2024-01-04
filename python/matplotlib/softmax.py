import matplotlib.pyplot as plt
import numpy as np

def softmax(x):
    v_exp = np.exp(x)
    v_sum = v_exp.sum(-1)
    v_out = v_exp / np.expand_dims(v_sum, 0);
    return v_out

xpoints = np.arange(-2, 2, 0.1)
ypoints = softmax(xpoints)
print("in: ", xpoints)
print("out: ", ypoints)

plt.plot(xpoints, ypoints)
plt.show()
