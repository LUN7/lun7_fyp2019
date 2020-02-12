import csv

datetime = []
x = 9
first = 1
with open('datalog.csv') as csv_file:
        csv_reader = csv.reader(csv_file, delimiter=',')
        for row in csv_reader:
            if (first == 1): 
                first = 0
                continue
            datetime.append(row[0])

# with open('eggs.csv', 'w', newline='') as csvfile:
#     spamwriter = csv.writer(csvfile, delimiter=' ',
#                             quotechar='|', quoting=csv.QUOTE_MINIMAL)
#     spamwriter.writerow(['Spam'] * 5 + ['Baked Beans'])
#     spamwriter.writerow(['Spam', 'Lovely Spam', 'Wonderful Spam'])
    
with open('time.csv', 'w', newline='') as w_file:
    w_writer = csv.writer(w_file, delimiter=' ' , 
                          quotechar=' ', quoting=csv.QUOTE_MINIMAL)

    w_writer.writerow(['datetime'])
    for item in datetime:

        print(x)
        print(item[0:13])
        w_writer.writerow( str(item[0:13] + str(x)))

        if ( x == 60):
            x = 1
        else:    
            x+=1

