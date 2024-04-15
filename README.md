# Arduino Temp Sensor With Postgres and Grafana
A simple project to read temperature from a sensor and store it in a postgres database. The data is then displayed in a grafana dashboard.

## Requirements:
- ESP8266
- DHT22 Sensor
- Postgres
- Grafana

## Setup:
1. Install the required libraries:
```
$ arduino-cli lib install "DHT sensor library"
$ arduino-cli lib install "Adafruit Unified Sensor"

```

2. Create a database in postgres:
```
$ psql
$ CREATE DATABASE temp_sensor;
$ \c temp_sensor
$ CREATE TABLE temp_data (id SERIAL PRIMARY KEY, temperature FLOAT, humidity FLOAT, created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP);
```

3. Update the `config.h` file with your wifi and database credentials.

4. Upload the code to the ESP8266.

5. Create a new datasource in Grafana with the following settings:
```
Name: temp_sensor
Type: Postgres
Host: <host>
Database: temp
User: <user>
Password: <password>
```

6. Create a new dashboard in Grafana with a new graph panel. Set the datasource to the one created in the previous step and the query to:
```
SELECT
  $__time(created_at),
  temperature
FROM temp_data
```

7. Enjoy your new dashboard!

## ESP8266 Pinout:
- DHT11 Sensor:
  - VCC -> 3.3V
  - GND -> GND
  - DATA -> GPIO5


![pinout](esp8266.png)

## Graficos com resultados:
![grafana](grafana.png)