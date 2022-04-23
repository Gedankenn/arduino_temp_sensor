#include <Arduino.h>
#define ESP8266 1
#ifdef ESP8266
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#else

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <Hash.h>

// Uncomment line below if you use WiFi shield instead of Ethernet
#define USE_ARDUINO WIFI 1
#define DHTPIN 5     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT11     // DHT 11
float t = 0.0;
float h = 0.0;

DHT dht(DHTPIN, DHTTYPE);

#ifdef USE_ARDUINO_WIFI
#include <WIFI.h>
#else
#include <Ethernet.h>
#endif

#endif
#include "SimplePgSQL.h"



IPAddress PGIP(192,168,100,124);        // your PostgreSQL server IP

const char ssid[] = "NotYourWiFi";      //  your network SSID (name)
const char pass[] = "karalhodesenha";      // your network password

const char user[] = "esp";       // your database user
const char password[] = "espBostinha";   // your database password
const char dbname[] = "APS_BANCO";         // your database name

#if defined(ESP8266) || defined(USE_ARDUINO_WIFI) || defined(ESP32)
int WiFiStatus;
WiFiClient client;
#else
#define USE_ARDUINO_ETHERNET 1
EthernetClient client;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // your mac address
byte ip[] = {192, 168, 100, 130};                    // your IP address
#endif

char buffer[1024];
PGconnection conn(&client, 0, 1024, buffer);

int pg_status = 0;

void setup(void)
{
    Serial.begin(
#ifdef __AVR__
    9600
#else
    115200
#endif
    );
#ifdef USE_ARDUINO_ETHERNET
    Ethernet.begin(mac, ip);
#else
    WiFi.begin((char *)ssid, pass);
#endif
    dht.begin();

    conn.setDbLogin(PGIP,
      user,
      password,
      dbname,
      "utf8");

    rc = conn.status();
    if (rc == CONNECTION_BAD || rc == CONNECTION_NEEDED) {
        char *c=conn.getMessage();
        if (c) Serial.println(c);
      }
    else if (rc == CONNECTION_OK) {
        Serial.println("CONNECTION OK");
      }
}

#ifndef USE_ARDUINO_ETHERNET
void checkConnection()
{
    int status = WiFi.status();
    if (status != WL_CONNECTED) {
        if (WiFiStatus == WL_CONNECTED) {
            Serial.println("Connection lost");
            WiFiStatus = status;
        }
    }
    else {
        if (WiFiStatus != WL_CONNECTED) {
            Serial.println("Connected");
            WiFiStatus = status;
        }
    }
}

#endif

static PROGMEM const char query_rel[] = "\
SELECT a.attname \"Column\",\
  pg_catalog.format_type(a.atttypid, a.atttypmod) \"Type\",\
  case when a.attnotnull then 'not null ' else 'null' end as \"null\",\
  (SELECT substring(pg_catalog.pg_get_expr(d.adbin, d.adrelid) for 128)\
   FROM pg_catalog.pg_attrdef d\
   WHERE d.adrelid = a.attrelid AND d.adnum = a.attnum AND a.atthasdef) \"Extras\"\
 FROM pg_catalog.pg_attribute a, pg_catalog.pg_class c\
 WHERE a.attrelid = c.oid AND c.relkind = 'r' AND\
 c.relname = %s AND\
 pg_catalog.pg_table_is_visible(c.oid)\
 AND a.attnum > 0 AND NOT a.attisdropped\
    ORDER BY a.attnum";

static PROGMEM const char query_tables[] = "\
SELECT n.nspname as \"Schema\",\
  c.relname as \"Name\",\
  CASE c.relkind WHEN 'r' THEN 'table' WHEN 'v' THEN 'view' WHEN 'm' THEN 'materialized view' WHEN 'i' THEN 'index' WHEN 'S' THEN 'sequence' WHEN 's' THEN 'special' WHEN 'f' THEN 'foreign table' END as \"Type\",\
  pg_catalog.pg_get_userbyid(c.relowner) as \"Owner\"\
 FROM pg_catalog.pg_class c\
     LEFT JOIN pg_catalog.pg_namespace n ON n.oid = c.relnamespace\
 WHERE c.relkind IN ('r','v','m','S','f','')\
      AND n.nspname <> 'pg_catalog'\
      AND n.nspname <> 'information_schema'\
      AND n.nspname !~ '^pg_toast'\
  AND pg_catalog.pg_table_is_visible(c.oid)\
 ORDER BY 1,2";
 
void loop()
{
#ifndef USE_ARDUINO_ETHERNET
    checkConnection();
    if (WiFiStatus == WL_CONNECTED) {
#endif
        doPg();
#ifndef USE_ARDUINO_ETHERNET
    }
#endif
    delay(30000);
}
