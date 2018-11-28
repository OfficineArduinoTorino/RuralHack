/*******************************************************************

    This sketch is a combination of ADAFruits DHT sketch, WiFi101 Webclient
    and The arduino example from ThingSpeak
    Modified by Stephen Borsay for the MKR1000, feel free to use

    Modified and adapted to MKR1010 and other minors stuff by Davide Gomba in Late Nov '18

 *******************************************************************/

#include <SPI.h> //you don't need this as we arn't using the shiled just chip
#include <WiFiNINA.h>
#include "DHT.h"
#include "arduino_secrets.h"
#define DHTPIN 5    // what pin we're connected to

// Uncomment whatever DHT sensor type you're using!
//#define DHTTYPE DHT11  // DHT 11
//#define DHTTYPE DHT21  // DHT 21
#define DHTTYPE DHT22  // DHT 22

DHT dht(DHTPIN,DHTTYPE);

String apiKey = API_KEY; // api from ThingSpeak

char ssid[] = SECRET_SSID; //  your network SSID (name)
char pass[] = SECRET_PASS;    //your network password
int keyIndex = 0;     // your network key Index number (needed only for WEP)

//#define WEBSITE "api.thingspeak.com"

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.thingspeak.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) 
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    //Connect to WPA/WPA2 network.Change this line if using open/WEP network
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  
  Serial.println("Connected to wifi");
  printWifiStatus();
  
}

void loop() {

   // Wait a few seconds between measurements.
  delay(10000);

  //prefer to use float, but package size or float conversion isnt working
  //will revise in future with a string fuction or float conversion function

  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  int f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  int hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  int hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F\n");

    Serial.println("\nStarting connection to server..."); 
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");


          client.print(F("POST "));
          client.print("/update?key=apiKey&field1=" 
          +               (String) h
          +  "&field2=" +(String) t
          +  "&field3=" +(String) f
          +  "&field4=" +(String) hic
          +  "&field5=" +(String) hif
                                   );
                                      
          String tsData = "field1="   //need the length to give to ThingSpeak
          +             (String)  h
          +  "&field2=" +(String) t
          +  "&field3=" +(String) f
          +  "&field4=" +(String) hic
          +  "&field5=" +(String) hif
        ; 


          client.print("POST /update HTTP/1.1\n");  
          client.print("Host: api.thingspeak.com\n");
          client.print("Connection: close\n");
          client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
          client.print("Content-Type: application/x-www-form-urlencoded\n");
          client.print("Content-Length: ");
          client.print(tsData.length());  //send out data string legth to ts
          client.print("\n\n");
          client.print(tsData);
          client.stop();
          delay(1000);
    } 

}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
