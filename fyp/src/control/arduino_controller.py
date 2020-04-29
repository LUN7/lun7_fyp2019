import serial
import random
import itertools
import time
import threading
import sys
import datetime
import pandas as pd 
import data_processing as d_pro

import os

from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from tensorflow.keras.layers import Dropout

os.environ["CUDA_VISIBLE_DEVICES"] = "-1"  #force using CPU

COM_PORT = "/dev/ttyACM0"
BAUD_RATES = 115200
ser = serial.Serial(COM_PORT, BAUD_RATES)
MODE = 'MIX'
onOffModeCount = 0
onOffmodeMode = 0

# RAND NUMBER is used for set mode
# Change rand number to get a diversify data
RANDA = 3 # A = [0 , ANY]  LOW RAND A increase the chance of having a long duration mode set

RANDB_lower = 2 # increase RAND B to increase the duration of a mode set [1 , ANY]
RANDB_upper = RANDB_lower + 4

RANDC = 5 # lower RANDC to increase chance of having off mode in continuous mode C [1 , ANY] 

RANDD = 0 #lowest rand mode
RANDE = 5 #highest rand mode

def timeInterrupt():
    loadingTrigger.clear()
    print("Current Time: " + str(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')))
    requestDataTrigger.set()
    #set mode every 60s pass
    setModeTrigger.set()
    
    loadingTrigger.set()
    threading.Timer(60, timeInterrupt).start()


def saveData(data):
    df = pd.DataFrame(data).T
    print(data)
    #df.to_csv('/home/lun7/Desktop/lun7_fyp2019/Latest/DATA/TrainingData/DATA_log.csv',mode='a',header=False, index=False)


def requestData():

    while True:
        requestDataTrigger.wait()
        data = [str(datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S'))]
        commArduino("requestData")
        
        print(getResponse())
        for i in range( 14 ):
            data.append(float(getResponse()))
        requestDataTrigger.clear()

        saveData(data)
        

def getMode():

    global MODE 

    if MODE == 'RAND':
        return random.randint(0,6)
    elif MODE == 'MAX':
        return 5
    elif MODE == 'ONOFF':
        global onOffmodeMode
        global onOffModeCount
        if onOffModeCount != 0:
            onOffModeCount -= 1 
        else:
            onOffModeCount = random.randint(RANDB_lower, RANDB_upper)
            if random.randint(0,RANDC) == 0 :
                onOffmodeMode = 6
            else:
                onOffmodeMode = random.randint(RANDD,RANDE)
        return onOffmodeMode    
    elif MODE == 'MIX':
        if (random.randint(0,RANDA) == 0 or onOffModeCount != 0):
            MODE = 'ONOFF'
            mode = getMode()
        else : 
            MODE = 'RAND'
            mode = getMode()
        MODE = 'MIX'
        return mode

def setMode():

    while True:
        #wait until timerInterrupted
        setModeTrigger.wait()
        while requestDataTrigger.is_set() :
            #wait unti requestData finish
            continue
        commArduino("SetMode")
        print(getResponse())
        commArduino(getMode())
        print(getResponse())
        print(getResponse())
        setModeTrigger.clear()

def commArduino(__pass):

    __passBytes = str(__pass).encode()
    ser.write(__passBytes)

def getResponse():

    try:
        while True:
            while ser.in_waiting:          # 若收到序列資料…
                data_raw = ser.readline()  # 讀取一行
                response = data_raw.decode()[0:-2]   # 用預設的UTF-8解碼
                return response

    except KeyboardInterrupt:
        ser.close()    # 清除序列通訊物件
        print('exit')

def animate():
        
    for c in itertools.cycle(['|', '/', '-', '\\']):
        loadingTrigger.wait()
        sys.stdout.write('\r' + c)
        sys.stdout.flush()
        time.sleep(0.1)


def init():

    while (getResponse() != "ready"):
        continue

def genModeForDebu():
    for i in range(30) :
        print(getMode())

def predict(model, x):
    
    delta = model.predict(x)
    return delta

if __name__ == '__main__':
    #System inititiation


    done = False
    loadingAnimation = threading.Thread(target=animate)
    loadingTrigger = threading.Event()
    loadingAnimation.start()
    loadingTrigger.set()

    setModeThread = threading.Thread(target=setMode)
    setModeTrigger = threading.Event()
    requestDataThread = threading.Thread(target=requestData)
    requestDataTrigger = threading.Event()

    try :
        init()
    except:
        COM_PORT = "/dev/ttyACM1"
        init()

    #wait until Arduino ready
    print('waiting 60s')
    time.sleep(60)
    print("System Started up successfully")

    print("Connection established")
    print("Timer Interrupt Started")

    setModeThread.start()
    requestDataThread.start()
    timeInterrupt()

