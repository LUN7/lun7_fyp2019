import pandas as  pd
import numpy as np
import seaborn as sns

from sklearn import preprocessing
from sklearn.model_selection import train_test_split
from sklearn.externals import joblib

from tensorflow.keras.models import Sequential
from tensorflow.keras.models import load_model
from tensorflow.keras.layers import Dense
from tensorflow.keras import metrics
from tensorflow.keras.layers import Dropout

import matplotlib.pyplot as plt
import sys

# ./TrainingData/beta.csv
class data:
	def __init__(self, name):
		self.name = name
		self.X, self.Y = self.get_data()
		self.X_norm = self.get_norm()
		self.model = self.get_model()

	def get_data(self):
		path = "./TrainingData/" + self.name + ".csv"
		reading = pd.read_csv(path)
		dataset = reading.values
		return dataset[:,:10],dataset[:,11:]

	def get_norm(self):
		path = "./onelayer36_" + self.name + "/norm_feature.pkl"
		min_max_scaler = joblib.load(path) 
		X_norm = min_max_scaler.transform(self.X)
		return X_norm
		
	def get_model(self):
		model = load_model("./onelayer36_" + self.name + "/no_load_model.h5")
		return model

	def pred(self, i):
		x = self.X_norm[i,:].reshape(1,10)
		y = self.Y[i,:].reshape(1,1)
		self.mse, self.mae = self.model.evaluate(x, y)
		self.y_pred = self.model.predict(x)

alpha = data("alpha")
beta = data("beta")
cita = data("cita")

# #----------------------read data-----------------------#
# reading = pd.read_csv('./TrainingData/beta.csv')
# # reading = reading.drop(['voltage_t','voltage_t1'], axis=1 )
# print(reading.describe())
# dataset = reading.values

# #------------------split X and Y----------------- -----#
# X = dataset[:100,:10]
# print('input data: ',X)

# Y = dataset[:100,11:]
# print('output data: ',Y)

# #for i in range(10):
# #	sns.set()
# #	sns.distplot(X[:,i].flatten())
# #	plt.show()
# # plt.plot(dataset[:,8:])
# # plt.show()

# min_max_scaler = joblib.load('onelayer36/norm_feature.pkl') 
# X_norm = min_max_scaler.transform(X)

# model = load_model('onelayer36/no_load_model.h5')
length = alpha.X_norm.shape[0]
for i in range(length-2):
	
	alpha.pred(i)
	beta.pred(i+1)
	cita.pred(i+1)
	
	prev = beta.X[i+1,8]
	print("Last Temp: ", prev)
	print("True temp: ", beta.Y[i+1,:])
	print("True delta: ", cita.Y[i+1,:])
	print("-----------------------------------------")
	print("@Alpha")
	print("	Alpha predicted temp: ", alpha.y_pred)
	print("	Alpha predicted delta: ", alpha.y_pred - prev)
	print("@Beta")
	print("	Beta predicted temp: ", beta.y_pred)
	print("	Beta predicted delta: ", beta.y_pred - prev)
	print("@Cita")
	print("	Cita predicted temp: ", cita.y_pred + prev)
	print("	Cita predicted delta: ", cita.y_pred)
	
	input()
