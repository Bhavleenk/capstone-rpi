import serial
import time
import pandas as pd
import numpy as np
df = pd.DataFrame({'col1':['0']})
ser = serial.Serial('/dev/ttyUSB0',9600,timeout=0.1) #port can be different check it from dev/tty
time.sleep(3)
ser.reset_input_buffer()
print('Serial OK')
i=50
while i:
    time.sleep(0.1)
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        #insert this line into dataframe 
        new_row = {'col1':line}
        df.loc[len(df)] = new_row
        
    i-=1
      
print("Serial comm closed")
ser.close()
