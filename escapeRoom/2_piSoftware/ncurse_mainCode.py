#import bluetooth				#handle bluetooth communication
from threading import Thread	#handle threading
from queue import *				#handle Queue functionality
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

#commands
hintCommand = "hint"
quitCommand = "olesQuit"
startCommand = "olesStart"
minutes = 20
isPassDone = 0

#titles for user input are below

userAnswer1 = "MYGEETONIA"
userAnswer2 = "mygeetonia"
passAnswer1 = "EARTH" 
passAnswer2 = "earth"
adminTitle = "            ADMIN"
userTitle = "     NAME OF 4TH DIMENSION"
passTitle = "            ORIGIN"
hintItem = "              TYPE -HINT- ANYTIME FOR PUZZLE CLUE"

isTimerKilled = 0

def isLetter(possibleLetter):
	if possibleLetter > 64:
		if possibleLetter < 91:
			return 1
		else:
			return 0
	else:
		if possibleLetter > 96:
			if possibleLetter < 123:
				return 1
			else:
				return 0
def countdown():
	#keeps track of countdown timer
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
#thread setup
#nfcArduino_thread = Thread(target=nfcArduinoFunctions, args=(stuff1,stuff2))				
#nfcArduino_thread = Thread(target=nfcArduinoFunctions)
#mainCubeArduino_thread = Thread(target=mainCubeArduinoFunctions)
#magnetArduino_thread = Thread(target=magnetArduinoFunctions)
#laserArduino_thread = Thread(target=laserArduinoFunctions)
#mainLogic_thread = Thread(target=mainFunction)
timer_thread = Thread(target=countdown)

#nfcArduino_thread.setDaemon(True)
#mainCubeArduino_thread.setDaemon(True)
#magnetArduino_thread.setDaemon(True)
#laserArduino_thread.setDaemon(True)
#mainLogic_thread.setDaemon(True)
timer_thread.setDaemon(True)

def killCommand():
	#nfcArduino_thread.terminate()
	#mainCubeArduino_thread.terminate()
	#magnetArduino_thread.terminate()
	#laserArduino_thread.terminate()
	#mainLogic_thread.terminate()
	timer_thread.terminate()
	
	curses.nocbreak()
	mainBody.keypad(False)
	curses.echo()
	curses.endwin()
def hintFunction():
	#hint functionality
	magnetArduino_queue.put("202"   , True)
	time.sleep(10)
	magnetArduino_queue.put("201"   , True)
#checks if data is a letter

def arduinoConnect():
	message = ""
	try:
		sock0 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock0.connect((bd_addr0,port))
		sock0.setblocking(0)
		message = message + "bluetooth success, NFC:"
	except:
		message = message + "bluetooth error, NFC:"
	try:
		sock1 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock1.connect((bd_addr1,port))
		sock1.setblocking(0)
		message = message + "bluetooth success, main Cube:"
	except:
		message = message + "bluetooth error, main Cube:"
	try:
		sock2 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock2.connect((bd_addr2,port))
		sock2.setblocking(0)
		message = message + "bluetooth success, magnet:"
	except:
		message = message + "bluetooth error, magnet:"
	try:
		sock3 = bluetooth.BluetoothSocket (bluetooth.RFCOMM)
		sock3.connect((bd_addr3,port))
		sock3.setblocking(0)
		message = message + "bluetooth success, laser:"
	except:
		message = message + "bluetooth error, laser:"	
	adminHint_queue.put(message,True)

