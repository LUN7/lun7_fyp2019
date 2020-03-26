import pandas as pd 

lst = ['Geeks', 'For', 'Geeks', 'is',  
            'portal', 'for', 'Geeks'] 
  
# Calling DataFrame constructor on list 
df = pd.DataFrame(lst) 
dt = pd.DataFrame(df.T)
print(dt) 