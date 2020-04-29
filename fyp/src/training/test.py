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
		if self.name == "delta_all":
			path = "./TrainingData/delta_all.csv"
		elif self.name[0] == 'd':
			path = "./TrainingData/delta.csv"
		else:
			path = "./TrainingData/" + self.name + ".csv"
		reading = pd.read_csv(path)
		dataset = reading.values
		l = dataset.shape[0]

		return dataset[:,:-2],dataset[:,-1:]

	def get_norm(self):
		path = "norm_feature.pkl"
		min_max_scaler = joblib.load(path) 
		X_norm = min_max_scaler.transform(self.X)
		return X_norm
		
	def get_model(self):
		model = load_model("./model_" + self.name + "/no_load_model.h5")
		return model

	def pred(self):
		self.mse, self.mae = self.model.evaluate(self.X_norm, self.Y)
		# self.y_pred = self.model.predict()

# alpha = data("alpha")
# beta = data("beta")
# cita = data("cita")
# delta_36 = data("delta_36")
# delta_72 = data("delta_72")
# delta_128 = data("delta_128")
# delta_3636 = data("delta_3636")
# delta_trilayer = data("delta_trilayer")
# delta_163616 = data("delta_163616")
# print("alpha: ")
# alpha.pred()
# print("beta: ")
# beta.pred()
# print("cita: ")
# cita.pred()
# print("delta_36: ")
# delta_36.pred()
# print("delta_72: ")
# delta_72.pred()
# print("delta_128: ")
# delta_128.pred()
# print("delta_3636: ")
# delta_3636.pred()
# print("delta_trilayer: ")
# delta_trilayer.pred()
# print("delta_163616: ")
# delta_163616.pred()


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

# length = alpha.X_norm.shape[0]
# for i in range(length-2):
	
# 	alpha.pred(i)
# 	beta.pred(i+1)
# 	cita.pred(i+1)
	
# 	prev = beta.X[i+1,8]
# 	print("Last Temp: ", prev)
# 	print("True temp: ", beta.Y[i+1,:])
# 	print("True delta: ", cita.Y[i+1,:])
# 	print("-----------------------------------------")
# 	print("@Alpha")
# 	print("	Alpha predicted temp: ", alpha.y_pred)
# 	print("	Alpha predicted delta: ", alpha.y_pred - prev)
# 	print("@Beta")
# 	print("	Beta predicted temp: ", beta.y_pred)
# 	print("	Beta predicted delta: ", beta.y_pred - prev)
# 	print("@Cita")
# 	print("	Cita predicted temp: ", cita.y_pred + prev)
# 	print("	Cita predicted delta: ", cita.y_pred)
	
# 	input()


