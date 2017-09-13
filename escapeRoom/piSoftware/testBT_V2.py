import bluetooth
 
#bd_addr = "98:D3:31:FC:59:F0" arduino 1
bd_addr = "98:D3:31:FC:5A:09" #replace this with different arduino 

port = 1
sock = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
sock.connect((bd_addr,port))
sock.setblocking(False)


data2 = ""
while 1:
	
	data = sock.recv(6)#how many chars at a time
	if data:
		data2 = data[1:]
	print (data2)
	sock.send(data2)
    
	tosend = input()
	#tosend += "oles\r"
    
	if tosend == 'q':
		break
    #else:
    #   sock.send(tosend)  
sock.close()


#just a test commit