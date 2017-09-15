import bluetooth
 
#bd_addr = "98:D3:31:FC:59:F0" arduino 1
#bd_addr = "00:06:66:08:5F:A8" #Firefly
bd_addr = "98:D3:31:FC:6C:97" #main cube arduino

port = 1
sock = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
sock.connect((bd_addr,port))
 
while 1:
    data = sock.recv(20)#how many chars at a time
    print data
    sock.send(data)
    
    tosend = raw_input()
    tosend += "oles\r"
    
    if tosend != 'q':
        sock.send(tosend)
    else:
        break
sock.close()
