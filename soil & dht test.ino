// Project : Smart Irrigation using IoT for Industry 4.0
// Program : Testing DHT-11 & Soil Moisture Sensor
// Status :  TESTED OK
// Created By : Talent Battle
// Created on Date : 27.06.2023
// Install following library : "DHT.h" by Adafruit from library manager
   
#include <DHT.h>

#define PIN D2      // Pin D2 = GPIO 4
#define DHT_TYPE DHT11  

#define SOIL_SENSOR A0    // Soil sensor is connected to analog 0 pin

// Create class & object for DHT library
DHT dht(PIN,DHT_TYPE);

// variables for sensor values i.e. temperature, humidity & soil moisture
float t;
float h;
int s;

void setup() 
{
  dht.begin();
  Serial.begin(9600);
  pinMode(A0,INPUT);
}

void loop() 
{
  s = analogRead(A0);             // value will be returned between 0 - 1023
    // Dry Soil = 1023
    // Wet Soil = 0 
  
  t = dht.readTemperature();      // value will be returned in degrees celcius
  h = dht.readHumidity();         // value will be returned in percentage 

  Serial.println("Temp = "+String(t)+" *C \t Humidity = "+String(h)+" % \t soil = "+String(s)+"");
  delay(500);

}
