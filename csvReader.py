import csv
from os import listdir, getcwd, path


print("Welcome to CSV reader by Lun7") 
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
                print('Entered path does not exist')
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
    selected_csv_num = input('please select a file ' + csv_list_range )
    if not int(selected_csv_num) in range_list:
        print('Please enter a correct number ')
    else :
        break

selected_csv_path = target_dir + csv_list[int(selected_csv_num)-1]
print(selected_csv_path)

data_cache = [[]]
with open (selected_csv_path, newline='') as csvfile:
    csv_reader = csv.reader(csvfile, delimiter=',', quotechar='|' )
    for data in csv_reader:
        print(data)
        




