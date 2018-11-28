
// This header is used to tell the Arduino IDE
// to compile with the proper WIFI Library 
// between MKR1000 and MKR1010

#ifdef ARDUINO_SAMD_MKRWIFI1010
#include <WiFiNINA.h>
#elif ARDUINO_SAMD_MKR1000
#include <WiFi101.h>
#else
#error unsupported board
#endif

#include <SPI.h>

char ssid[] = "YourWifi"; //  your network SSID (name)
char pass[] = "YourPassword";
int keyIndex = 0;            // your network key Index number (needed only for WEP)


String location= "YourLocation";
String apiKey= "YourApiKey";


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
    client.print("q="+location);
    client.print("&apiKey="+apiKey);
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
  }
}
