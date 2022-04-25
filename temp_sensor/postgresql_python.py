import psycopg2 as pg
import socket

port = 1337

try:
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    try:
        s.bind("localhost",port)
        try:
            s.listen()
            while True:
                c, addr = s.accept()
                print("Connectado com: ",addr)
                esp = s.recv(1024).decode()
                location = s.recv(1024).decode()
                temp = s.recv(1024).decode()
                humid = s.recv(1024).decode()
                c.close()
                print(esp," ",location," ",temp," ",humid)
                
                
                try:
                    connection = pg.connect(user = "esp",
                                            password = "espBostinha",
                                            host = "192.168.10.48",
                                            port = "5432",
                                            database = "APS_BANCO")
                    
                    cursor = connection.cursor()
                    query = "INSERT INTO SensorData (sensor, localizacao, temperature, humidade) VALUES(%s, %s, %d, %d);"
                    data = (esp,location,temp,humid)
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