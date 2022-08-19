import socket
from ssl import SOL_SOCKET
import time
from threading import Thread


s2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s2.setsockopt(SOL_SOCKET, socket.SO_BROADCAST,1)

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 1337 ))

def send_server():
    while True:
        s2.sendto(b'testando',('192.168.100.255',1338))
        time.sleep(10)

def receiv_server():
    while True:
        m = s.recvfrom(1024)
        m = m[0].decode('ascii')
        print(m)


t1 = Thread(target=send_server)
t2 = Thread(target=receiv_server)

t1.start()
t2.start()