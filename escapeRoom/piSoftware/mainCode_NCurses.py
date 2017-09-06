import bluetooth				#handle bluetooth communication
from threading import Thread	#handle threading
from Queue import *				#handle Queue functionality
import curses, time				#ncurses stuff

#setup connection for all devices, update these
bd_addr0 = "98:D3:31:FC:59:F0" #arduino 1 - nfc
bd_addr1 = "98:D3:31:FC:6C:97"#arduino 2 - main cube
bd_addr2 = "98:D3:31:FC:5A:09"  #arduino 3 - magnet
bd_addr3 = "98:D3:31:FD:68:77" #arduino 4 - laser

#setup socket per device
port = 1

nfcArduino_queue = Queue()
mainCubeArduino_queue = Queue()
magnetArduino_queue = Queue()
laserArduino_queue = Queue()
raspPi_queue = Queue()

hintCommand = "hint"
def hintFunction():
	#light up LED strip - specific command
	print("hello")

def nfcArduinoFunctions():
	sock0 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock0.connect((bd_addr0,port))
	while 1: 
		#handle reading from arduino
		data = sock0.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi		
		if data2:
			#send data to queue for raspPi
			raspPi_queue.put(data2,false)	
		#handle sending to arduino
		if nfcArduino_queue.empty():
			pass
		else:
			dataToSend = nfcArduino_queue.get(false)
			if dataToSend :
				sock0.send(dataToSend)
			nfcArduino_queue.task_done()
def mainCubeArduinoFunctions():
	sock1 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock1.connect((bd_addr1,port))
	while 1: 
		#handle reading from arduino
		data = sock1.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,false)	
		#handle sending to arduino
		if mainCubeArduino_queue.empty():
			pass
		else:
			dataToSend = mainCubeArduino_queue.get(false)
			if dataToSend :
				sock1.send(dataToSend)
			mainCubeArduino_queue.task_done()
def magnetArduinoFunctions():
	sock2 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock2.connect((bd_addr2,port))
	while 1: 
		#handle reading from arduino
		data = sock2.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,false)	
		#handle sending to arduino
		if magnetArduino_queue.empty():
			pass
		else:
			dataToSend = magnetArduino_queue.get(false)
			magnetArduino_queue.task_done()
			if dataToSend :
				sock2.send(dataToSend)
def laserArduinoFunctions():
	sock3 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock3.connect((bd_addr3,port))
	while 1: 
		#handle reading from arduino
		data = sock0.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,false)	
		#handle sending to arduino
		if laserArduino_queue.empty():
			pass
		else:
			dataToSend = laserArduino_queue.get(false)
			if dataToSend :
				sock3.send(dataToSend)
			laserArduino_queue.task_done()
def mainFunction():
	if raspPi_queue.empty():
		pass
	else:
		dataToProcess = raspPi_queue.get(False)
		if dataToProcess:
			if data[0] == '1' : 
				if data[1:] == '01':
					#NFC card 1 was tapped
					print (data[1:])
				if data[1:] == '02':
					#NFC card 2 was tapped
					print (data[1:])
				if data[1:] == '03':
					#NFC card 3 was tapped
					print (data[1:])
			
			if data[0] == '3':
				if data[1:] == '11':
					#cat5e_1 cable is plugged in and correct
					print (data[1:])
				if data[1:] == '12':
					#cat5e_2 cable is plugged in and correct
					print (data[1:])
				if data[1:] == '13':
					#cat5e_3 cable is plugged in and correct
					print (data[1:])
				if data[1:] == '14':
					#cat5e_4 cable is plugged in and correct
					print (data[1:])
				if data[1:] == '15':
					#cat5e_5 cable is plugged in and correct
					print (data[1:])
				if data[1:] == '16':
					#chest on cube is open
					print (data[1:])
				if data[1:] == '18':
					#keypad password is correct
					print (data[1:])
		raspPi_queue.task_done()

