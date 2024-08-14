import serial
import time
import paho.mqtt.client as mqtt
import ssl
import json
import _thread

ser = serial.Serial('/dev/ttyUSB0',9600,timeout=0.1) #port can be different check it from dev/tty
time.sleep(2)
ser.reset_input_buffer()
print('Serial OK')

def on_connect(client, userdata, flags, rc):
    print("Connected to AWS IoT: " + str(rc))
    
def readAndPublishData(txt):
    print(txt)
    ctr = 1
    while (True):
        msg = "Testing" + str(ctr)
        print(msg)
        client.publish("raspi/data", payload=json.dumps({"msg": msg}), qos=0, retain=False)
        ctr = ctr + 1
        
        time.sleep(5)


_thread.start_new_thread(publishData, ("Spin-up new Thread...",))

client.loop_forever()
    
