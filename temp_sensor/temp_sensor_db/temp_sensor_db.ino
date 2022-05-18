#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
 
#define SSID "NotYourWiFi"
#define PASSWD "karalhodesenha"

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN2 4
#define esp "FABIO"
#define location "Sala"


String apiKeyValue = "tPmAT5Ab3j7F9";
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
unsigned long int last_time = millis();
const uint16_t port = 1337;
const char * host = "192.168.100.4";
String string;
int i =0 ;
float i_t = 0;
float i_h = 0;
float aux = 0;
float media_t = 0;
float media_h = 0;
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
    dht.begin();
    dht2.begin();
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
    media_t = 0;
    media_h = 0;
    i_t = 0;
    i_h = 0;
    i=0;
    while(i<14)
    {
      aux = dht.readTemperature();
      while(aux == NULL)
      {
        aux =  dht.readTemperature();
      }
      i_t++;
      media_t=media_t+aux;
      aux = dht.readHumidity();
      while(aux == NULL)
      {
        aux = dht.readHumidity();
      }
      i_h++;
      media_h=media_h+aux;
      i++;
    }
    media_t = media_t/i_t;
    media_h = media_h/i_h;
    string = String("Fabio,Sala,")+String(media_t)+String(",")+String(media_h);
    client.print(string);
    client.stop();
    Serial.println(string);
    delay(900000);
}
