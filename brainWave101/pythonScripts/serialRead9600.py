import serial
import re
import termios, sys, os

try:
	arduino = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
	time.sleep(5)
except:
	print ('cannot open serial port. check serialport in above command')
	sys.exit()

def getkey():
    term = open("/dev/tty", "r")
    fd = term.fileno()
    old = termios.tcgetattr(fd)
    new = termios.tcgetattr(fd)
    new[3] &= ~termios.ICANON & ~termios.ECHO
    termios.tcsetattr(fd, termios.TCSANOW, new)
    c = None
    try:
        c = os.read(fd, 1)
    finally:
        termios.tcsetattr(fd, termios.TCSAFLUSH, old)
        term.close()
    return c

dataFile = open('/home/athul/matlab/eyeBlink57600.csv', 'a')
arduino.flushInput()

print ("press R for recording a dataset. press E to exit")

while(1):
	key = getkey()
	if (key == 'r'):
		arduino.write('R')
		while(arduino.readline()[0:-2] != 'S'):
			pass
		stringData = re.sub("[^0-9^,]","", arduino.readline())
		arduino.readline()
		if (arduino.readline()[0:-2] != 'E'):
			print ("Signal Strength ------> "+stringData.split(',')[0])
			if (int(stringData.split(',')[0]) == 0):
				print (stringData.split(','))
				dataFile.write(stringData+'\n')
			else:
				print "Signal was poor at that moment. try again"				
			
	if (key == 'e'):
		sys.exit()
