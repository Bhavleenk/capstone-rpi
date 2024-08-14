import time
import paho.mqtt.client as mqtt
import ssl
import json
import _thread
import serial

ser = serial.Serial('/dev/ttyUSB0',9600,timeout=0.1) #port can be different check it from dev/tty
time.sleep(3)
ser.reset_input_buffer()
print('Serial OK')

def on_connect(client, userdata, flags, rc):
    print("Connected to AWS IoT: " + str(rc))


client = mqtt.Client()
client.on_connect = on_connect
client.tls_set(ca_certs='./AWS_Certificate/rootCA.pem', certfile='./AWS_Certificate/certificate.pem.crt', keyfile='./AWS_Certificate/private.pem.key',
               tls_version=ssl.PROTOCOL_SSLv23)
client.tls_insecure_set(True)
client.connect("a2f5ot1cictc7c-ats.iot.ap-southeast-2.amazonaws.com", 8883, 60)


def publishData(txt):
    ctr = 1
    while (True):
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').rstrip()
        client.publish("raspi/data", payload=json.dumps({
        "mq2": line,
        
        "time" : int(time.time()) 
        }), qos=0, retain=False)
        ctr = ctr + 1
        time.sleep(5)
        #if ctr>=10:
         #   break


_thread.start_new_thread(publishData, ("Spin-up new Thread...",))

client.loop_forever()
