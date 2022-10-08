# import keras
# import keras.layers.TimeDistributed
from keras.models import Sequential
from keras.layers import ZeroPadding2D, Conv2D, BatchNormalization, Dropout, MaxPooling2D, Flatten, Dense, Reshape, Activation, TimeDistributed



model = Sequential()

model.add(TimeDistributed(ZeroPadding2D(2), input_shape=(4,100,100,3)))  # extra padding

model.add(TimeDistributed(Conv2D(50, kernel_size=(5,5), padding='same', activation='relu', strides=2))) # padding=same for more padding
model.add(TimeDistributed(BatchNormalization()))
model.add(TimeDistributed(MaxPooling2D()))                                                              # only one maxpool layer

model.add(TimeDistributed(Conv2D(100, kernel_size=(5,5), padding='same', activation='relu', strides=2)))
model.add(TimeDistributed(BatchNormalization()))
model.add(TimeDistributed(Dropout(0.3)))

model.add(TimeDistributed(Conv2D(100, kernel_size=(3,3), padding='same', activation='relu', strides=2)))
model.add(TimeDistributed(BatchNormalization()))
model.add(TimeDistributed(Dropout(0.3)))

model.add(TimeDistributed(Conv2D(200, kernel_size=(3,3), padding='same', activation='relu', strides=1)))
model.add(TimeDistributed(BatchNormalization()))
model.add(TimeDistributed(Dropout(0.3)))

model.add(Flatten())  # combining all the features

model.add(Dense(600, activation='relu'))
model.add(BatchNormalization())
model.add(Dense(400, activation='relu'))
model.add(BatchNormalization())
model.add(Dropout(0.3))
model.add(Dense(16))
model.add(Reshape((4, 4)))        # reshaping the final output
model.add(Activation('softmax'))  # softmax would be applied row wise

# view rawjigsaw_model.py hosted with ❤ by GitHub