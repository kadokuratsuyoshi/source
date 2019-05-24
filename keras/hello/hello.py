# -*- coding: utf-8 -*-
import numpy as np
from keras.models import Sequential, model_from_json
from keras.layers.core import Dense
from keras.optimizers import RMSprop
from keras.utils.vis_utils import plot_model

def main():
    # modeling
    model = Sequential()
    
    # input layer
    model.add(Dense(2, activation='sigmoid', input_shape=(2,)))
    
    # output layer
    model.add(Dense(1, activation='linear'))
    
    # compile（gradient method：RMSprop、loss function：mean_squared_error、evaluate function：accuracy）
    model.compile(loss='mean_squared_error', optimizer=RMSprop(), metrics=['accuracy'])
    
    # display summary
    model.summary()
    
    # output sumarry to picture
    #plot_model(model, to_file='model.png')
 
if __name__ == '__main__':
    main()
