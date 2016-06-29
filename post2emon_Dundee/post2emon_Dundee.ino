/*

  Example Data to Post Sensor Data do EmonCMS

  This Example is based on
  - Connect to WPA ezample by Arduino.cc
  - Emoncms library by Giacomo Leonzi
  - Edits and

*/
#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "Agrigator";      //  your network SSID (name)
char pass[] = "hamradio";       // your network password
int status = WL_IDLE_STATUS;    // the Wifi radio's status

char host[] = "192.168.2.100";  //installation of the platform emoncms.org
const char* APIKEY = "26fbc95fa1e96990b6dd6541e59c22a3";


#define HTTPPORT 80          // change this value to point at a local


WiFiClient client;


// set up net client info:
const unsigned long postingInterval = 6000;  //delay between updates to emoncms.com
unsigned long lastRequest = 0;      // when you last made a request


void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");

  // http://192.168.2.100/emoncms/input/post.json?node=1&json={power:23,data2:93}&apikey=26fbc95fa1e96990b6dd6541e59c22a3

  //printCurrentNet();
  //printWifiData();

}

void loop() {
  // check the network connection once every 10 seconds:
  delay(10000);
  printCurrentNet();

  int data1 = int(random(0, 30));     //replace this value with your
  int data2 = int(random(50, 100));   //measure
  long now = millis();
  if (now - lastRequest >= postingInterval) {
    String Data = "power:";
    Data += data1;
    post2emon(String(4), Data);
    //emoncms.addValue("data2", data2);
    // Serial.println(emoncms.publish());  //open the connection and pubblish
    lastRequest = now;
  }
  delay(10);


}

void post2emon(String nodes, String Data) {

  String _APIKEY = APIKEY;
  String node = nodes;
  String DataIn = Data;
  //String  data = "POST http://192.168.2.100/emoncms/input/post.json?node=1&json={altro1:10,altro2:10}&apikey=26fbc95fa1e96990b6dd6541e59c22a3 HTTP/1.1";


  //begin to build the string to send
  String _url = "POST /input/post.json?node=";
  _url += node;
  _url += "&json={";
  _url += Data;
  _url += "}&apikey=";
  _url += String(APIKEY);
  _url += String(" HTTP/1.1");
  //Serial.println(_url);
  //ended to build the string to send



  while (!client.connected()) {
    Serial.println("reconnecting");
    client.connect(host, HTTPPORT);
    delay(2000);
  }

  String data = _url;

  if (client.connect(host, HTTPPORT)) {
    //client.println(data);
    client.println("Host: 192.168.2.100");
//    client.println("Content-Type: application/x-www-form-urlencoded\n");
   client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.println(data.length());
    client.println();
    client.print(data);

    client.stop();  // DISCONNECT FROM THE SERVER
    Serial.println(data);

    Serial.println("IN");
    delay(2000);
    char c = client.read();
    Serial.println(c);



  }
  else {
    Serial.println("FAILED");
  }
  data = ""; //clear the string with data ready for the next lecture




}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);

}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

