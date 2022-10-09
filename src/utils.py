from cProfile import label
import numpy as np
import copy
import numpy as np
import pandas as pd
from PIL import Image
import matplotlib.pyplot as plt
from itertools import permutations
import os
import shutil
import csv


#### starter code

def get_pieces(img, rows, cols, row_cut_size, col_cut_size):
    pieces = []
    for r in range(0, rows, row_cut_size):
        for c in range(0, cols, col_cut_size):
            pieces.append(img[r:r+row_cut_size, c:c+col_cut_size, :])
    return pieces

# Splits an image into uniformly sized puzzle pieces
def get_uniform_rectangular_split(img, puzzle_dim_x, puzzle_dim_y):
    rows = img.shape[0]
    cols = img.shape[1]
    if rows % puzzle_dim_y != 0 or cols % puzzle_dim_x != 0:
        print('Please ensure image dimensions are divisible by desired puzzle dimensions.')
    row_cut_size = rows // puzzle_dim_y
    col_cut_size = cols // puzzle_dim_x

    pieces = get_pieces(img, rows, cols, row_cut_size, col_cut_size)

    return pieces


def swap_piece(im, pos, true_pos, cuts=2, dim=200):

    """
        swap 2 pieces and returns image
    """

    im = im

    cut_len = dim // cuts

    x = pos // cuts
    y = pos % cuts

    x_true = true_pos // cuts
    y_true = true_pos % cuts

    piece = copy.copy(im[x * cut_len:(x + 1) * cut_len, y * cut_len:(y + 1) * cut_len, ])
    swap_piece = copy.copy(im[x_true * cut_len:(x_true + 1) * cut_len, y_true * cut_len:(y_true + 1) * cut_len, ])

    im[x * cut_len:(x + 1) * cut_len, y * cut_len:(y + 1) * cut_len, ] = swap_piece
    im[x_true * cut_len:(x_true + 1) * cut_len, y_true * cut_len:(y_true + 1) * cut_len, ] = piece

    return im


def rearrange(im, label, cuts=2, dim=128, channel=3):

    """
        rearranges the image according to the label
    """

    cut_len = dim // cuts

    new_im = np.zeros((dim, dim, channel))
    for i in range(cuts):

        hor_cut = im[i * cut_len:(i + 1) * cut_len]

        for j in range(cuts):
            piece = hor_cut[:, j * cut_len:(j + 1) * cut_len]

            pos = label[i * cuts + j]
            x = pos // cuts
            y = pos % cuts

            new_im[x * cut_len:(x + 1) * cut_len, y * cut_len:(y + 1) * cut_len] = piece

    plt.imshow(new_im)
    plt.show()


def extract_piece(a, size=128, cuts=2):

    """
       extracts each piece of the puzzle and returns
    """

    cut_len = size // cuts

    if cuts == 3:
        a = np.array([a[:, 0:cut_len, :], a[:, cut_len:cut_len * 2, :], a[:, cut_len * 2:cut_len * 3, :]])
        a = np.concatenate(
            (a[:, 0:cut_len, :, :], a[:, cut_len:cut_len * 2, :, :], a[:, cut_len * 2:cut_len * 3, :, :]))
    if cuts == 2:
        a = np.array([a[:, 0:cut_len, :], a[:, cut_len:, :]], dtype=object)
        a = np.concatenate((a[:, 0:cut_len, :, :], a[:, cut_len:, :, :]))

    return a

# base_path: ./../
# path_to_data: ./../train.csv
# path_to_image: ./../path/image_name 
def load_data(base_path, path):

    """
        loads and returns data
    """
    data = pd.read_csv(base_path + '{}.csv'.format(path))
    path = base_path + path + '/'

    x = []
    y = []
    total = len(data)
    for i in range(total):
        if (i%1000 == 0):
            print(f"entry {i}")
        im = Image.open(path + data.iloc[i]['image'])
        im = np.array(im).astype('float16')
        im = im / 255 - 0.5

        if path.split('/')[-2] == 'test': # ???????
            x.append(im)
        else:
            x.append(extract_piece(im, size=128))

        label = data.iloc[i]['label']
        label = [int(i) for i in label.split()]
        y.append(label)

    return (np.array(x), np.expand_dims(np.array(y), axis=-1))



def split_data(base_path):
    train_path = base_path + '/train'
    valid_path = base_path + '/valid'

    combs = [''.join(str(x) for x in comb) for comb in list(permutations(range(0, 4)))]
    for c in combs: # iterate through permutations
        data = os.listdir(f"{base_path}/{c}")
        for i in range(len(data)): # iterate through each image in shuffled (os.listdir())
            if i < 1658: # for the first 80% images, add to train, named with permutation
                # print(data[i])
                shutil.copy(src = f"{base_path}/{c}/{data[i]}", dst = f"{train_path}/{c}_{data[i]}")
            else:        # for the last 20% images, add to valid, named with permutation 
                # print(data[i])
                shutil.copy(src = f"{base_path}/{c}/{data[i]}", dst = f"{valid_path}/{c}_{data[i]}")

#split_data(base_path = 'C:\\Users\\sophi\\OneDrive\\Desktop\\TAMU\\TAMU Datathon\\2022 Datathon\\Datathon.env\\datathon2022\\all_data')
# Desktop\TAMU\TAMU Datathon\2022 Datathon\Datathon.env\datathon2022

def data_to_csv(base_path, csv_train_name, csv_valid_name):
    train_path = base_path + '/train'
    valid_path = base_path + '/valid'

    train_full_names = os.listdir(train_path)
    valid_full_names = os.listdir(valid_path)

    # train_solutions = [(i.split("_")[0]) for i in train_full_names]
    # valid_solutions = [(i.split("_")[0]) for i in valid_full_names]
    # for label in train_solutions:
    #     new_label = ""
    #     for c in label[:-1]:
    #         new_label += c + ' '
    #     new_label += label[-1]
    #     label = new_label

    train_solutions = []
    valid_solutions = []
    for i in range(len(train_full_names)):
        label = train_full_names[i][0] + ' ' + train_full_names[i][1] + ' ' + train_full_names[i][2] + ' ' + train_full_names[i][3]
        train_solutions.append(label)
    for i in range(len(valid_full_names)):
        label = valid_full_names[i][0] + ' ' + valid_full_names[i][1] + ' ' + valid_full_names[i][2] + ' ' + valid_full_names[i][3]
        valid_solutions.append(label)
            
    
    print(train_solutions)

    # train_names = [(i.split("_")[1]) for i in train_full_names]
    # valid_names = [(i.split("_")[1]) for i in valid_full_names]

    train_df = pd.DataFrame({"image" : train_full_names, "label" : train_solutions})
    valid_df = pd.DataFrame({"image" : valid_full_names, "label" : valid_solutions})

    train_df.to_csv(csv_train_name + '.csv')
    valid_df.to_csv(csv_valid_name + '.csv')


def foo():
    print(1)
# data_to_csv('C:\\Users\\suaar\\OneDrive\\Documents\\datathon2022\\all_data', 'train', 'valid')