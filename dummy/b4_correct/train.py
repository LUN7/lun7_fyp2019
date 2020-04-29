import pandas as  pd
import seaborn as sns

from sklearn import preprocessing
from sklearn.model_selection import train_test_split
from sklearn.externals import joblib

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras import metrics
from tensorflow.keras.layers import Dropout
from tensorflow.keras.backend import clear_session

import matplotlib.pyplot as plt
import sys

clear_session()
#------------read and shuffle data--------------------#
reading = pd.read_csv('./TrainingData/beta.csv')

# reading = reading.drop(['voltage_t','voltage_t1'], axis=1 )
print(reading.describe())
reading.sample(frac=1).reset_index(drop=True)
dataset = reading.values

#------------------split X and Y----------------- -----#
X = dataset[:,:10]
print('input data: ',X)

Y = dataset[:,11:]
print('output data: ',Y)

# plt.plot(dataset[:,8:])
# plt.show()


#------------------Normalized X-----------------------#
min_max_scaler = preprocessing.MinMaxScaler()
X_norm = min_max_scaler.fit_transform(X)
joblib.dump(min_max_scaler, 'onelayer36/norm_feature.pkl') 
print('normalized feature X:', X_norm)


#----------------Split test and verify----------------#
#------------70% train 15% verify 15% test------------#
X_train, X_val_and_test, Y_train, Y_val_and_test = train_test_split(X_norm, Y, test_size=0.3)
X_val, X_test, Y_val, Y_test = train_test_split(X_val_and_test, Y_val_and_test, test_size=0.5)
print('shape of training input and output: ', X_train.shape, Y_train.shape)
print('shape of verify input and output: ',X_val.shape, Y_val.shape)
print('shape of test input and output: ', X_test.shape, Y_test.shape)


#--------------create sequential model---------------#
model = Sequential([
            Dense(36, activation='relu', input_shape=(X_train.shape[1],)),
            Dense(1),
            ])


#----------------define model algo-------------------#
model.compile(optimizer='sgd',
        loss='mse',
        metrics=['mae'])


#---------------------fit model----------------------#
hist = model.fit(X_train, Y_train,
    batch_size=X_train.shape[0], epochs=70000,
    validation_data=(X_val, Y_val))

model.evaluate(X_test, Y_test)

for x,y in zip(X_test[:100],Y_test[:100]):
	
	x = x.reshape((1,10))
	y = y.reshape((1,1))
	loss, mae = model.evaluate(x,y)
	print("True val: ",y )
	print("Predicted:",model.predict(x))


#model.save_weights('./weight')

plt.plot(hist.history['loss'])
plt.plot(hist.history['val_loss'])
plt.plot(hist.history['mae'])
plt.plot(hist.history['val_mae'])
plt.xlabel('Epoch')
plt.legend(['Train loss', 'Validate loss', 'Train mae', 'Validate mae'], loc='lower right')
plt.show()


model.save('onelayer36/no_load_model.h5') 

# plt.plot(dataset[:,8:])
# plt.show()



