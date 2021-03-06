import threading, Queue, bluetooth, time
from sys import exit

#Addresses correct
main_cube_addr = "98:D3:31:FC:6C:97"
nfc_addr = "98:D3:31:FC:59:F0"
magnet_addr = "98:D3:31:FC:5A:09" 
laser_addr = "98:D3:31:FD:68:77" 

MainCubeBTSocket = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
MainCubeBTSocket.connect((main_cube_addr,1))



incoming = Queue.Queue()
toLaser = Queue.Queue()
toMagnet = Queue.Queue()
toCube1 = Queue.Queue()
toCube2 = Queue.Queue()

#BEFIN BLUETOOTH ######################################################

def bluetoothFunction(incoming, toLaser, toMagnet, toCube1, toCube2, stop_event):
	while not stop_event.wait(1):   	
		#print 'bluetooth receiving (2 sec wait)'		
		data = MainCubeBTSocket.recv(4)  #how many chars at a time
		#print '>' +data.strip() + '<in the queue'
		incoming.put(data.strip())
		
		# if something in the queue send it
		while not toCube1.empty():
			MainCubeBTSocket.send(toCube1.get())
		while not toCube2.empty():
			MainCubeBTSocket.send(toCube2.get())
		while not toMagnet.empty():
			MainCubeBTSocket.send(toMagnet.get())
		while not toLaser.empty():
			MainCubeBTSocket.send(toLaser.get())
		#time.sleep(2)
	print 'bluetooth stopping'
	
# END OF BLUETOOTH #######################################################

def logicFunction(incoming, toLaser, toMagnet, toCube1, toCube2, stop_event):
	while not stop_event.wait(1):   
		#print 'logic 2 sec'
		if not incoming.empty():
			print "Received>" + incoming.get() + "<"
		#time.sleep(2)
	#print 'logic stopping'

#END OF LOGIC ############################################################

pill2kill = threading.Event()

bluetoothThread = threading.Thread(target=bluetoothFunction,args=(incoming, toLaser, toMagnet, toCube1, toCube2, pill2kill))
logicThread = threading.Thread(target=logicFunction,args=(incoming, toLaser, toMagnet, toCube1, toCube2, pill2kill))


print 'Starting Threads'
bluetoothThread.start()
logicThread.start()

while 1:
	tosend = raw_input()
	if tosend != "q":
		toCube1.put(tosend + "\n")
	else:
		pill2kill.set()
		print 'killing threads'
		bluetoothThread.join()#waits kill that thread is dead (busy wait)
		print 'killed Bluetooth Thread'
		logicThread.join()
		print 'killed LogicThread'
		exit(0)

