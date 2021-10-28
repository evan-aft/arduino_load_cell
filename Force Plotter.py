# in order to run the following code, use pip install to install the following libraries:
#'pyserial', 'keyboard', 'plotly', 'pandas'
# written for python 3.9.7

#change the following string to reflect the port that the arduino is connected to: (may take a different format for different OS's)
port = 'COM3'

#to use this code:
#connect to an arduino that is printing a value to plot over the serial bus at 9600 baud
#run this script to begin the recording
#when done recording, press 'e' in the terminal to stop the recording and plot using plotly
#plotly can be a little fucky, if the plot doesn't show up refresh the page in the browser
#raw data is stored in a txt file of the current time

import os
import pdb
import string
import time
import serial
import keyboard
import plotly.express as px

ser = serial.Serial('COM3', 9600)
ser.readline(10000) #make sure the arduino is connected, ignore the first value

startTime = time.time()

times = []
pressures = []

while True:
	times.append(time.time() - startTime)
	pressures.append(float(ser.readline(10000).decode('utf-8')))
	print(pressures[-1])
	if keyboard.is_pressed('e'):
		break

fig = px.scatter(x = times, y = pressures)
fig.show()

file = open(time.strftime("%Y-%m-%d_%H-%M-%S.txt", time.gmtime()), 'w')
for i in range(0, len(times)):
	file.write(str(times[i]) + ',' + str(pressures[i]) + "\n")
file.close()