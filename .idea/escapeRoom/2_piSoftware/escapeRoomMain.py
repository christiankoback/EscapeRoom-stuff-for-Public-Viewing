import threading, Queue, bluetooth, time
from sys import exit
import serial

# BEGIN BLUETOOTH ######################################################

def bluetoothFunction(incoming, toLaser, toMagnet, toMainCube, toNfcCube, stop_event):
    MainCube_BT = serial.Serial("/dev/rfcomm3", 9600)
    nfc_BT = serial.Serial("/dev/rfcomm1", 9600)
    magnet_BT= serial.Serial("/dev/rfcomm2",9600)
    laser_BT= serial.Serial("/dev/rfcomm4",9600)

    while not stop_event.wait(1):
        # print 'doing bluetooth'

        # Check MainCube for codes
        MainCube_BT.write("\n")  # request an update
        data = MainCube_BT.readline()
        # print 'data>'   + data+ '<'
        if data.strip() != "!":
            incoming.put(data.strip())
        # print 'inserted>' + data.strip() + '<'

        # Check nfc_BT for codes
        nfc_BT.write("\n")  # request an update
        data = nfc_BT.readline()
        # print 'data>'   + data+ '<'
        if data.strip() != "!":
            incoming.put(data.strip())
        # print 'inserted>' + data.strip() + '<'

        # if something in the queue send it
        while not toMainCube.empty():
            sendItem = toMainCube.get()
            print 'sending to mainCube>' + sendItem + '<'
            MainCube_BT.write(sendItem)

        while not toNfcCube.empty():
            sendItem = toNfcCube.get()
            print 'sending to NFC>' + sendItem + '<'
            nfc_BT.write(sendItem)

    print 'bluetooth stopping'


# END OF BLUETOOTH #######################################################


# BEGIN OF LOGIC #######################################################

def logicFunction(incoming, toLaser, toMagnet, toMainCube, toNfcCube, stop_event):
    while not stop_event.wait(1):
        # print 'logic 2 sec'
        if not incoming.empty():
            print "Received>" + incoming.get() + "<"
            # time.sleep(2)
    print 'logic stopping'


# END OF LOGIC ############################################################

pill2kill = threading.Event()

incoming = Queue.Queue()
toLaser = Queue.Queue()
toMagnet = Queue.Queue()
toMainCube = Queue.Queue()
toNfcCube = Queue.Queue()

bluetoothThread = threading.Thread(target=bluetoothFunction,
                                   args=(incoming, toLaser, toMagnet, toMainCube, toNfcCube, pill2kill))
logicThread = threading.Thread(target=logicFunction,
                               args=(incoming, toLaser, toMagnet, toMainCube, toNfcCube, pill2kill))

print 'Starting Threads'
bluetoothThread.start()
logicThread.start()

while 1:
    tosend = raw_input()
    if tosend == "qq":
        toNfcCube.put("11\n")
    if tosend != "q":
        toMainCube.put(tosend + "\n")
    else:
        pill2kill.set()
        print 'killing threads'
        bluetoothThread.join()  # waits kill that thread is dead (busy wait)
        print 'killed Bluetooth Thread'
        logicThread.join()
        print 'killed LogicThread'
        exit(0)

