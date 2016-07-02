
#include <WiFi101.h>
#include <ArduinoCloud.h>

/////// Wifi Settings ///////
char ssid[] = "CasaJasmina";
char pass[] = "c0nn3ct3d";


// Arduino Cloud settings and credentials
const char userName[]   = "casajasmina";
const char thingName[] = "waterPump";
const char thingId[]   = "bb9eb916-6030-4216-9854-70f0ca1c8a69";
const char thingPsw[]  = "0b00d12d-1cc3-41f3-a5c7-6997688ee994";


WiFiSSLClient sslClient;


// build a new object "waterPump"
ArduinoCloudThing waterPump;

boolean pumpStatus;
int pumpPin = 3;

void setup() {
  Serial.begin (9600);

  // attempt to connect to WiFi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // unsuccessful, retry in 4 seconds
    Serial.print("failed ... ");
    delay(4000);
    Serial.print("retrying ... ");
  }

  waterPump.begin(thingName, userName, thingId, thingPsw, sslClient);
  waterPump.enableDebug();
  // define the properties
  waterPump.addProperty("open", STATUS, RW);
  waterPump.addExternalProperty("waterSensor", "humidity", FLOAT);
  pinMode(pumpPin, OUTPUT);

}

void loop() {
  waterPump.poll();
  Serial.println( waterPump.readProperty("open"));

  if (waterPump.readProperty("open") == "true") {
    pumpStatus = true;
  } else {
    pumpStatus = false;
  }
  Serial.println(pumpStatus);

  if (pumpStatus) {
    digitalWrite(pumpPin, HIGH);
    waterPump.writeProperty("open", "true");
  } else {
    digitalWrite(pumpPin, LOW);
    waterPump.writeProperty("open", "false");

  }

  delay(1000);
}





