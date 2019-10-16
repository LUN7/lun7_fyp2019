import  matplotlib.pyplot as py  
import pandas as pd

mode = []

data = pd.read_csv('data/oonoff.csv')
for i, j in data.iterrows():
    if j[7] == 0:
        if j[8] == 0:
            mode.append('1')
        elif j[8] == 1:
            mode.append('2')
        else :
            mode.append('3')
    elif j[7] == 1:
        if j[8] == 0:
            mode.append('4')
        elif j[8] == 1:
            mode.append('5')
        else :
            mode.append('6')
    else :
        if j[8] == 0:
            mode.append('7')
        elif j[8] == 1:
            mode.append('8')
        else :
            mode.append('9')

data['mode'] = mode
print(data)
data.to_csv('Result.csv')