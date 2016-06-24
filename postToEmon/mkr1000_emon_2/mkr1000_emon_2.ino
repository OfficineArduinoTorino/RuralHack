#include <SPI.h>
#include <WiFi101.h>
#include <Emoncms.h>


WiFiClient client;

char ssid[] = "Agrigator"; //  your network SSID (name)
char pass[] = "hamradio";    // your network password (use for WPA, or use as key for WEP)

int status = WL_IDLE_STATUS;


//#define APIKEY        "26fbc95fa1e96990b6dd6541e59c22a3" // replace your emoncms apikeywrite here
//#define HTTPPORT 80          // change this value to point at a local
//#define HOST "http://192.168.2.100/emoncms" //installation of the platform emoncms.org

String APIKEY = "26fbc95fa1e96990b6dd6541e59c22a3";    // replace your emoncms apikeywrite here
int HTTPPORT = 80;          // change this value to point at a local
String HOST = "http://192.168.2.100/emoncms"; //installation of the platform emoncms.org


//  http://192.168.2.100/emoncms/input/post.json?node=1&json={power:200}&apikey=26fbc95fa1e96990b6dd6541e59c22a3


// set up net client info:
const unsigned long postingInterval = 6000;  //delay between updates to emoncms.com
unsigned long lastRequest = 0;      // when you last made a request


Emoncms emoncms;  //Initialize the object

void setup() {
  Serial.begin(9600);
  Serial.print(HOST);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println("Connected to wifi");
  emoncms.begin(HOST, HTTPPORT, APIKEY, client); //Initialize the object
}

void loop() {

  int data1 = int(random(0, 30));     //replace this value with your
  int data2 = int(random(50, 100));   //measure
  long now = millis();
  if (now - lastRequest >= postingInterval) {
    emoncms.addValue("power", data1);
    emoncms.addValue("data2", data2);
    Serial.println(emoncms.publish());  //open the connection and pubblish
    lastRequest = now;
  }
  delay(10);
}
