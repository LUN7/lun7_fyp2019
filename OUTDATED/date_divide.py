import csv
from datetime import datetime

with open ('data/date.csv', newline='') as inputfile:
    with open('date_output.csv', 'w', newline='') as outputfile:
        
        fieldnames = ['date', 'month']
        date_writer = csv.DictWriter(outputfile, fieldnames = fieldnames )
        date_writer.writeheader()

        date_reader = csv.DictReader(inputfile, delimiter=',', quotechar='|' )
        for date in date_reader:
            save_date = datetime.strptime(date['hours'],"%d/%m/%Y")
            date_writer.writerow({ 'date':save_date.strftime('%d'), 'month': save_date.strftime('%m')})


            


