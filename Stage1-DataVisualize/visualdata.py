import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.dates as mpl_dates

plt.style.use('seaborn')
fig = plt.figure()  # an empty figure with no axes
fig.suptitle('Tempeature trend')  # Add a title so we know which it is

data = pd.read_csv('coWeather.csv').to_numpy()
print(data)
date = []
tempeature = []
humidity = []
for row in data:
    date.append(row[0])
    tempeature.append(row[2])
    humidity.append(row[3])

print(date)
print(tempeature)
print(humidity)

dev_x = date
dev_temp = tempeature
dev_hum = humidity
plt.plot(dev_x, dev_temp, label='tempeature')
plt.plot(dev_x, dev_hum, label='humidity')
#plt.plot_date(dev_x, dev_temp, tz=None, xdate=True, ydate=False)
plt.xlabel('date')
plt.ylabel('')

plt.show()

plt.savefig('fig')
# print(date)
# fig = plt.figure()  # an empty figure with no axes
# fig.suptitle('DateTime')  # Add a title so we know which it is

# fig, ax_lst = plt.subplots(2, 2)  # a figure with a 2x2 grid of Axes

# https://www.kancloud.cn/thinkphp/python-guide/39430
