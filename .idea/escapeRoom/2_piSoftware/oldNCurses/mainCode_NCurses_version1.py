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
timer_queue = Queue()
hint_queue = Queue()

hintCommand = "hint"
restartCommand = "~restart"
isDonePassword = 0
isChestOpen = 0
minutes = 25

def isLetter(possibleLetter):
	if possibleLetter > 64 and possibleLetter <91:
		return 1
	else:
		if possibleLetter > 96 and possibleLetter < 123:
			return 1
	return 0
def countdown():
	t = minutes * 60
	while t:
		mins, secs = divmod(t, 60)
		timeformat = '{:02d}:{:02d}'.format(mins, secs)
		#print(timeformat, end='\r')
		if timer_queue.empty():
				pass
		else:
			garbage = timer_queue.get(True)
		timer_queue.put( timeformat , True)
		time.sleep(1)
		t -= 1
	
	timer_queue.put(t, True)
def hintFunction():
	#light up LED strip - specific command
	magnetArduino_queue.put("202"   , True)
	time.sleep(10)
	magnetArduino_queue.put("201"   , True)

def nfcArduinoFunctions():
	try:
		sock0 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock0.connect((bd_addr0,port))
		sock0.setblocking(0)
		hint_queue.put("bluetooth success, NFC",True)
	except:
		hint_queue.put("bluetooth error, NFC",True)
	while 1:
		data2='999999'
		message = " "
		try:
			#handle reading from arduino
			data = sock0.recv(4)#garbage in the first char, ignore
			if data:
				if len(data) == 4:
					data2 = data[1:] #useful data, add to queue for Pi
				if len(data) == 3:
					data2 = data
				message = "data: " + data2
				#send data to queue for raspPi
				#print("nfc arduino:")
				#print(data2)
				if data2 != '999999':
					message = message + "data is valid"
					hint_queue.put(message,True)
					raspPi_queue.put(data2,True)
		except bluetooth.btcommon.BluetoothError:
			pass
		#handle sending to arduino
		if nfcArduino_queue.empty():
			pass
		else:
			message = "NFC queue NOT empty"
			hint_queue.put(message,True)
			dataToSend = nfc_queue.get(True)
			if dataToSend :
				sock0.send(dataToSend)
			nfc_queue.task_done()
		
		time.sleep(5.0/100.0)
def mainCubeArduinoFunctions():
	try:
		sock1 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock1.connect((bd_addr1,port))
		sock1.setblocking(0)
	except:
		hint_queue.put("bluetooth error, main Cube",True)
	while 1:
		try:
			#handle reading from arduino
			data = sock1.recv(4)#garbage in the first char, ignore
			if data:
				if len(data) == 4:
					data2 = data[1:] #useful data, add to queue for Pi
				if len(data) == 3:
					data2 = data
				#send data to queue for raspPi
				#print("nfc arduino:")
				#print(data2)
				if data2 != '999999':
					#send data to queue for raspPi
					#print("mainCubeArduino_queue:")
					#print(data2)
					raspPi_queue.put(data2,True)
	    	except bluetooth.btcommon.BluetoothError:
                	pass
	    	#handle sending to arduino
		if mainCubeArduino_queue.empty():
			pass
		else:
			dataToSend = mainCubeArduino_queue.get(True)
			if dataToSend :
				sock1.send(dataToSend)
			mainCubeArduino_queue.task_done()	
		time.sleep(7.0/100.0)
def magnetArduinoFunctions():
	try:
		sock2 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock2.connect((bd_addr2,port))
		sock2.setblocking(0)
	except:
		hint_queue.put("bluetooth error, magnet",True)
	while 1:
		try:
			#handle reading from arduino
			data = sock2.recv(4)#garbage in the first char, ignore
			if data:
				if len(data) == 4:
					data2 = data[1:] #useful data, add to queue for Pi
				if len(data) == 3:
					data2 = data
				message = "data: " + data2
				#send data to queue for raspPi
				#print("nfc arduino:")
				#print(data2)
				if data2 != '999999':
					#send data to queue for raspPi
					#print("magnetArduino_queue:")
					#print(data2)
					raspPi_queue.put(data2,True)
	    	except bluetooth.btcommon.BluetoothError:
                	pass
	    	#handle sending to arduino
		if magnetArduino_queue.empty():
			pass
		else:
			dataToSend = magnetArduino_queue.get(True)
			if dataToSend :
				sock2.send(dataToSend)
			magnetArduino_queue.task_done()	
		time.sleep(11.0/100.0)