def ncursesFunction(): 
	userAnswer = "earth"
	passAnswer = "goodbye" 
	tempAnswer = ""
	isDone = False;

	state = "user"

	try:
		mainwindow = curses.initscr()
		mainWindow_height,mainWindow_width = mainwindow.getmaxyx()

		# Some curses-friendly terminal settings
		curses.cbreak()
		curses.noecho()
		mainwindow.refresh()

		#container for textbox
		begin_x = 0
		begin_y = 0
		height = mainWindow_height
		width = mainWindow_width
		window1 = curses.newwin(height, width, begin_y, begin_x)
		window1.box()
		window1.refresh()

		begin_x = (int) (mainWindow_width/3 ) + 1
		begin_y = (int) (mainWindow_height/3 ) + 2
		height = 7
		width = (int) (mainWindow_width/3) - 2
		window2 = curses.newwin(height, width, begin_y, begin_x)
		window2.keypad(1)
		window2.refresh()

		#border
		begin_x = (int) (mainWindow_width/2) - 12
		begin_y = (int) (mainWindow_height/2) - 9
		height = 3
		width = 23
		border = curses.newwin(height, width, begin_y, begin_x)
		border.box()
		border.refresh()

		#textbox
		begin_x = (int) (mainWindow_width/2) -11
		begin_y = (int) (mainWindow_height/2) - 8
		height = 1
		width = 21
		textbox1 = curses.newwin(height, width, begin_y, begin_x)
		textbox1.addstr( "     USERNAME")
		answer = userAnswer
		textbox1.refresh()

		#hint box
		begin_x = (int) (mainWindow_width/4)
		begin_y = (int) ( (mainWindow_height/2) +(mainWindow_height/4) )
		height = 6
		width = (int) (mainWindow_width/2)
		textbox1 = curses.newwin(height, width, begin_y, begin_x)
		textbox1.addstr( "     USERNAME")
		answer = userAnswer
		textbox1.refresh()
		
		while (isDone == False):	
			c = window2.getkey()
			if c == "q" :
				break	#exit while loop 
			window2.addch( c )
			if c == '\x08' :
				window2.delch( )
				tempAnswer = tempAnswer[: len(tempAnswer)-1]
			if ( c == "\n"):
				#enter was pressed
				if (tempAnswer == answer):
					#change to password section
					if (state == "user"):
						state = "pass"
						answer = passAnswer
						window2.clear()
						textbox1.clear()
						textbox1.addstr( "     PASSWORD")
						tempAnswer = ""
						textbox1.refresh()
						window2.refresh()
					else:
						if (state == "pass"):
							break
							#isDone = True
							#while(isDone):
							#	print ("Done puzzle")		#puzzle is done
				else:
					if (tempAnswer == hintCommand):
						#do hint
						print("this is hint command")
					tempAnswer = ""
					curses.beep()
					window2.clear()
					
			else:		
				if (tempAnswer == "" and c != "\n" ):
					tempAnswer = c
				else:
					tempAnswer += c
			window2.refresh()
	except:
		curses.nocbreak()
		window2.keypad(False)
		curses.echo()
		curses.endwin()

	curses.nocbreak()
	window2.keypad(False)
	curses.echo()
	curses.endwin()

#nfcArduino_thread = Thread(target=nfcArduinoFunctions, args=(stuff1,stuff2))				
nfcArduino_thread = Thread(target=nfcArduinoFunctions)
mainCubeArduino_thread = Thread(target=mainCubeArduinoFunctions)
magnetArduino_thread = Thread(target=magnetArduinoFunctions)
laserArduino_thread = Thread(target=laserArduinoFunctions)
mainLogic_thread = Thread(target=mainFunction)
ncurses_thread = Thread(target=ncursesFunction)

nfcArduino_thread.setDaemon(True)
mainCubeArduino_thread.setDaemon(True)
magnetArduino_thread.setDaemon(True)
laserArduino_thread.setDaemon(True)
mainLogic_thread.setDaemon(True)
ncurses_thread.setDaemon(True)

nfcArduino_thread.start()
mainCubeArduino_thread.start()
magnetArduino_thread.start()
laserArduino_thread.start()
mainLogic_thread.start()
ncurses_thread.start()

print("hello stranger. welcome to an infinite loop")
while(1):
	print("in infinite loop")
	time.sleep(200)
