import os
import threading 
from time import sleep

def stream():
	os.system("cvlc --no-audio v4l2:///dev/video0 --v4l2-chroma MJPG  --sout '#standard{access=http,mux=mpjpeg,dst=:8554/}' -I dummy")

def taskB():
	for i in range(100):
		print(i)
		sleep(0.1)


p1 = threading.Thread(target=stream, args=())
p2 = threading.Thread(target=taskB, args=())

p1.start()
p2.start()

p1.join()
p2.join()
print("AFTER")
