import matplotlib.pyplot as plt
import numpy as np
import torch

def log_softmax(x):
    v_exp = np.exp(x)
    v_sum = v_exp.sum(-1);
    v_div = v_exp/v_sum
    v_log = np.log(v_div)
    return v_log;
    
def log_softmax_torch(x):
    m = torch.nn.LogSoftmax(dim=0)
    return m(x)

xpoints = np.arange(-20.0, 20.0, 1.0)
x_tensor = torch.tensor(xpoints)
y_tensor = log_softmax_torch(x_tensor)
ypoints = y_tensor.numpy()
#ypoints = log_softmax(xpoints)
print("in: ", xpoints)
print("out: ", ypoints);

plt.plot(xpoints, ypoints)
plt.show()

