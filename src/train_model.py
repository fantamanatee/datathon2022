import os
import sys
import copy
import keras
import numpy as np
import pandas as pd
from PIL import Image
import matplotlib.pyplot as plt

# from model import get_model
from utils import *

from keras.models import Sequential
from keras.layers import ZeroPadding2D, Conv2D, BatchNormalization, Dropout, MaxPooling2D, Flatten, Dense, Reshape, Activation, TimeDistributed


def get_model():

    model = keras.models.Sequential()

    model.add(TimeDistributed(ZeroPadding2D(2), input_shape=(4, 64, 64, 3)))

    model.add(TimeDistributed(Conv2D(50, kernel_size=(5,5), padding='same', activation='relu', strides=2)))
    model.add(TimeDistributed(BatchNormalization()))
    model.add(TimeDistributed(MaxPooling2D()))

    model.add(TimeDistributed(Conv2D(100, kernel_size=(5,5), padding='same', activation='relu', strides=2)))
    model.add(TimeDistributed(BatchNormalization()))
    model.add(TimeDistributed(Dropout(0.3)))

    model.add(TimeDistributed(Conv2D(100, kernel_size=(3,3), padding='same', activation='relu', strides=2)))
    model.add(TimeDistributed(BatchNormalization()))
    model.add(TimeDistributed(Dropout(0.3)))

    model.add(TimeDistributed(Conv2D(200, kernel_size=(3,3), padding='same', activation='relu', strides=1)))
    model.add(TimeDistributed(BatchNormalization()))
    model.add(TimeDistributed(Dropout(0.3)))

    model.add(Flatten())
    
    model.add(Dense(600, activation='relu'))
    model.add(BatchNormalization())
    model.add(Dense(400, activation='relu'))
    model.add(BatchNormalization())
    model.add(Dropout(0.3))
    model.add(Dense(16))
    model.add(Reshape((4, 4)))
    model.add(Activation('softmax'))

    return model

base_path = './../'

cuts = 2    
channel = 3
im_size = 128

model = get_model()
adam = keras.optimizers.Adam(lr=.001)
model.compile(loss='sparse_categorical_crossentropy', optimizer=adam)

x_train, y_train = load_data(base_path, 'all_data/train')
x_val,   y_val   = load_data(base_path, 'all_data/valid')   

model.fit(x_train, y_train, batch_size=64, epochs=1, validation_data=(x_val, y_val))

model.save(path = os.getcwd() + '/model1.h5')