import socket

PORT = 8000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

address = ("localhost", PORT)

s.connect(address)

s.send(b"Hello")

print(s.recv(1024))

s.close()

