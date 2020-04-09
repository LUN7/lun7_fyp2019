import operator

import pandas as pd
import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt

from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error, r2_score
from sklearn.preprocessing import PolynomialFeatures

def feature_scaling(features):
    max_features, min_features, mean_features = features.max(), features.min(), np.mean(features)
    features = (features-mean_features)/(max_features-min_features)
    return features

#read data
data = pd.read_csv('data/train.csv')

#print(data.isna().sum(axis =0, skipna= True)) #check empty data
data = data.dropna(axis=0) #eliminate error data
train_data = data.sample(frac=0.8, random_state=0 )
test_data = data.drop(train_data.index) #drop -> delete the numer of row
train_data_counter = train_data.pop('counter')
test_data_counter = test_data.pop('counter')
data_counter = data.pop('counter')
print(test_data)

#sns.set()
#plot = sns.pairplot(train_data[["date", "month", "hours", "tempeature", "humidity", "counter", "total_cooling_load"]], diag_kind="kde")
#plt.show()

train_labels = train_data.pop('total_cooling_load')
test_labels = test_data.pop('total_cooling_load')
data_labels = data.pop('total_cooling_load')
#train_data = feature_scaling(train_data)
#test_data = feature_scaling(test_data)
#review the data

#features = data.iloc[:,0:2]
#features = feature_scaling(features)
#features = features.transpose()
#print(features)

#label = data.iloc[:,3]
#label = feature_scaling(label)
#label = label.transpose()

polynomial_features= PolynomialFeatures(degree=4)

data_poly = polynomial_features.fit_transform(data)
train_data_poly = polynomial_features.fit_transform(train_data)
test_data_poly = polynomial_features.fit_transform(test_data)

model = LinearRegression()
model.fit(train_data_poly, train_labels)
print(len(model.coef_))
#pred = model.predict(data_poly)
pred = model.predict(test_data_poly)
prediction = pd.DataFrame(pred)

# x= list(range(4000))
# y= list(range(4000))
# plt.title('predicted value against true value') 
# plt.xlabel('true value')
# plt.ylabel('predicted value')
# plt.plot(x,y)

plt.plot
# plt.plot(data_counter, data_labels)
# plt.plot(data_counter, prediction)
plt.plot(test_data_counter, test_labels)
plt.plot(test_data_counter, prediction)

#plt.scatter(test_labels, pred  ction)
plt.show()

rmse = np.sqrt(mean_squared_error(test_labels, pred))
r2 = r2_score(test_labels,pred)
print(rmse)
print(r2)
