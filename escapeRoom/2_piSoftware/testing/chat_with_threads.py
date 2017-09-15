import threading, Queue, bluetooth, time

bd_addr = "98:D3:31:FC:6C:97" #main cube arduino
sock = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
sock.connect((bd_addr,1))


def bluetooth(num, q, stop_event):
	while not stop_event.wait(1):   	
		print 'bluetooth receiving (2 sec wait)\n'		
		data = sock.recv(1000)  #how many chars at a time
		#q.put(data)
		sock.
		time.sleep(2)
	print 'bluetooth stopping'
# END OF BLUETOOTH #########################################

def logic(num, q, stop_event):
	while not stop_event.wait(1):   
		print 'logic 2 sec'
		#print "Received>" + q.get() + "<"
		time.sleep(2)
	print 'logic stopping'

#END OF LOGIC #########################################


print 'setup'

num = 2
q = Queue.Queue()
pill2kill = threading.Event()
thread1 = threading.Thread(target=bluetooth,args=(num,q, pill2kill))
thread2 = threading.Thread(target=logic,args=(num,q, pill2kill))



print 'end setup. Starting Threads'
thread1.start()
thread2.start()

while 1:

	tosend = raw_input()
   	tosend = tosend + "\n"
	if tosend != "q\n":
		sock.send(tosend)
	else:
		pill2kill.set()
		print 'killing threads'
		thread1.join()#waits kill that thread is dead (busy wait)
		print 'killed 1st'
		thread2.join()
		print 'killed 2nd'
		break

