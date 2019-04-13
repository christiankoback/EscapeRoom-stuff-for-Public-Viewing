from threading import Thread	#handle threading
import Queue 				#handle Queue functionality
import curses			#ncurses stuff	
import time

hint_queue = Queue.Queue()

#commands for various software sections
hintCommand = "hint"
quitCommand = "olesQuit"
startCommand = "olesStart"

#variables to keep track of software progress

wordLength = 30
minutes = 20	#number of minutes in timer countdown

#various variables used in software
adminTitle ="          ADMIN"
userTitle="     NAME OF 4TH DIMENSION"
passTitle="            ORIGIN"
hintTitle="              TYPE -HINT- ANYTIME FOR PUZZLE CLUE"
ncursesResult = 0
userAnswer1 = "MYGEETONIA"
userAnswer2 = "mygeetonia"
passAnswer1 = "EARTH"
passAnswer2 = "earth"


def isLetter(possibleLetter):
	if 64 < possibleLetter < 91:
		return 1
	else:
		if 96 < possibleLetter < 123:
			return 1
	return 0


def ncursesAdmin():
	tempAnswer = ""
	letterCount = 0
	try:
		mainwindow = curses.initscr()
		#mainWindow_height,mainWindow_width = mainwindow.getmaxyx()
		curses.raw()
		curses.cbreak()
		curses.noecho()
		mainwindow.refresh()
		
		#timer
		begin_x = 68
		begin_y = 7
		height = 3
		width = 6
		timer = curses.newwin(height,width,begin_y,begin_x)
		timer.refresh()
		
		#user input
		begin_x = 36
		begin_y = 16
		height = 15
		width = 74
		bodyBorder = curses.newwin(height, width, begin_y, begin_x)
		bodyBorder.box()
		bodyBorder.refresh()
		
		begin_x = 60
		begin_y = 23
		height = 5
		width = 48
		body = curses.newwin(height, width, begin_y, begin_x)
		body.nodelay(1)
		body.keypad(1)
		body.refresh()
		
		# text border
		begin_x = 57
		begin_y = 14
		height = 5
		width = 32
		titleBorder = curses.newwin(height, width, begin_y, begin_x)
		titleBorder.box()
		titleBorder.refresh()
		
		#textbox
		begin_x = 58
		begin_y = 16
		height = 1
		width = 30
		title = curses.newwin(height, width, begin_y, begin_x)
		title.addstr( adminTitle)
		title.refresh()
		
		#hint box
		begin_x = 45
		begin_y = 35
		height = 10
		width = 74
		hint = curses.newwin(height, width, begin_y, begin_x)
		hint.refresh()
		body.refresh()
		
		while True:
			isDonePassword = 0
			data = body.getch()
			if data > 0 :
				if data == 263 :
					if tempAnswer != "":
						if len(tempAnswer) == 1:
							tempAnswer = ""
						else:
							tempAnswer = tempAnswer[: len(tempAnswer)-1]
						body.clear( )
						body.addstr(tempAnswer)
						body.refresh()	
					else:
						pass
				else:
					if isLetter(data) == 1:
						letter = chr(data)
						if letterCount < wordLength:
							if tempAnswer == "":
								tempAnswer = letter
								letterCount = 1 
							else:
								tempAnswer += letter
								letterCount += 1
							body.clear( )
							body.addstr(tempAnswer)
							body.refresh()	
						else:
							letterCount = 0 
							tempAnswer = letter
							body.clear( )
							body.addstr(tempAnswer)
							body.refresh()
					else:
						if data == 10:
							if tempAnswer == startCommand:
								title.clear()
								body.clear()
								title.addstr(userTitle)
								title.refresh()
								body.refresh()
								tempAnswer = ""

								title.clear()
								body.clear()
								title.addstr(userTitle)
								title.refresh()
								body.refresh()
								tempAnswer = ""
								startTime = time.time()
								while isDonePassword == 0:
									elapsedTime = time.time() - startTime
									if  elapsedTime <= (minutes * 60):
										mins, secs = divmod( minutes - (elapsedTime*60), 60)
										timeformat = '{:02d}:{:02d}'.format(mins, secs)
										timer.clear()
										timer.addstr(timeformat)
										timer.refresh()
										body.refresh()
									else:
										isDonePassword = 2
									data = body.getch()
									if data == 263 :
										if tempAnswer != "":
											if len(tempAnswer) == 1:
												tempAnswer = ""
											else:
												tempAnswer = tempAnswer[: len(tempAnswer)-1]
											body.clear( )
											body.addstr(tempAnswer)
											body.refresh()	
										else:
											pass
									else:
										if isLetter(data) == 1:
											if letterCount < wordLength:
												letter = chr(data)
												if tempAnswer == "":
													tempAnswer = letter
													letterCount = 1 
												else:
													tempAnswer += letter
													letterCount += 1
												body.clear( )
												body.addstr(tempAnswer)
												body.refresh()	
											else:
												letterCount = 0 
												tempAnswer = letter
												body.clear( )
												body.addstr(tempAnswer)
												body.refresh()
										else:
											if (data == 10):
												if tempAnswer == userAnswer1 or tempAnswer == userAnswer2:
													#change to password section
													body.clear()
													title.clear()
													title.addstr(passTitle)
													tempAnswer = ""
													letterCount = 0
													title.refresh()
													body.refresh()
												if tempAnswer == passAnswer1 or tempAnswer == passAnswer2:
													isDonePassword = 1
													tempAnswer = ""
													letterCount = 0
												else:
													if (tempAnswer == hintCommand):
														#do hint
														hint.clear()
														hint.addstr("I shall give you pity points. Here's a Clue.")
														#hintFunction()
														hint.refresh()
													if (tempAnswer == quitCommand):
														isDonePassword = 3
													tempAnswer = ""
													curses.beep()
													body.clear()
													body.refresh()
											else:
												pass
									
								if isDonePassword == 1:
									#user and pass are correct
									hint.clear()
									hint.addstr("escapeRoom is complete")
									hint.refresh()
								else:
									if isDonePassword == 3:
										#quit partway
										hint.clear()
										hint.addstr("room was incomplete")
										hint.refresh()
									else:
										if isDonePassword == 2:
											#out of time
											hint.clear()
											hint.addstr("room ran out of time")
											hint.refresh()
										else:
											pass
								isDonePassword = 0
								title.clear()
								title.addstr( adminTitle)
								title.refresh()
							else:
								pass
								
							tempAnswer = ""
							hint.clear()
							body.clear()
							hint.refresh()
							title.refresh()
							body.refresh()
							letterCount = 0 
						else:
							pass
	except: 
		curses.nocbreak()
		body.keypad(False)
		curses.echo()
		curses.endwin()
ncursesAdmin()
