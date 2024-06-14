# -*- coding:utf-8 -*-

import matplotlib as mpl
import matplotlib.pyplot as plt

#mpl.rcParams["font.sans-serif"] = ["SimHei"]
#mpl.rcParams["axes.unicode_minus"] = False

# some simple data
x = [1,2,3]
y = [51099, 17534, 20642]
x_label = ["benchmark", "version 1", "version 2"]

# create bar
plt.bar(x,y,align="center",color="c",tick_label=x_label,hatch="/")

# set x,y_axis label
#plt.xlabel("箱子编号")
plt.ylabel("ns")

plt.show()
