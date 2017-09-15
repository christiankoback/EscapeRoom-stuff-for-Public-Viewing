import bluetooth	      	#handle bluetooth communication
from threading import Thread	#handle threading
from Queue import *				#handle Queue functionality
import time

#setup connection for all devices, update these
bd_addr0 = "98:D3:31:FC:59:F0" #arduino 1 - nfc
bd_addr1 = "98:D3:31:FC:6C:97"#arduino 2 - main cube
bd_addr2 = "98:D3:31:FC:5A:09"  #arduino 3 - magnet
bd_addr3 = "98:D3:31:FD:68:77" #arduino 4 - laser

#setup socket per device
port = 1

nfc_queue = Queue()
mainCube_queue = Queue()
magnet_queue = Queue()
laser_queue = Queue()
raspPi_queue = Queue()



def nfc_Functions():
	sock0 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock0.connect((bd_addr0,port))
	sock0.setblocking(0)
	while 1:
            try:
                #handle reading from arduino
                data = sock0.recv(4)#garbage in the first char, ignore
                data2 = data[1:] #useful data, add to queue for Pi		
                if data2:
                    #send data to queue for raspPi
                    #print("nfc arduino:")
                    #print(data2)
                    raspPi_queue.put(data2,False)
	    except bluetooth.btcommon.BluetoothError:
                pass
	    #handle sending to arduino
            dataToSend = nfc_queue.get(False)
            nfc_queue.task_done()
            if dataToSend :
                sock0.send(dataToSend)
            #sock0.send("100")
            #time.sleep(1);
            #sock0.send("101")
            #time.sleep(1)
def mainCube_Functions():
	sock1 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock1.connect((bd_addr1,port))
	sock1.setblocking(0)
	while 1:
            try:
		#handle reading from arduino
		data = sock1.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
                    #print("main cube arduino:")
                    #print(data2)
		    raspPi_queue.put(data2,False)
	    except bluetooth.btcommon.BluetoothError:
                pass
            #handle sending to arduino
            dataToSend = mainCube_queue.get(False)
            mainCube_queue.task_done()
            if dataToSend :
                sock1.send(dataToSend)
            #sock1.send("100")
            #time.sleep(1);
            #sock1.send("101")
            #time.sleep(1)
def magnet_Functions():
	sock2 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock2.connect((bd_addr2,port))
	sock2.setblocking(0)
	while 1:
            try:
                #handle reading from arduino
                data = sock2.recv(4)#garbage in the first char, ignore
                if data:
                    data2 = data[1:] #useful data, add to queue for Pi	
                    print("magnet arduino:")
                    print(data2)
                    #raspPi_queue.put(data2,False)
            except bluetooth.btcommon.BluetoothError:
                pass
            #handle sending to arduino
            #dataToSend = magnet_queue.get(False)
            #magnet_queue.task_done()
            #if dataToSend :
            #    sock2.send(dataToSend)
            sock2.send("100")
            time.sleep(1);
            sock2.send("101")
            time.sleep(1)
def laser_Functions():
	sock3 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock3.connect((bd_addr3,port))
	sock3.setblocking(0)
	while 1:
            try:
                #handle reading from arduino
                data = sock3.recv(4)#garbage in the first char, ignore    
                if data:
                    data2 = data[1:] #useful data, add to queue for Pi	
                    print("laser Arduino")
                    print(data2)
                #	raspPi_queue.put(data2,False)
            except bluetooth.btcommon.BluetoothError:
                pass
            #handle sending to arduino
            #dataToSend = magnet_queue.get(False)
            #laser_queue.task_done()
            #if dataToSend :
            #    sock3.send(dataToSend)
        
            sock3.send("100")
            time.sleep(1);
            sock3.send("101")
            time.sleep(1)
def mainFunction():
	dataToProcess = raspPi_queue.get(False)
	raspPi_queue.task_done()
	if dataToProcess:
		if data[0] == '1' : 
			if data[1:] == '01':
				#NFC card 1 was tapped
				print data[1:]
			if data[1:] == '02':
				#NFC card 2 was tapped
				print data[1:]
			if data[1:] == '03':
				#NFC card 3 was tapped
				print data[1:]
		
		if data[0] == '3':
			if data[1:] == '11':
				#cat5e_1 cable is plugged in and correct
				print data[1:]
			if data[1:] == '12':
				#cat5e_2 cable is plugged in and correct
				print data[1:]
			if data[1:] == '13':
				#cat5e_3 cable is plugged in and correct
				print data[1:]
			if data[1:] == '14':
				#cat5e_4 cable is plugged in and correct
				print data[1:]
			if data[1:] == '15':
				#cat5e_5 cable is plugged in and correct
				print data[1:]
			if data[1:] == '16':
				#chest on cube is open
				print data[1:]
			if data[1:] == '18':
				#keypad password is correct
				print data[1:]

#nfc_thread = Thread(target=nfc_Functions, args=(stuff1,stuff2))				
#nfc_thread = Thread(target=nfc_Functions)
#mainCube_thread = Thread(target=mainCube_Functions)
magnet_thread = Thread(target=magnet_Functions)
laser_thread = Thread(target=laser_Functions)
#mainLogic_thread = Thread(target=mainFunction)

#nfc_thread.setDaemon(True)
#mainCube_thread.setDaemon(True)
magnet_thread.setDaemon(True)
laser_thread.setDaemon(True)
#mainLogic_thread.setDaemon(True)

#nfc_thread.start()
#mainCube_thread.start()
laser_thread.start()
magnet_thread.start()
#mainLogic_thread.start()
count= 0
while(1):
	if count==0 :
		print("hello")
		count = 1

	
