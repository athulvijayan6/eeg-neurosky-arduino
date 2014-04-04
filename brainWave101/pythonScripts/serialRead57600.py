import serial,time
import re
import termios, sys, os

try:
	arduino = serial.Serial("/dev/ttyACM0", 57600, timeout=1)
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

dataFile = open('eyeBlink9600.csv', 'a')
arduino.flushInput()

print ("press R for recording a dataset. press E to exit")

while(1):
	key = getkey()
	if (key == 'r'):
		arduino.write('R')
		while(arduino.readline()[0:-2] != 'S'):
			pass
		rawValue = arduino.readline()[0:-1]
		if (arduino.readline()[0:-2] == 'E'):
				dataFile.write(rawValue+'\n')
				print ("Last reading is  -->  " +rawValue)

	if (key == 'e'):
		sys.exit()
