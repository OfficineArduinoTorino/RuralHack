/*******************************************************************

      A code that offers all the sensor data taken from our setup and 
      brings them to Telegram

      Shared by Luda Delucchi during Muse RuralHack of November
      Minor edits by Davide Gomba
    
 *******************************************************************/

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


#include <WiFiSSLClient.h>
#include <UniversalTelegramBot.h>
#include "DHT.h"
#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

#include "arduino_secrets.h"

I2CSoilMoistureSensor sensor;

#define DHTPIN 5
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);
int sensorPin = A0;    // select the input pin for the potentiometer
// Initialize Wifi connection to the router

char ssid[] = SECRET_SSID; // your network SSID (name)
char password[] = SECRET_PASS;  // your network key

String BOTtoken = BOT_TOKEN; // your Bot Token (Get from Botfather)

const int ledPin = 3;
const int relay = 8;
float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux

String mychatid = CHAT_ID; //my chat id

WiFiSSLClient client;

UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

bool Start = false;
int ledStatus = 0;

int humiHigh = 0;
int humiLow = 0;
float t;
float soil;
float h;
float logLux;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i = 0; i < numNewMessages; i++) {

    String chat_id = String(bot.messages[i].chat_id);
    if (chat_id != mychatid){
      bot.sendMessage(chat_id, "You cannot use this bot", "");
      return;
    }
    //Serial.println(chat_id);
    String text = bot.messages[i].text;
    if (text == "/ledon") {
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      bot.sendMessage(chat_id, "Led is ON", "");
    }

    if (text == "/ledoff") {
      ledStatus = 0;
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(chat_id, "Led is OFF", "");
    }

    if (text == "/status") {
      if (ledStatus) {
        bot.sendMessage(chat_id, "Led is ON", "");
      } else {
        bot.sendMessage(chat_id, "Led is OFF", "");
      }
    }

    if (text == "/start" || text == "/help") {
      String from_name = bot.messages[i].from_name;
      if (from_name == "") from_name = "Anonymous";
      String welcome = "Welcome, " + from_name + ", from FlashLedBot, your personal Bot on Arduino 101\n";
      welcome = welcome + "/ledon : to switch the Led ON \n";
      welcome = welcome + "/ledoff : to switch the Led OFF \n";
      welcome = welcome + "/status : Returns current status of LED \n";
      welcome = welcome + "/help : Show the complete command list \n";
      welcome = welcome + "/temperature : Returns current value Temperature \n";
      welcome = welcome + "/humidity : Returns current Humidity value \n";
      welcome = welcome + "/light : Returns current LUX value \n";
      welcome = welcome + "/soilmoisture : Returns value of soil moisture \n";
      welcome = welcome + "/data : Returns values of all sensors";
      bot.sendMessage(chat_id, welcome, "Markdown");

    }

    if (text == "/Temperature" || text == "/temperature") {
      bot.sendMessage(chat_id, "Temperature " + String(t)+ "°C", "");
    }
    if (text == "/Humidity" || text == "/humidity") {
      bot.sendMessage(chat_id, "Humidity " + String(h)+"%", "");
    }

    if (text == "/Light" || text == "/light") {
      bot.sendMessage(chat_id,  String(logLux) +  " Lumen" , "");
    }
    if (text == "/soilmoisture" || text == "/SoilMoisture") {
      bot.sendMessage(chat_id, "Soil moisture " + String(soil), "");
    }
    if (text == "/data" || text == "/Data") {
      String data = "Temperature " + String(t)+ "°C \n";
      data = data + "Humidity " + String(h)+"% \n";
      data = data + "Soil moisture " + String(soil) + " \n";
      data = data + "Light " + String(logLux) +  " Lumen \n";
      bot.sendMessage(chat_id, data, "Markdown");
    }

  }

}

void setup() {
    Wire.begin();
    sensor.begin(); // reset sensor
    Serial.begin(115200);
    dht.begin();
    delay(100);

    // attempt to connect to Wifi network:
    Serial.print("Connecting Wifi: ");
    Serial.println(ssid);

    while (WiFi.begin(ssid, password) != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
    }
    while (sensor.isBusy()) delay(50); // available since FW 2.3

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
    pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
    delay(10);
    digitalWrite(ledPin, LOW); //initialize pin as off
    pinMode(relay, OUTPUT); // initialize digital relay pin as an output.
}

void readData(){
  while (sensor.isBusy()) delay(100); // available since FW 2.3
  soil = float(sensor.getCapacitance());
  t = dht.readTemperature();
  h = dht.readHumidity();
  int rawValue = analogRead(sensorPin);
  logLux = pow(10, (rawValue * logRange / rawRange));
}

void loop() { 
  delay(1000);
  readData();
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    Bot_lasttime = millis();
  }
  if (t > 25.0) {
    bot.sendMessage(mychatid, "Temperature " + String(t)+ "°C");
  }
  if (soil < 190){
    digitalWrite(relay, HIGH);
    digitalWrite(ledPin, HIGH);
    ledStatus = 1;
    if (humiLow == 0){
      bot.sendMessage(mychatid, "Humidity " + String(soil) + ", Relay on", "");
    }
    humiLow = 1;
    humiHigh = 0;
  }
  if (soil > 210){
    digitalWrite(relay, LOW);
    digitalWrite(ledPin, LOW);
    ledStatus = 0;
    if (humiHigh == 0){
      bot.sendMessage(mychatid, "Humidity " + String(soil) + ", Relay off", "");
    }
    humiLow = 0;
    humiHigh = 1;
  }
}
