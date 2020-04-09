import  matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('data/train.csv')
plt.scatter(data['total_cooling_load'], data['mode'])
plt.show()