def laserArduinoFunctions():
	try:
		sock3 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock3.connect((bd_addr3,port))
		sock3.setblocking(0)
	except:
		hint_queue.put("bluetooth error, laser",True)
	while 1:
		try:
			#handle reading from arduino
			data = sock3.recv(4)#garbage in the first char, ignore
			if data:
				if len(data) == 4:
					data2 = data[1:] #useful data, add to queue for Pi
				if len(data) == 3:
					data2 = data
				message = "data: " + data2
				#send data to queue for raspPi
				#print("nfc arduino:")
				#print(data2)
				if data2 != '999999':	
					#send data to queue for raspPi
					#print("magnetArduino_queue:")
					#print(data2)
					raspPi_queue.put(data2,True)
	    	except bluetooth.btcommon.BluetoothError:
                	pass
	    	#handle sending to arduino
		if laserArduino_queue.empty():
			pass
		else:
			dataToSend = laserArduino_queue.get(True)
			if dataToSend :
				sock3.send(dataToSend)
			laserArduino_queue.task_done()	

def mainFunction():
	
	if raspPi_queue.empty():
		message = "pi queue is empty"
		hint_queue.put(message,True)
		#pass
	else:
		dataToProcess = raspPi_queue.get(True)
		message = "pi queue NOT empty"
		hint_queue.put(message,True)
		if dataToProcess:
			message = "data processed in main: " + dataToProcess + " mmm"
			hint_queue.put(message,True)
			if data[0] == '1' : 
				if data[1:] == '01':
					#NFC card 1 was tapped, start servo laser, blink orange wire
					laserArduino_queue.put("300" , True)
					mainCubeArduino_queue.put("228" , True)
				if data[1:] == '02':
					#NFC card 2 was tapped, change lol shield pattern
					mainCubeArduino_queue.put("242" , True)
				if data[1:] == '03':
					#NFC card 3 was tapped, check state of puzzles, blink orange wire
					print (data[1:])
					mainCubeArduino_queue.put("228" , True)
					if (isDonePassword == 1) :
						if (isChestOpen == 1)  :
							#do something, video or something
							hint.clear()
							hint.addstr("GAME OVER !!")
			
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
					#chest on cube is open, turn on blue wire 
					isChestOpen = 1
					mainCubeArduino_queue.put("224" , True)
				if data[1:] == '18':
					#keypad password is correct
					mainCubeArduino_queue.put("223" , True)
		raspPi_queue.task_done()
		time.sleep(3.0/100.0)
