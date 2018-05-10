import serial
import sys
from os import write
from os import path

# CLA check
if (len(sys.argv) != 3):
	print "Usage:", sys.argv[0], "<port> <filename>"
	exit(1)

# Open serial port and file
ard  = serial.Serial(sys.argv[1], 9600, timeout=10)
file = open(sys.argv[2], "rb")

size = path.getsize(sys.argv[2])

# line_val is what the program thinks the current line should be
line_val = 0
chunk = 64
data = file.read(chunk)
	
while ( len(data) > 0 ):
	write(1, "\rProgress: " + str(100*line_val/size) + "%")
	ard_val = int(ard.readline()[:-2])
	# check if ard_val and line_val are same
	# if not, then arduino messed up somewhere
	if (ard_val != line_val):
		print "Error! ard_val =", ard_val, "while line_val =", line_val
		exit(1)

	# data
	ard.write(data)
	# update line_val
	line_val += len(data)
	data = file.read(chunk)

write(1, "\rProgress: 100%\n")
print "Done"

file.close()