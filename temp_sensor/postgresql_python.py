import psycopg2 as pg
import socket
import time
import datetime

port = 1337

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        s.bind(('0.0.0.0', port ))
        try:
            s.listen(0)
            while True:
                c, addr = s.accept()
                c.settimeout(5)
                print("Connectado com: ",addr)
                pacote = c.recv(1024).decode()
                pacote=pacote.split(",")
                esp=pacote[0]
                location=pacote[1]
                temp=pacote[2]
                humid=pacote[3]
                c.close()
                print("esp= ",esp)
                print("location= ", location)
                print("temp= ",temp)
                print("humid= ",humid)
                
                
                try:
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
                except:
                    print("erro do postgre")
        except:
            print("Erro ao escutar")
    except:
        print("Erro ao dar bind")
except:
    print("Erro ao criar o socket")