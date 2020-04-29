import pandas as  pd
import numpy as np
import seaborn as sns

from sklearn import preprocessing
from sklearn.model_selection import train_test_split
from sklearn.externals import joblib

from tensorflow.keras.models import Sequential
from tensorflow.keras.models import load_model
from tensorflow.keras.models import Model
from tensorflow.keras.layers import Dense
from tensorflow.keras.layers import Input
from tensorflow.keras import metrics
from tensorflow.keras.layers import Dropout
from tensorflow.keras.utils import plot_model

import matplotlib.pyplot as plt
import sys

# ./TrainingData/beta.csv
class noload:
    def __init__(self):
        self.min_max_scaler = joblib.load("./model/norm_feature_final.pkl")
        self.model_H1 = self.get_model("H1")
        self.model_H1._name = "H1"
        self.model_T1 = self.get_model("T1")
        self.model_T1._name = "T1"
        self.model_H2 = self.get_model("H2")
        self.model_H2._name = "H2"
        self.model_T2 = self.get_model("T2")
        self.model_T2._name = "T2"
        self.model_H3 = self.get_model("H3")
        self.model_H3._name = "H3"
        self.model_T3 = self.get_model("T3")
        self.model_T3._name = "T3"
        self.model_H4 = self.get_model("H4")
        self.model_H4._name = "H4"
        self.model_T4 = self.get_model("T4")
        self.model_T4._name = "T4"
        self.model_EH = self.get_model("EH")
        self.model_EH._name = "EH"
        self.model_ET = self.get_model("ET")
        self.model_ET._name = "ET"
        inpX = Input(shape=self.model_T1.input_shape[1:])
        self.model_indoor = Model(inputs=[inpX], outputs=[self.model_EH(inpX), self.model_ET(inpX),
                                                          self.model_H1(inpX), self.model_T1(inpX),
                                                          self.model_H2(inpX), self.model_T2(inpX),
                                                          self.model_H3(inpX), self.model_T3(inpX),
                                                          self.model_H4(inpX), self.model_T4(inpX),
                                                          ])
        plot_model(self.model_indoor, to_file='model.png')

        # self.model_indoor.summary()


    def get_norm(self, X):
        X_norm = self.min_max_scaler.transform(X)
        return X_norm

    def get_model(self, name):
        model = load_model("./model/no_load_model_" + name +".h5")
        plot_model(model, to_file= name+'.png')
        return model

    def pred(self, X):
        X = self.get_norm(X.reshape(1,25)) 
        pred = np.asarray(self.model_indoor.predict([X])).reshape(1,10)
        # pred = self.model_H1.predict(X)
        # pred = np.concatenate((pred, self.model_T1.predict(X)))
        # pred = np.concatenate((pred, self.model_H2.predict(X)))
        # pred = np.concatenate((pred, self.model_T2.predict(X)))
        # pred = np.concatenate((pred, self.model_H3.predict(X)))
        # pred = np.concatenate((pred, self.model_T3.predict(X)))
        # pred = np.concatenate((pred, self.model_H4.predict(X)))
        # pred = np.concatenate((pred, self.model_T4.predict(X)))
        # pred = np.concatenate((pred, self.model_EH.predict(X2)))
        # pred = np.concatenate((pred, self.model_ET.predict(X1)))
        return pred
        # self.y_pred = self.model.predict()

    def eval(self, X, Y):
        X1 = self.get_norm(X) 
        X = X1[:,3:]
        self.model_indoor.compile(optimizer='sgd',
            loss='mse',
            metrics=['mae','mae','mae','mae','mae','mae','mae','mae','mae','mae'])
        
no_load_model = noload()

# reading = pd.read_csv('./TrainingData/delta_ex.csv')
# print(reading.describe())
# dataset = reading.values
# X = dataset[:,:25]
# print('input data: ',X)
# l = X.shape[0]
# Y = dataset[:,25:]

# print("EH:")
# no_load_model.model_EH.evaluate(no_load_model.get_norm(X), Y[:,0].reshape(l,1))
# print("ET:")
# no_load_model.model_ET.evaluate(no_load_model.get_norm(X), Y[:,1].reshape(l,1))
# print("H1:")
# no_load_model.model_H1.evaluate(no_load_model.get_norm(X), Y[:,2].reshape(l,1))
# print("T1:")
# no_load_model.model_T1.evaluate(no_load_model.get_norm(X), Y[:,3].reshape(l,1))
# print("H2:")
# no_load_model.model_H2.evaluate(no_load_model.get_norm(X), Y[:,4].reshape(l,1))
# print("T2:")
# no_load_model.model_T2.evaluate(no_load_model.get_norm(X), Y[:,5].reshape(l,1))
# print("H3:")
# no_load_model.model_H3.evaluate(no_load_model.get_norm(X), Y[:,6].reshape(l,1))
# print("T3:")
# no_load_model.model_T3.evaluate(no_load_model.get_norm(X), Y[:,7].reshape(l,1))
# print("H4:")
# no_load_model.model_H4.evaluate(no_load_model.get_norm(X), Y[:,8].reshape(l,1))
# print("T4:")
# no_load_model.model_T4.evaluate(no_load_model.get_norm(X), Y[:,9].reshape(l,1))

i = [6,6,6,6,6,6,6,2,2,2,2,2,2,2,2,2,2,2,10,6,0,2,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6]

inpu=np.array([[0.5,-1.90,0.40,-4.7,-0.9,-3.7,-1.5,-3.3,-1.4,-3.5,-1.3,6,73.3,25.3,78.4,25.8,56.5,26,60.4,25.2,57.9,25.3,56.3,25.4,0]])
for v in i:
    pred = no_load_model.pred(inpu)
    inpu[:,0] = pred[:,1] - inpu[:,13]
    inpu[:,1:11] = pred - inpu[:,14:24]
    inpu[:,11] = inpu[:,24]
    inpu[:,14:24] = pred
    inpu[:,24] = v
    print(pred)
 


