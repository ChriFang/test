import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import numpy as np

# 创建图像和轴
fig, ax = plt.subplots()

# 生成平均分布在0~2*pi之间的100个坐标点
x_data = np.linspace(0, 2 * np.pi, 100)
# 画出初始图
line, = ax.plot(x_data, np.sin(x_data))

# 更新函数（更新y坐标值）
def update(frame):
    line.set_ydata(np.sin(x_data + frame / 10))
    return line,

# 实现动画效果，每50ms更新一次，参数在0~200直接
animation = FuncAnimation(fig, update, frames=range(200), interval=50)

plt.show()