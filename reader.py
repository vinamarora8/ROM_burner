import serial
import sys
from os import write
from os import path

# CLA check
if (len(sys.argv) != 4):
	print "Usage:", sys.argv[0], "<port> <filename> <size (in bytes)>"
	exit(1)

# Open serial port and file
ard  = serial.Serial(sys.argv[1], 9600, timeout=10)
file = open(sys.argv[2], "wb")
N = int(sys.argv[3])

ard_val = int(ard.readline()[:-2])
if (ard_val == 0):
	raw_input("Ready?")

ard.write("1")

for i in range(N):
	if (i%32 == 0):
		write(1, "\rProgress: " + str(int(100.*float(i)/float(N))) + "%")
	x = chr(int(ard.readline()[:-2]))
	file.write(x)

write(1, "\rProgress: 100%\n")
print "Done"

file.close()