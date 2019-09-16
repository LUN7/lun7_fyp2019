import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from tkinter import *

sns.set(style="darkgrid")

data = pd.read_csv('coWeather.csv')

y = sns.lineplot(x="counter", y="tempeature", data=data)
plt.show(y)

class Application(Frame):
    def __init__(self, master=None):
    Frame.__init__(self, master)