import operator

import pandas as pd
import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt

from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import PolynomialFeatures

batch_size = 64
learning_rate = 0.001


def feature_scaling(features):
    max_features, min_features, mean_features = features.max(), features.min(), np.mean(features)
    features = (features-mean_features)/(max_features-min_features)
    return features

#read data
data = pd.read_csv('data/train.csv')

#print(data.isna().sum(axis =0, skipna= True)) #check empty data
data = data.dropna() #eliminate error data
train_data = data.sample(frac=0.8, random_state=0 )
test_data = data.drop(train_data.index) #drop -> delete the numer of row

print(test_data)

#sns.set()
#plot = sns.pairplot(train_data[["date", "month", "hours", "tempeature", "humidity", "counter", "total_cooling_load"]], diag_kind="kde")
#plt.show()

train_labels = train_data.pop('total_cooling_load')
test_labels = test_data.pop('total_cooling_load')

train_data = feature_scaling(train_data)
test_data = feature_scaling(test_data)
#review the data

#features = data.iloc[:,0:2]
#features = feature_scaling(features)
#features = features.transpose()
#print(features)

#label = data.iloc[:,3]
#label = feature_scaling(label)
#label = label.transpose()

polynomial_features= PolynomialFeatures(degree=10)
train_data_poly = polynomial_features.fit_transform(train_data)
test_data_poly = polynomial_features.fit_transform(test_data)

model = LinearRegression()
model.fit(train_data_poly, train_labels)
pred = model.predict(test_data_poly)
prediction = pd.DataFrame(pred)

x= list(range(4000))
y= list(range(4000))
plt.title('predicted value against true value')
plt.xlabel('true value')
plt.ylabel('predicted value')
plt.plot(x,y)
plt.scatter(test_labels, prediction)
plt.show()

rmse = np.sqrt(mean_squared_error(test_labels, pred))
r2 = r2_score(test_labels,pred)
print(rmse)
print(r2)
