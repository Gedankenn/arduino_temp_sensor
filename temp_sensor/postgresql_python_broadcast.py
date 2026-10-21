import os
import psycopg2 as pg
from dotenv import load_dotenv
import socket
import time
import datetime
from ssl import SOL_SOCKET
from threading import Thread

load_dotenv()  # PG_* settings from temp_sensor/.env (gitignored)


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
    pacote=pacote.strip().split(",")
    # The sketch sends "<id>,<esp>,<location>,<temperature>,<humidity>";
    # read the last four fields so the columns line up with SensorData.
    esp=pacote[-4]
    location=pacote[-3]
    temp=pacote[-2]
    humid=pacote[-1]
    print("esp= ",esp)
    print("location= ", location)
    print("temp= ",temp)
    print("humid= ",humid)
    print("tempo= ",datetime.datetime.now())

    connection = pg.connect(user = os.environ["PG_USER"],
                            password = os.environ["PG_PASSWORD"],
                            host = os.environ["PG_HOST"],
                            port = os.environ.get("PG_PORT", "5432"),
                            database = os.environ.get("PG_DATABASE", "APS_BANCO"))
    
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