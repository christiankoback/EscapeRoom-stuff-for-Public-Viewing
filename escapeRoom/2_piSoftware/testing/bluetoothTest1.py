import bluetooth
import time
 
#bd_addr = "98:D3:31:FC:59:F0" arduino 1
#bd_addr = "00:06:66:08:5F:A8" #Firefly
bd_addr = "98:D3:31:FC:6C:97" #main cube arduino

sock = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
sock.connect((bd_addr,1))
print "starting"

while 1:

	tosend = raw_input()
   	tosend = tosend + "\n"
	if tosend != "q\n":
		sock.send(tosend)
	else:
		break



	data = sock.recv(99)  #how many chars at a time
	print ">" + data + "<"
sock.close()
