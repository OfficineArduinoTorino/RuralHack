#include <I2CSoilMoistureSensor.h>
#include <Wire.h>
#include <WiFi101.h>
#include <ArduinoCloud.h>

I2CSoilMoistureSensor sensor;
int sensorPin = A0;


/////// Wifi Settings ///////
char ssid[] = "officine1";
char pass[] = "OfficineRulez";


const char userName[]   = "casajasmina";
const char thingName[] = "watersensor";
const char thingId[]   = "4606ce19-b6f2-4aec-9b0c-c8cf4037a3cd";
const char thingPsw[]  = "87995808-225e-45d4-95b7-25bda4b622d7";



WiFiSSLClient sslClient;


// build a new object "watersensor"
ArduinoCloudThing watersensor;




void setup() {
  pinMode(6, OUTPUT);

  //sensorInit();
  Wire.begin();
  Wire.setClock(50000);
  Serial.begin(9600);
  sensor.begin(); // reset sensor
  Serial.println(" sensor started");
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor.getAddress(), HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor.getVersion(), HEX);
  Serial.println();


  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  if (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");

  }


  watersensor.begin(thingName, userName, thingId, thingPsw, sslClient);
  watersensor.enableDebug();
  // define the properties
  watersensor.addProperty("humidity", FLOAT, R);
  watersensor.addProperty("BatteryValue", FLOAT, R);

  //m2m_wifi_set_sleep_mode(M2M_PS_DEEP_AUTOMATIC, true);
}

void loop() {

  if (!WL_CONNECTED) {
    WiFi.begin(ssid, pass) ;
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
  }

  digitalWrite(6, HIGH);
  delay(1000);
  digitalWrite(6, LOW);
  delay(1000);
  watersensor.poll();
  int humidity = sensor.getCapacitance();
  int sensorValue = analogRead(sensorPin);
  float voltage = sensorValue * (4.3 / 1023.0);
  Serial.print("Soil Moisture Capacitance: ");
  Serial.println(humidity); //read capacitance register
  Serial.print(voltage);
  Serial.println("V");
  watersensor.writeProperty("humidity", humidity);
  watersensor.writeProperty("BatteryValue", voltage);
  delay(1000);

  for (int a = 0; a < 10; a++) {
    digitalWrite(6, HIGH);
    delay(1000);
    digitalWrite(6, LOW);
    delay(1000);
  }




}

void sensorInit() {
  pinMode(SDA, INPUT_PULLUP);
  pinMode(SCL, OUTPUT);
  for (int i = 0; i < 100; i++) {
    digitalWrite(SCL, HIGH);
    digitalWrite(SCL, LOW);
  }

}

