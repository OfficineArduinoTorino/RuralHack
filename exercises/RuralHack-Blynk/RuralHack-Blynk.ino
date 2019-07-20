/*
 *  Send Soil Moisture I2C sensor 
 *  and MKR ENV Shield to Blynk 
 *  as Virtual Pins 5, 6, 8, 9, 10, 11
 *  
 */

#define BLYNK_PRINT Serial

#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

I2CSoilMoistureSensor sensor;

#include <Arduino_MKRENV.h>
// Environmental Shield Data
float t;
float h;
float p;
float l;

#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Wifi-SSID";
char pass[] = "Wifi-PASSWORD";

float soil = 0;
float temp = 0;

BlynkTimer timer;


void setup()
{
  // Debug console
  Serial.begin(9600);

  Wire.begin();
  sensor.begin(); // reset sensor
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor.getAddress(), HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor.getVersion(), HEX);
  Serial.println();

  if (!ENV.begin()) {
    Serial.println("Failed to initialize MKR ENV shield!");
    while (1);
  }

    Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sendSensor);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void sendSensor()
{
  float soil = sensor.getCapacitance();
  float temp = sensor.getTemperature();
  float h = ENV.readHumidity();
  float t = ENV.readTemperature();
  float p = ENV.readPressure();
  float l = ENV.readLux();
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, soil);
  Blynk.virtualWrite(V6, temp / 10);

  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V8, h);
  Blynk.virtualWrite(V9, t);
  Blynk.virtualWrite(V10, p);
  Blynk.virtualWrite(V11, l);


}
