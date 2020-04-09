import numpy as np
import os
import sys

def load_csv(csv_file_name):
    print("loading csv @", csv_file_name)

    buffer = np.loadtxt(csv_file_name, dtype=np.str, delimiter=",")
    print("csv loaded succesfully")

    return buffer
    

def extract_data(np_iterable, *args):
    '''
    Parameters:
    np_iterable: numpy iterable, source data for extraction
    *args : no of column to extract, e.g. : 1, 2, 3, 6,7
    
    extracted data must be in float type
    '''
    print("extracting data")

    row_size = np_iterable.shape[0]
    col_size = len(args)
    buffer = np.zeros((row_size,col_size), dtype = float )

    i = 0
    for arg in args:

        np.copyto(buffer[:,i], np_iterable[:,arg].astype(float))
        i+=1

    return buffer

def get_file_list(folder_path, file_type):

    print("getting file list in folder: ", folder_path)
    file_list = []

    for i in os.listdir(folder_path):
        if i.endswith(file_type):
            file_list.append(i)

    if len(file_list) == 0 :
        print('No file founds')
    else:
        print("File list:")
        i = 0
        for file_name in file_list:
            print(i, ":", file_name)
            i+=1
    return file_list

def data_delta(input):
    '''
    description: calculate and return table with delta T and h
    data input sequence : 
        outdoor_h, outdoor_t, 
        exchanged_h, exchanged_t, indoor_h,
        indoor_t, V, Amps
    data output sequenec :
        outdoor_h, outdoor_t, 
        exchanged_h, exchanged_t, 
        delta indoor_h (t,t+1), 
        delta indoor_t (t,t+1),
        V
    '''
    length, width = input.shape
    no_of_output_row = length - 1
    no_of_output_col = width
    output = np.zeros((no_of_output_row, no_of_output_col), dtype=float)

    for i in range(width):
        if ( i != 4 and i != 5 ):
            output[:,i] = input[:-1,i]

    for i in range(no_of_output_row):
        j = i+1
        output[i,4] = input[j,4] - input[i,4]
        output[i,5] = input[j,5] - input[i,5]

    return output

def label_data(input):
    '''
    delta t,t+1 + data of t+1 => delta t+1, t+2
    '''
    length, width = input.shape
    no_of_output_row = length -1
    no_of_output_col = width + 3
    output = np.zeros((no_of_output_row, no_of_output_col), dtype=float)

    for i in range(3):
        output[:,i] = input[:-1,i+4]
        # output [:]                   input [0:length-1]
        # row x |0|1|2|3|4|5|6|x|x|x|  row x|0|1|2|3|4|5|6|x|x|x|
        #        ↑ ↑ ↑             move forward            | | |
        #        | | |_____________________________________| | |
        #        | |_________________________________________| |
        #        |_____________________________________________| 

    for row in range(no_of_output_row):
        for col in range(4):
            output[row,col+3] = input[row+1,col]
        output[row,7] = input[row+1,6]
        output[row,8] = input[row+1,4]
        output[row,9] = input[row+1,5]

        # output [:] 
        # row x    |0|1|2|3|4|5|6|x|x|x|
        #                 ↑ ↑ ↑ ↑ ↑ ↑ ↑
        #                / / / /  | | |
        #               / / / /  _| | |
        #              / / / / _|___| |
        #             / / / / / |_____|
        # input [1:] / / / / / /|
        # row x+1  |0|1|2|3|4|5|6|x|x|x|
    return output


if __name__ == "__main__":
    folder_path = "./data"
    # folder_path = input("enter folder path:")
    if './' in folder_path:
        folder_path = os.getcwd() + folder_path[1:]
    csv_file_list = get_file_list(folder_path, ".csv")

    itr=0
    training_data = np.zeros((1,10), dtype="float")
    for csv_file_name in csv_file_list:
        csv_file_name = folder_path + "/" + csv_file_name
        loaded_data = load_csv(csv_file_name)
        extracted_data = extract_data(loaded_data, 1, 2, 3, 4, 5, 6, 13)
        delta_data = data_delta(extracted_data)
        labeled_data = label_data(delta_data)
        training_data = np.concatenate((training_data, labeled_data), axis=0)
        itr+=1
    
    training_data = np.delete(training_data, 0, axis=0)
    print(training_data.shape)
    