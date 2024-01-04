import matplotlib.pyplot as plt
import numpy as np

def log_softmax(x):
    print("in: ", x)
    v_sum = np.exp(x).sum(-1);
    v_log = np.log(v_sum)
    v_out = x - np.expand_dims(v_log, 0);
    print("out: ", v_out);
    return v_out;

xpoints = np.arange(-2, 2, 0.1)
ypoints = log_softmax(xpoints)

plt.plot(xpoints, ypoints)
plt.show()
