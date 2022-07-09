#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <WiFiUdp.h>

#include <Adafruit_Sensor.h>
#include <DHT.h>
 
#define SSID "NotYourWiFi"
#define PASSWD "karalhodesenha"

#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
#define DHTPIN2 2
#define esp "FABIO"
#define location "Sala"
WiFiUDP udp;

String apiKeyValue = "tPmAT5Ab3j7F9";
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
unsigned long int last_time = millis();
const uint16_t port = 1337;
//const char * host = "192.168.100.4";
IPAddress IP(192,168,100,255);
String string;
int i =0 ;
float i_t = 0;
float i_h = 0;
float aux = 0;
float media_t = 0;
float media_h = 0;
WiFiClient client;
int packetsize;
char packetbuffer[197];

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
    delay(100);
    dht2.begin();
    delay(200);
    udp.begin(port);
}

void loop()
{
    
    media_t = 0;
    media_h = 0;
    i_t = 0;
    i_h = 0;
    i=0;
    packetsize = udp.parsePacket();
    if(packetsize)
    {
      Serial.print("Received packet of size: ");
      Serial.println(packetsize);
      Serial.print("From: ");
      Serial.println((IPAddress)udp.remoteIP());

      //Reading the packet
      udp.read(packetbuffer, 197);
      Serial.print("Message: ");
      Serial.println(packetbuffer);
      while(i<14)
      {
        aux = dht.readTemperature();
        Serial.println(aux);
        while(aux == NULL)
        {
          Serial.println(aux);
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
      string = String("Fabio,Escritorio,")+String(media_t)+String(",")+String(media_h);
      udp.beginPacket(IP,port);
      udp.print(string);
      udp.endPacket();
      Serial.println(string);
    }
}
