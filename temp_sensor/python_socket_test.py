import socket
from ssl import SOL_SOCKET
import time 
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 1337 ))              
s2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s2.setsockopt(SOL_SOCKET, socket.SO_BROADCAST,1)
s2.sendto(b'testando',('192.168.100.255',1337))

m = s.recvfrom(1024)
m = s.recvfrom(1024)
m = m[0].decode('ascii')
print(m)

m = s.recvfrom(1024)
m = m[0].decode('ascii')
print(m)