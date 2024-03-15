// Project : Smart Irrigation using IoT for Industry 4.0
// Program : Final Program : Uploading sensor data to ubidots & controlling water pump
// Status :  TESTED OK
// Created By : Talent Battle
// Created on Date : 27.06.2023
// Install following library : "DHT.h" by Adafruit from library manager
// Download & Install following library : https://github.com/ubidots/ubidots-esp8266
//**********************************************************************************   
#include "Ubidots.h" 
#include <DHT.h>

#define PIN D2      // Pin D2 = GPIO 4
#define DHT_TYPE DHT11  

#define SOIL_SENSOR A0
#define RELAY D1    // Pin D1 = GPIO 5

// Create class & object for DHT library
DHT dht(PIN,DHT_TYPE);

// variables for sensor values i.e. temperature, humidity & soil moisture
float t;
float h;
int s;
int count = 0;   // Optional. Not necessarily required
    
// Use your UBIDOTS & Wi-Fi Credentials Below 
const char* UBIDOTS_TOKEN = "*****************************";  // Use your ubidots token  
const char* WIFI_SSID = "**********";      // Put your Wi-Fi SSID here
const char* WIFI_PASS = "**********";      // Put your Wi-Fi password here

// Create class & object for UBIDOTS library
Ubidots ubidots(UBIDOTS_TOKEN, UBI_HTTP);   // Token & Communication Protocol 
  
void setup() 
{
  dht.begin();
  Serial.begin(115200);

  pinMode(RELAY,OUTPUT);     
  pinMode(SOIL_SENSOR,INPUT);      
  
  // Default state of relay should be OFF
  digitalWrite(RELAY,HIGH);     // LOW = ON : HIGH = OFF

  ubidots.wifiConnect(WIFI_SSID, WIFI_PASS);
  Serial.println("Cloud connection initializing...");
  delay(1000);
  Serial.println("Sensor Initializing... Please Wait....");
  delay(2000);
}

void loop() 
{
  //read sensor values
  t = dht.readTemperature();
  h = dht.readHumidity();
  s = analogRead(A0);    // value will be returned between 0 - 1023
  
  // Dry Soil = 1023
  // Wet Soil = 0 

  // 0-1023 value is technical term which will be difficult to understand for any common person. 
  // So we will Convert analog soil value into percentage i.e. 0 - 100 for easy understanding.
 
  int soil_val = map(s,1023,0,0,100);  
  // Working of above "map()" function 
  // value of "s" from 0- 1023 will be mapped into 0-100.  
  // If the soil is totally dry then it will diplay as 0% 
  // If the soil is totally wet, it will display as 100%
  // So by using above logic & "map()" function, we will get the soil moisture value between 
  // 0 - 100 % range respectively. 
  Serial.println("Temp = "+String(t)+"\t Humidity = "+String(h)+"\t soil = "+String(soil_val)+" %");
  
  // create routine for turning on & off the pump based on soil moisture value in %
  if(soil_val <= 30)                // if soil value is below OR around 30%
  {
    digitalWrite(RELAY, LOW);       // Turn on the pump. LOW = ON : HIGH = OFF
  } 
  else if (soil_val >=70)           // if soil value is above OR around 70%
  {
    digitalWrite(RELAY, HIGH);      // Turn off the pump. LOW = ON : HIGH = OFF
  }

  //Create variables for sending on server & assign them sensor returned values
  ubidots.add("temperature", t);  
  ubidots.add("humidity", h);
  ubidots.add("soil", soil_val);
 
  // initial buffer should be false to start sending data as new data
  bool bufferSent = false;
  bufferSent = ubidots.send(); // Will send data to a device label that matches the device Id

  if (bufferSent) 
  {
    count++;   // just print the count how many times data is uploaded on server
    Serial.println("Values sent by the device : Count ("+String(count)+")");
  }
  delay(5000);      // After every 5 second, value will be updated on Ubidots
}