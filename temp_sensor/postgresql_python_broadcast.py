import psycopg2 as pg
import socket
import time
import datetime
from ssl import SOL_SOCKET
from threading import Thread


port = 1337
s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind(('', 1337 ))              
s2 = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s2.setsockopt(SOL_SOCKET, socket.SO_BROADCAST,1)

def send_server():
    while True:
        s2.sendto(b'testando',('192.168.100.255',1338))
        time.sleep(900)

def receiv_server():
    while True:
        m = s.recvfrom(1024)
        m = m[0].decode('ascii')
        print(m)
        sdb_insert(m)

def sdb_insert(msg):
    pacote=msg
    pacote=pacote.split(",")
    esp=pacote[0]
    location=pacote[1]
    temp=pacote[2]
    humid=pacote[3]
    print("esp= ",esp)
    print("location= ", location)
    print("temp= ",temp)
    print("humid= ",humid)
    print("tempo= ",datetime.datetime.now())

    connection = pg.connect(user = "th00r",
                            password = "valhalla",
                            host = "192.168.10.48",
                            port = "5432",
                            database = "APS_BANCO")
    
    cursor = connection.cursor()
    query = "INSERT INTO SensorData (esp, localizacao, temperatura, humidade, tempo) VALUES(%s, %s, %s, %s, %s);"
    data = (esp,location,temp,humid,datetime.datetime.now())
    cursor.execute(query,data)
    connection.commit()
    cursor.close()
    connection.close()
    print("inserted")

t1 = Thread(target=send_server)
t2 = Thread(target=receiv_server)

t1.start()
t2.start()