def ncursesFunction(): 
	characterLengthMax = 45
	countTemp = 0
	userAnswer1 = "MYGEETONIA"
	userAnswer2 = "mygeetonia"
	passAnswer1 = "EARTH" 
	passAnswer2 = "earth"
	tempAnswer = ""
	isDonePassword = 0

	state = "user"

	try:
		mainwindow = curses.initscr()
		mainWindow_height,mainWindow_width = mainwindow.getmaxyx()
		# Some curses-friendly terminal settings
		curses.cbreak()
		curses.noecho()
		mainwindow.refresh()
		
		#timer
		begin_x = 68
		begin_y = 7
		height = 3
		width = 6
		timer = curses.newwin(height, width, begin_y, begin_x)
		timer.box()
		timer.refresh()
		
		#user input
		begin_x = 36
		begin_y = 16
		height = 15
		width = 74
		window1 = curses.newwin(height, width, begin_y, begin_x)
		window1.box()
		window1.refresh()
		
		begin_x = 60
		begin_y = 23
		height = 5
		width = 48
		window2 = curses.newwin(height, width, begin_y, begin_x)
		window2.nodelay(1)
		window2.keypad(1)
		window2.refresh()
		
		# text border
		begin_x = 57
		begin_y = 14
		height = 5
		width = 32
		border = curses.newwin(height, width, begin_y, begin_x)
		border.box()
		border.refresh()
		
		
		userItem = "     NAME OF 4TH DIMENSION"
		passItem = "            ORIGIN"
		hintItem = "              TYPE -HINT- ANYTIME FOR PUZZLE CLUE"
		#textbox
		begin_x = 58
		begin_y = 16
		height = 1
		width = 30
		textbox1 = curses.newwin(height, width, begin_y, begin_x)
		textbox1.addstr( userItem)
		textbox1.refresh()
		
		#hint box
		begin_x = 35
		begin_y = 35
		height = 10
		width = 74
		hint = curses.newwin(height, width, begin_y, begin_x)
		#hint.addstr( hintItem)
		hint.refresh()
		
		#count = 0
		#while(1):
		#	if count == 0:
		#		count = 1
		
		
		while (isDonePassword == 0):
			if hint_queue.empty():
				pass
			else:
				hint.clear()
				hint.addstr(hint_queue.get(False) )
				hint.refresh()
				
			if timer_queue.empty():
				pass
			else:
				tempTime = timer_queue.get(True)
				if tempTime:
					if (isDonePassword == 0)  :
						timer.clear()
						timer.addstr(tempTime )
						timer.refresh()
						window2.refresh()
				
			data = window2.getch()
			if (data > 0) :
				if (data == 10):
					if tempAnswer == userAnswer1 or tempAnswer == userAnswer2:
						#change to password section
						window2.clear()
						textbox1.clear()
						textbox1.addstr(passItem)
						tempAnswer = ""
						window2.refresh()
						textbox1.refresh()
					if tempAnswer == passAnswer1 or tempAnswer == passAnswer2:
						isDonePassword = 1
						tempAnswer = ""
						while(True):
							data = window2.getch()
							if data > 0:
								c = chr(data)
								if data == 10:
									if (tempAnswer == restartCommand):
										#restart everything
										isDonePassword = 0
										isChestOpen = 0
										tempAnswer = ""
										textbox1.clear()
										textbox1.addstr( userItem)
										textbox1.refresh()
										timer_thread.start()
										nfc_queue.put("320", True)
										magnet_queue.put("301", True)
										window2.clear()
									else:
										tempAnswer = ""
										window2.clear()
								else:
									if isLetter(data):
										if (tempAnswer == "" and c != "\n" ):
											tempAnswer = c
										else:
											tempAnswer += c
										tempCount = tempCount + 1
										window2.addch( c )
					else:
						if (tempAnswer == hintCommand):
							#do hint
							hint.clear()
							#hint.addstr("I shall give you pity points. Here's a Clue.")
							#hintFunction()
							hint.refresh()
						if (tempAnswer == restartCommand):
							#restart everything
							isDonePassword = 0
							isChestOpen = 0
							state = "user"
							tempAnswer = ""
							textbox1.addstr(userItem)
							timer_thread.start()
							nfc_queue.put("320", True)
							magnet_queue.put("301", True)
								
						tempAnswer = ""
						curses.beep()
						window2.clear()
						
				else:
					if data == 263 :
						tempString = tempAnswer[: len(tempAnswer)-1]
						window2.clear( )
						window2.addstr(tempString)
						tempAnswer = tempString
					else:
						c = chr(data)
						if isLetter(data):
							if (tempAnswer == "" and c != "\n" ):
								tempAnswer = c
							else:
								tempAnswer += c
							window2.addch( c )
				window2.refresh()
	except:
		curses.nocbreak()
		#window2.keypad(False)
		curses.echo()
		curses.endwin()

	curses.nocbreak()
	#window2.keypad(False)
	curses.echo()
	curses.endwin()

#nfcArduino_thread = Thread(target=nfcArduinoFunctions, args=(stuff1,stuff2))				
nfcArduino_thread = Thread(target=nfcArduinoFunctions)
mainCubeArduino_thread = Thread(target=mainCubeArduinoFunctions)
magnetArduino_thread = Thread(target=magnetArduinoFunctions)
laserArduino_thread = Thread(target=laserArduinoFunctions)
mainLogic_thread = Thread(target=mainFunction)
#ncurses_thread = Thread(target=ncursesFunction)
timer_thread = Thread(target=countdown)

nfcArduino_thread.setDaemon(True)
mainCubeArduino_thread.setDaemon(True)
magnetArduino_thread.setDaemon(True)
laserArduino_thread.setDaemon(True)
mainLogic_thread.setDaemon(True)
#ncurses_thread.setDaemon(True)
timer_thread.setDaemon(True)

nfcArduino_thread.start()
mainCubeArduino_thread.start()
magnetArduino_thread.start()
laserArduino_thread.start()
mainLogic_thread.start()
#ncurses_thread.start()
timer_thread.start()


ncursesFunction()
