#!/bin/python2
import socket
UDP_IP = "192.168.42.250"
UDP_PORT = 9694
MESSAGE = input("Quelle lame faut-il allumer ? (1-4)")
if 1 < MESSAGE > 4:
    quit()
MESSAGE = "lame" + str(MESSAGE)
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
