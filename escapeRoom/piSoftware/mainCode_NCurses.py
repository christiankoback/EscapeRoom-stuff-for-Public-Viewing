import bluetooth				#handle bluetooth communication
from threading import Thread	#handle threading
import Queue					#handle Queue functionality
import curses, time				#ncurses stuff

#setup connection for all devices, update these
bd_addr0 = "98:D3:31:FC:59:F0" #arduino 1
bd_addr1 = "98:D3:31:FC:5A:09" #arduino 2
bd_addr2 = "98:D3:31:FC:6C:97" #arduino 3
bd_addr3 = "98:D3:31:FD:68:77" #arduino 4

#setup socket per device
port = 1

arduino1_queue = Queue()
arduino2_queue = Queue()
arduino3_queue = Queue()
arduino4_queue = Queue()
raspPi_queue = Queue()



def arduino1Functions():
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
		dataToSend = arduino1_queue.get(false)
		arduino1_queue.task_done()
		if dataToSend :
			sock0.send(dataToSend)

def arduino2Functions():
	sock1 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock1.connect((bd_addr1,port))
	while 1: 
		#handle reading from arduino
		data = sock0.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,false)	
		#handle sending to arduino
		dataToSend = arduino2_queue.get(false)
		arduino2_queue.task_done()
		if dataToSend :
			sock1.send(dataToSend)

def arduino3Functions():
	sock2 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock2.connect((bd_addr2,port))
	while 1: 
		#handle reading from arduino
		data = sock0.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,false)	
		#handle sending to arduino
		dataToSend = arduino3_queue.get(false)
		arduino3_queue.task_done()
		if dataToSend :
			sock2.send(dataToSend)

def arduino4Functions():
	sock3 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
	sock3.connect((bd_addr3,port))
	while 1: 
		#handle reading from arduino
		data = sock0.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,false)	
		#handle sending to arduino
		dataToSend = arduino3_queue.get(false)
		arduino4_queue.task_done()
		if dataToSend :
			sock3.send(dataToSend)
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
		window2.clear()
		window2.refresh()

	curses.nocbreak()
	window2.keypad(False)
	curses.echo()
	curses.endwin()
	
#arduino1_thread = Thread(target=arduino1Functions, args=(stuff1,stuff2))				
arduino1_thread = Thread(target=arduino1Functions)
arduino2_thread = Thread(target=arduino2Functions)
arduino3_thread = Thread(target=arduino3Functions)
arduino4_thread = Thread(target=arduino4Functions)
mainLogic_thread = Thread(target=mainFunction)
ncurses_thread = Thread(target=ncursesFunction)

arduino1_thread.setDaemon(True)
arduino2_thread.setDaemon(True)
arduino3_thread.setDaemon(True)
arduino4_thread.setDaemon(True)
mainLogic_thread.setDaemon(True)
ncurses_thread.setDaemon(True)

arduino1_thread.start()
arduino2_thread.start()
arduino3_thread.start()
arduino4_thread.start()
mainLogic_thread.start()
ncurses_thread.start()
