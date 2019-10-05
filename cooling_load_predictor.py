from __future__ import absolute_import, division, print_function, unicode_literals

import pandas as pd
import numpy as np
import seaborn as sns
from matplotlib import pyplot as plt

import tensorflow as tf

from tensorflow import keras
from tensorflow.keras import layers

batch_size = 64
learning_rate = 0.001

def feature_scaling(features):
    max_features, min_features, mean_features = features.max(), features.min(), np.mean(features)
    features = (features-mean_features)/(max_features-min_features)
    return features

def build_model():
    
    model = keras.Sequential([
        layers.Dense(batch_size , activation='relu', input_shape=[len(train_data.keys())]),
        layers.Dense(batch_size , activation='relu'),
        layers.Dense(1)
    ])

    optimizer = tf.keras.optimizers.RMSprop(learning_rate)

    model.compile(loss='mse', optimizer = optimizer, metrics = ['mae', 'mse'])
    return model

#read data
data = pd.read_csv('data/train.csv')

#print(data.isna().sum(axis =0, skipna= True)) #check empty data
data = data.dropna() #eliminate error data
train_data = data.sample(frac=0.8, random_state=0 )
test_data = data.drop(train_data.index) #drop -> delete the numer of row

print(test_data)

train_labels = train_data.pop('total_cooling_load')
test_labels = test_data.pop('total_cooling_load')

train_data = feature_scaling(train_data)
test_data = feature_scaling(test_data)
#review the data
#sns.set()
#plot = sns.pairplot(train_data[["date", "month", "hours", "tempeature", "humidity", "counter", "total_cooling_load"]], diag_kind="kde")
#plt.show()

model = build_model()
print(model.summary())

features = data.iloc[:,0:5]
#features = feature_scaling(features)
#features = features.transpose()
#print(features)

label = data.iloc[:,6]
#label = feature_scaling(label)
#label = label.transpose()


#below is code copy

class PrintDot(keras.callbacks.Callback):
  def on_epoch_end(self, epoch, logs):
    if epoch % 100 == 0: print('')
    print('.', end='')

EPOCHS = 1000

history = model.fit(
  train_data, train_labels,
  epochs=EPOCHS, validation_split = 0.2, verbose=0,
  callbacks=[PrintDot()])

for his in history:
    print(his)