#include <ArduinoJson.h>


#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "IncartataWiFi"; //  your network SSID (name)
char pass[] = "bosconauta2000";
int keyIndex = 0;            // your network key Index number (needed only for WEP)


String location = "galvanico";
String apiKey = "b3bdd8d39bb4fca6062cac3701101f41";


int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.openweathermap.org";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(1000);
  }
  Serial.println("Connected to wifi");


}

void loop() {
  getWeather();
  delay(5000);

}


void getWeather() {

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.print("GET /data/2.5/weather?");
    client.print("q=" + location);
    client.print("&apiKey=" + apiKey);
    client.println("&units=metric");

    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();
  } else {
    Serial.println("unable to connect");
  }

  delay(1000);
  String line = "";

  while (client.connected()) {
    line = client.readStringUntil('\n');
    Serial.println(line);

    Serial.println("parsingValues");
    
    StaticJsonBuffer<5000> jsonBuffer;

    JsonObject& root = jsonBuffer.parseObject(line);
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return;
    }

    String temperature = root["main"]["temp"];

    // Print values.
    Serial.println(temperature);

  }
}





