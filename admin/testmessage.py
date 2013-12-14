import socket

UDP_IP = "127.0.0.1"
UDP_PORT = 1234
MESSAGE = "HI ADMIN"

MESSAGE = bytes(MESSAGE, "utf-8")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
