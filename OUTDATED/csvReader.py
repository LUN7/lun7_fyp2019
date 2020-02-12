import csv
from os import listdir, getcwd, path
import matplotlib.pyplot as plt


print("Welcome to CSV graph ploter by Lun7") 
while True:
    read_default = input("read the deafult file? (Y/N)")
    if read_default == 'Y' :
        current_dir = getcwd() #get current directory path
        target_dir = current_dir + '/data/' 
        break  
    elif read_default == 'N' :
        while True:
            target_dir = input('Enter your target folder path:')
            if not path.isdir(target_dir):
                print('Errror: Entered path does not exist')
            else:
                break
        break
    

data_list = listdir(target_dir) #get list of data in "./data"
csv_list = []
for data in data_list:
    if ( data[len(data)-3] == 'c' and data[len(data)-2] == 's' and data[len(data)-1] == 'v'):
        csv_list.append(data)

if len(csv_list) == 0 :
    print('Selected folder does not have any CSV file')
else :
    print("CSV list:")
    counter = 1
    for csv_item in csv_list:
        print(str(counter) + ': ' + csv_item)
        counter += 1
csv_list_range = '[1-' + str( len(csv_list)) + ']:'

range_list = list(range(1, len(csv_list)+1) )
print(range_list)
while True:
    selected_csv_num = input('please select a file ' + csv_list_range + '\n')
    if not int(selected_csv_num) in range_list:
        print('Error: Please enter a correct number ')
    else :
        break

selected_csv_path = target_dir + csv_list[int(selected_csv_num)-1]
print(selected_csv_path)

data_cache = [[]]
with open (selected_csv_path, newline='') as csvfile:   
    dict_csv_list = csv.DictReader(csvfile, delimiter=',', quotechar='|' )
    csv_list = csv.reader(csvfile, delimiter=',', quotechar='|')
    
    dict_name = []
    print('the csv contain the following dictionary: ')
    for csv_data in csv_list:
        for j in list(range(len(csv))):
            print(str(j) + ':' + csv_data[j])
            dict_name.append(csv[j])
        break

    print(dict_name)
# while True :
#     x_label_num = input('please select the x-axis data')
#     if x_label_num in list(range(len(dict_name))):
#         break
#     print('incorrect input')

# while True :
#     y_label_num = input('please select the y-axis data')
#     if y_label_num in list(range(len(dict_name))) and y_label_num != x_label_num: 
#         break            
#     print('incorrect input')    


    
