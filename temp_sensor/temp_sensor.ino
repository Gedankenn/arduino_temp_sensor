#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
 
#define SSID "NotYourWiFi"
#define PASSWD "karalhodesenha"

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
#define esp "FABIO"
#define location "Escritorio"


String apiKeyValue = "tPmAT5Ab3j7F9";
DHT dht(DHTPIN, DHTTYPE);
unsigned long int last_time = millis();
const uint16_t port = 1337;
const char * host = "192.168.100.4";
String string;
WiFiClient client;
void setup()
{
    Serial.begin(115200);
    Serial.println("Connecting...\n");
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASSWD); // change it to your ussid and password
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
}

void loop()
{
    if (!client.connect(host, port))
    {
        Serial.println("Connection to host failed");
        delay(1000);
        return;
    }
    Serial.println("Connected to server successful!");
    string = String("Fabio,Escritorio,")+String(dht.readTemperature())+String(",")+String(dht.readHumidity());
    client.print(string);
    //client.print(esp);
    //client.print(location);
    //client.print(String(dht.readTemperature()));
    //client.print(String(dht.readHumidity()));
    Serial.print('\n');
    client.stop();
    delay(30000);
}
