import bluetooth				#handle bluetooth communication
from threading import Thread	#handle threading
from Queue import *				#handle Queue functionality
import curses, time				#ncurses stuff

#setup connection for all devices, update these
bd_addr0 = "98:D3:31:FC:59:F0" #arduino 1
bd_addr1 = "98:D3:31:FC:5A:09" #arduino 2
bd_addr2 = "98:D3:31:FC:6C:97" #arduino 3
bd_addr3 = "98:D3:31:FD:68:77" #arduino 4

#setup socket per device
port = 1

nfcArduino_queue = Queue()
mainCubeArduino_queue = Queue()
magnetArduino_queue = Queue()
laserArduino_queue = Queue()
raspPi_queue = Queue()

hintCommand = "hint"
hintWanted = 0;
cursesPuzzleComplete = 0;

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
			raspPi_queue.put(data2,True)	
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
		data = sock0.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,True)	
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
		data = sock0.recv(4)#garbage in the first char, ignore
		data2 = data[1:] #useful data, add to queue for Pi	
		if data2:
			raspPi_queue.put(data2,True)	
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
			raspPi_queue.put(data2,True)	
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
					#NFC card 1 was tapped, start/reset PanTilt laser
					laserArduino_queue.put("300",True)
				if data[1:] == '02':
					#NFC card 2 was tapped
					mainCubeArduino_queue.put("242", True)
				if data[1:] == '03':
					#NFC card 3 was tapped, check finished conditions for curses/box puzzle
					if cursesPuzzleComplete == 1:
						#blink long orange glowing wire for 10 sec(on for 1sec, off for 1sec)
						#keep long orange glowing wire on bright - constant
						mainCubeArduino_queue.put("222",True)
					else:
						#blink long orange glowing wire for 10 sec(on for 1sec, off for 1sec)
						#turn long orange glowing wire off
						mainCubeArduino_queue.put("223",True)
			
			if data[0] == '3':
				if data[1:] == '11':
					#cat5e puzzle is complete
					print (data[1:])
				if data[1:] == '12':
					#cat5e 2 custom
					print (data[1:])
				if data[1:] == '13':
					#cat5e 3 custom
					print (data[1:])
				if data[1:] == '14':
					#cat5e 4 custom
					print (data[1:])
				if data[1:] == '15':
					#cat5e 5 custom
					print (data[1:])
				if data[1:] == '16':
					#chest on cube is open, turn on green glowing wire
					mainCubeArduino_queue.put("225",True)
				if data[1:] == '18':
					#keypad password is correct, light blue glowing wire
					mainCubeArduino_queue.put("224",True)
					
			if data[0] == '5':
				if data[1:] == '01':
					#sensor 1 is triggered, PanTilt sensor. turn off magnet?
					magnetArduino_queue.put("301",True)
				if data[1:] == '01':
					#sensor 2 is triggered
					print (data[1:])
				if data[1:] == '01':
					#sensor 3 is triggered
					print (data[1:])
				if data[1:] == '01':
					#sensor 4 is triggered
					print (data[1:])
				if data[1:] == '01':
					#sensor 5 is triggered
					print (data[1:])
				if data[1:] == '01':
					#sensor 6 is triggered
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
		cursesPuzzleComplete = 0
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
							cursesPuzzleComplete = 1
							break		#is this wanted ? ?
							#isDone = True
							#while(isDone):
							#	print ("Done puzzle")		#puzzle is done
				else:
					if (tempAnswer == hintCommand):
						#do hint
						hintWanted = 0
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
		cursesPuzzleComplete = 0
		curses.nocbreak()
		window2.keypad(False)
		curses.echo()
		curses.endwin()
	cursesPuzzleComplete = 0
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