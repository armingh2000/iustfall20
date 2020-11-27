import socket

PORT = 8000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

address = ("localhost", PORT)

s.bind(address)

s.listen(5)

s2, addr = s.accept()

print(s2.recv(1024))
s2.send(b"Hello")

s.close()

