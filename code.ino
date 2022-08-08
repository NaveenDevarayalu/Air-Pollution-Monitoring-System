#include <SDS011.h>
#include <Adafruit_AHTX0.h>
#include <ESP8266WiFi.h>
#include<ThingSpeak.h>
String apiKey = "9QYFK4UB8UPJMZRO";
const char* ssid = "Stark 4G";
const char* password = "12345666";
const char* server = "api.thingspeak.com";
float p10,p25;
int error;
SDS011 my_sds;
Adafruit_AHTX0 aht;
WiFiClient client;
void setup() {
  Serial.begin(9600);
   my_sds.begin(14,12);
 Serial.println("Adafruit AHT10/AHT20 demo!");
 if (! aht.begin()) {
 Serial.println("Could not find AHT? Check wiring");
 while (1) delay(10);
 }
  Serial.println("AHT10 or AHT20 found");
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
  }
}
void loop() {
  error = my_sds.read(&p25,&p10);
  if (! error) {
    Serial.println("P2.5: "+String(p25));
    Serial.println("P10: "+String(p10));
  }
  delay(100);
  sensors_event_t humidity, temp;
  aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data
  if (client.connect(server,80))  {   //--> "184.106.153.149" or api.thingspeak.com
   
    String postStr = apiKey;
    postStr +="&field3=";
    postStr += String(p25);
    postStr +="&field4=";
    postStr += String(p10);
    postStr +="&field1=";
    postStr+=String(temp.temperature);
    postStr+="&field2=";
    postStr+= String(humidity.relative_humidity);
    postStr += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.print("Temperature: "); 
    Serial.print(temp.temperature);
    Serial.println(" degrees C");
    Serial.print("Humidity: ");
    Serial.print(humidity.relative_humidity);
    Serial.println("% rH");
 }
  delay(500);
}