def ncursesFunction(titleWindow,textWindow, hintWindow):
	count = 0
	isUserDone = 0
	tempAnswer = ""
	while(isPassDone == 0):
		if count == 0:
			count = 1
			
			#timer
			begin_x = 68
			begin_y = 7
			height = 3
			width = 6
			timer = curses.newwin(height, width, begin_y, begin_x)
			timer.box()
			timer.refresh()
			
			titleWindow.addstr(userTitle)
			titleWindow.refresh()
			
		
		else:
			pass
		if hint_queue.empty():
			pass
		else:
			hintWindow.clear()
			hintWindow.addstr(hint_queue.get(False) )
			hintWindow.refresh()
		
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
						
		data = textWindow.getch()
		if data > 0 :
			if data == 10:
				if isUserDone == 0:
					if tempAnswer == userAnswer1 or tempAnswer == userAnswer2:
						titleWindow.clear()
						titleWindow.addstr(userTitle)
						tempAnswer = ""
						textWindow.clear()
						textWindow.refresh()
						isUserDone = 1
						titleWindow.refresh()
					else:
						if tempAnswer == quitCommand:
							killCommand()
						else:
							if tempAnswer == hintCommand:
								hintFunction()
							else:
								pass
						tempAnswer = ""
						textWindow.clear()
						textWindow.refresh()
				else:
					if isUserDone == 1:
						if tempAnswer == passAnswer1 or tempAnswer == passAnswer2:
							titleWindow.clear()
							titleWindow.addstr(passTitle)
							isPassDone = 1
							textWindow.clear()
							textWindow.refresh()
							titleWindow.refresh()
						else:
							if tempAnswer == quitCommand:
								killCommand()
							else:
								if tempAnswer == hintCommand:
									hintFunction()
								else:
									pass
							tempAnswer = ""
							textWindow.clear()
							textWindow.refresh()
					else:
						if tempAnswer == quitCommand:
							killCommand()
						else:
							if tempAnswer == hintCommand:
								hintFunction()
							else:
								pass
						tempAnswer = ""
						textWindow.clear()
						textWindow.refresh()
			else:
				if data == 263 :
					tempAnswer = tempAnswer[: len(tempAnswer)-1]
					textWindow.clear()
					textWindow.addstr(tempAnswer)
					textWindow.refresh()
				else:
					if isLetter(data) == 1 or data == 126:
						c = chr(data)
						if tempAnswer == "":
							tempAnswer = c
						else:
							tempAnswer = tempAnswer + c
						textWindow.addch(c)
						textWindow.refresh()
					else:
						pass
		else:
			pass
			
			
			
def mainNCurses():
	tempAnswer = ""
	try:
		mainwindow = curses.initscr()
		mainWindow_height,mainWindow_width = mainwindow.getmaxyx()
		curses.raw()
		curses.cbreak()
		curses.noecho()
		mainwindow.refresh()

		#user input
		begin_x = 36
		begin_y = 16
		height = 15
		width = 74
		mainBodyBorder = curses.newwin(height, width, begin_y, begin_x)
		mainBodyBorder.box()
		mainBodyBorder.refresh()
		
		begin_x = 60
		begin_y = 23
		height = 5
		width = 48
		mainBody = curses.newwin(height, width, begin_y, begin_x)
		mainBody.nodelay(1)
		mainBody.keypad(1)
		mainBody.refresh()
		
		# text border
		begin_x = 57
		begin_y = 14
		height = 5
		width = 32
		mainTitleBorder = curses.newwin(height, width, begin_y, begin_x)
		mainTitleBorder.box()
		mainTitleBorder.refresh()
		
		#textbox
		begin_x = 58
		begin_y = 16
		height = 1
		width = 30
		mainTitle = curses.newwin(height, width, begin_y, begin_x)
		mainTitle.clear()
		mainTitle.addstr(adminTitle)
		mainTitle.refresh()
		
		#hint box
		begin_x = 45
		begin_y = 35
		height = 10
		width = 74
		hint = curses.newwin(height, width, begin_y, begin_x)
		hint.refresh()
		
		while(1):
			if hint_queue.empty():
				pass
			else:
				hint.clear()
				hint.addstr(adminHint_queue.get(False) )
				hint.refresh()
			
			data = mainBody.getch()
			if data > 0 :
				if isLetter(data) == 1 or data == 126:
					c = chr(data)
					if tempAnswer == "":
						tempAnswer = c
					else:
						tempAnswer = tempAnswer + c
					mainBody.addch(c)
					mainBody.refresh()
				else:
					if data == 263 :
						tempAnswer = tempAnswer[: len(tempAnswer)-1]
						mainBody.clear()
						mainBody.addstr(tempAnswer)
						mainBody.refresh()
					else:
						if data == 10:
							if tempAnswer == startCommand:
								tempAnswer = ""
								hint.clear()
								hint.refresh()
								mainTitle.addstr(userTitle)
								
								#nfcArduino_thread.start()
								#mainCubeArduino_thread.start()
								#magnetArduino_thread.start()
								#laserArduino_thread.start()
								#mainLogic_thread.start()
								timer_thread.start()
								
								escapeRoomResult = ncursesFunction(mainTitle, mainBody, hint)
							else:
								if tempAnswer == quitCommand:
									killCommand()
								tempAnswer = ""
								mainBody.clear()
								mainBody.refresh()
	except:
		killCommand()
		
mainNCurses()	


	