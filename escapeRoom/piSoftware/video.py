from omxplayer import OMXPlayer
from time import sleep
#test git from rpi
#omxplayer --win "50 50 1390 1030" file.mp4 # how to run things from the terminal

file_path_or_url = 'file.mp4'

player = OMXPlayer(file_path_or_url, args=['--alpha','150'])# from 0 to 255 transparency


#player.pause()
sleep(3) 

#set_alpha(0.25)

player.quit()
