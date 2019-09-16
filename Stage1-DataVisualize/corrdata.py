import csv

with open('weather.csv') as csvfile:
    readCSV = csv.reader(csvfile, delimiter=',')
    month = 1
    day = 1
    time = 0
    first = 1
    counter = 0
    with open('coWeather.csv', mode='w', newline='') as coWeather:
        fieldnames = ['dateTime', 't', 'humidity', 'count']
        writer = csv.DictWriter(coWeather, fieldnames=fieldnames)
        for row in readCSV:
            if first == 1 :
                writer.writerow({'dateTime': row[0], 't': row[1],  'humidity' : row[2] , 'count' : 'counter'})
                first = 0
                continue
            row[0] = str(day) + "/" + str(month) + ' time: ' + str(time)
            
            writer.writerow({'dateTime': row[0], 't': row[1],  'humidity' : row[2], 'count' : counter})
            counter +=1 
            if time == 23 :
                time = 0 
                if month == 2 :
                    if day == 28 :
                        day = 1
                        month += 1
                    else :
                        day +=1
                elif month == 1 or month == 3 or month == 5 or month == 7 or month == 8 or month == 10 or month == 12 :
                    if day == 31 :
                        day = 1
                        month += 1
                    else :
                        day +=1
                else :
                    if day == 30 :
                        day = 1 
                        month += 1
                    else :
                        day +=1
            else :
                time += 1 

    
    
    
    #2-> 28 192.168.0.118 60-8C-4A-5A-FB-4A