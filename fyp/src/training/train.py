import pandas as  pd
import seaborn as sns
import numpy as np

from sklearn import preprocessing
from sklearn.model_selection import train_test_split
from sklearn.externals import joblib

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.layers import Dropout
from tensorflow.keras import metrics
from tensorflow.keras import callbacks
from tensorflow.keras.backend import clear_session

import matplotlib.pyplot as plt
import sys

from datetime import datetime

clear_session()
#------------read and shuffle data--------------------#
reading = pd.read_csv('./TrainingData/delta_ex.csv')

# reading = reading.drop(['deh','det','eh1','et1'], axis=1 )
print(reading.describe())
reading.sample(frac=1).reset_index(drop=True)
dataset = reading.values

#------------------split X and Y----------------- -----#
X = dataset[:,:25]
print('input data: ',X)

l = X.shape[0]
Y = dataset[:,25:]
# Y = dataset[:,23]
# print('output data: ',Y)

# plt.plot(dataset[:,8:])
# plt.show()

#------------------Normalized X-----------------------#
min_max_scaler = joblib.load("./model/norm_feature_final.pkl")
# joblib.load("./model/norm_feature_ex.pkl")
X_norm = min_max_scaler.fit_transform(X)
print('normalized feature X:', X_norm)
# joblib.dump(min_max_scaler, "./model/norm_feature_final.pkl")

#----------------Split test and verify----------------#
#------------70% train 15% verify 15% test------------#
X_train, X_val_and_test, Y_train, Y_val_and_test = train_test_split(X_norm, Y, test_size=0.3)
X_val, X_test, Y_val, Y_test = train_test_split(X_val_and_test, Y_val_and_test, test_size=0.5)
print('shape of training input and output: ', X_train.shape, Y_train.shape)
print('shape of verify input and output: ',X_val.shape, Y_val.shape)
print('shape of test input and output: ', X_test.shape, Y_test.shape)

#--------------create sequential model---------------#

model = Sequential([
            Dense(36,activation = "relu", input_shape=(25,)),
            Dense(36,activation = "relu", input_shape=(25,)),
            Dense(1)
            ])

#----------------define model algo-------------------#
model.compile(optimizer='sgd',
        loss='mse',
        metrics=['mae'])

#---------------------fit model----------------------#
# model.fit(X_train, Y_train,
#     batch_size=X_train.shape[0], epochs=110000,
#     validation_data=(X_val, Y_val))

# model.save('model/no_load_model_T1.h5')

checkpoint = callbacks.ModelCheckpoint("model/T1.h5", monitor='loss', verbose=1,
    save_best_only=True, mode='auto', period=100)

hist = model.fit(X_train, Y_train[:,3],
    batch_size=X_train.shape[0], epochs=100000,
    validation_data=(X_val, Y_val[:,3]),
    callbacks=[checkpoint])

model.save('model/T1.h5')

###

checkpoint = callbacks.ModelCheckpoint("model/T2.h5", monitor='loss', verbose=1,
    save_best_only=True, mode='auto', period=100)

hist = model.fit(X_train, Y_train[:,5],
    batch_size=X_train.shape[0], epochs=100000,
    validation_data=(X_val, Y_val[:,5]),
    callbacks=[checkpoint] )

model.save('model/T2.h5')

###

checkpoint = callbacks.ModelCheckpoint("model/T3.h5", monitor='loss', verbose=1,
    save_best_only=True, mode='auto', period=100)

hist = model.fit(X_train, Y_train[:,7],
    batch_size=X_train.shape[0], epochs=100000,
    validation_data=(X_val, Y_val[:,7]),
    callbacks=[checkpoint] )

model.save('model/T3.h5')

###
checkpoint = callbacks.ModelCheckpoint("model/T4.h5", monitor='loss', verbose=1,
    save_best_only=True, mode='auto', period=100)

hist = model.fit(X_train, Y_train[:,9],
    batch_size=X_train.shape[0], epochs=100000,
    validation_data=(X_val, Y_val[:,9]),
    callbacks=[checkpoint] )

model.save('model/T4.h5')

###


# model.save_weights('./weight')


# model.save('model/no_load_model.h5') 

# plt.plot(dataset[:,8:])
# plt.show()



