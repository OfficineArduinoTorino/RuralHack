/*******************************************************************

    This code merges the Telegram Example and The Thingspeak Example together.
    Shared by Aldo Biasi during Muse RuralHack of November

    Modified and adapted to MKR1000 and other minors stuff by Davide Gomba in Late Nov '18
    
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
#include "arduino_secrets.h"
#include <I2CSoilMoistureSensor.h>
#include <Wire.h>

I2CSoilMoistureSensor sensor;
#define DHTPIN 5
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
int sensorPin = A0;    // select the input pin for the potentiometer


// Initialize Wifi connection to the router
char ssid[] = SECRET_SSID; // your network SSID (name)
char password[] = SECRET_PASS;  // your network key
String BOTtoken = BOT_TOKEN; // your Bot Token (Get from Botfather)

// you need to know the chat_id or number 
// of your conversation with the robot
// in order to talk to him

String Mychat_id = CHAT_ID; 

String apiKey = API_KEY; // api from ThingSpeak 

int pushbutton = 0;
const int ledPin = 3;
float rawRange = 1024; // 3.3v
float logRange = 5.0; // 3.3v = 10^5 lux

WiFiSSLClient client; //client SSL per connessione a BOT Telgram
WiFiClient clientTS;  //client 

UniversalTelegramBot bot(BOTtoken, client);

int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done
bool Start = false;
int ledStatus = 0;

char server[] = "api.thingspeak.com";   // name address for Google (using DNS)

float capacityI2C;
float temperatureI2C;
float lightI2C;
int stato_rele=0;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    Serial.println(chat_id);
    String text = bot.messages[i].text;
    if (text == "/ledon") {
      digitalWrite(ledPin, HIGH);   // turn the LED on (HIGH is the voltage level)
      ledStatus = 1;
      bot.sendMessage(Mychat_id, "Led is ON", "");
    }
    if (text == "/ledoff") {
      ledStatus = 0;
      digitalWrite(ledPin, LOW);    // turn the LED off (LOW is the voltage level)
      bot.sendMessage(Mychat_id, "Led is OFF", "");
    }
    if (text == "/status") {
    if (ledStatus) {
        bot.sendMessage(Mychat_id, "Led is ON", "");
      }
      else {
        bot.sendMessage(Mychat_id, "Led is OFF", "");
      }
    }

    if (text == "/start") {
      String from_name = bot.messages[i].from_name;
      if (from_name == "") from_name = "Anonymous";
      String welcome = "Welcome, " + from_name + ", from FlashLedBot, your personal Bot on Arduino 101\n";
      welcome = welcome + "/ledon : to switch the Led ON \n";
      welcome = welcome + "/ledoff : to switch the Led OFF \n";
      welcome = welcome + "/status : Returns current status of LED \n";
      welcome = welcome + "/list : Show the complete command list \n";
      bot.sendMessage(Mychat_id, welcome, "Markdown");
    }
    if (text == "/Temperature" || text == "/temperature") {
      float t = dht.readTemperature();
      bot.sendMessage(Mychat_id, "Temperature " + String(t) + "°C");
    }
    if (text == "/Humidity" || text == "/humidity") {
      float h = dht.readHumidity();
      bot.sendMessage(Mychat_id, " Humidity " + String(h) + "%");
    }

    if (text == "/list" || text == "/help") {
      String from_name = bot.messages[i].from_name;
      String list = "Hi, " + from_name + ", this is what can I do for you \n";
      list = list + "/ledon : to switch the Led ON \n";
      list = list + "/ledoff : to switch the Led OFF \n";
      list = list + "/status : Returns current status of LED \n";
      list = list + "/temperature : Returns current value Temperature\n";
      list = list + "/humidity : Returns current Humidity value\n";
      list = list + "/light : Returns current LUX value\n";
      bot.sendMessage(Mychat_id, list, "Markdown");
    }

    if (text == "/Light" || text == "/light") {
      int rawValue = analogRead(sensorPin);
      float logLux = rawValue * logRange / rawRange;
      logLux = pow(10, logLux);
      bot.sendMessage(Mychat_id,  String(logLux) +  " Lumen"  );
    }
    if (text == "/releON") {
      digitalWrite(8, HIGH);
      bot.sendMessage(Mychat_id,  "Accensione RELE"  );
    }
    if (text == "/releOFF") {
      digitalWrite(8, LOW);
      bot.sendMessage(Mychat_id,  "Spegnimento RELE" );
    }
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin(); 
  delay(100);
  pinMode(0, INPUT);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  Wire.begin();
  sensor.begin(); // reset sensor
  delay(1000); // give some time to boot up
  Serial.print("I2C Soil Moisture Sensor Address: ");
  Serial.println(sensor.getAddress(), HEX);
  Serial.print("Sensor Firmware version: ");
  Serial.println(sensor.getVersion(), HEX);
  Serial.println();
  // attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  delay(10);
  digitalWrite(ledPin, LOW); //initialize pin as off
}

void loop() {
  //lettura sensore capacitivo I2C
  while (sensor.isBusy()) delay(50); // available since FW 2.3
  Serial.print("Soil Moisture Capacitance: ");
  capacityI2C=sensor.getCapacitance();
  Serial.print(capacityI2C); //read capacitance register
  Serial.print(", Temperature: ");
  temperatureI2C=sensor.getTemperature() / (float)10;
  Serial.print(temperatureI2C); //temperature register
  Serial.print(", Light: ");
  lightI2C=sensor.getLight(true);
  Serial.println(lightI2C); //request light measurement, wait and read light register
  sensor.sleep(); // available since FW 2.3

  //lettura sensore DHT22
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  Serial.print("Temperature DHT 22: ");
  Serial.println(t);
  Serial.print("Humidity DHT 22: ");
  Serial.println(h);

  //lettura ga1a
  int rawValue = analogRead(sensorPin);
  float logLux = rawValue * logRange / rawRange;
  logLux = pow(10, logLux);
  Serial.print("Lux Ga1a: ");
  Serial.println(logLux);

  //accensione RELE, LED e INVIO MESSAGGIO SU BOT
  if (t > 25) {
    //invio messaggio su bot telegram
    bot.sendMessage(Mychat_id, "ALLARME TEMPERATURA");
    //accensione RELE per simulare accensione di un carico su rele a seguito della temperatura
    digitalWrite(8, HIGH);
    //accensione led su pin3
    digitalWrite(3, HIGH);
    stato_rele=1;
  }
  else   {
    //spegnimento RELE per simulare spegnimento di un carico su rele
    digitalWrite(8, LOW);
    //accensione led su pin3
    digitalWrite(3, LOW);
    stato_rele=0;
  }

  //lettura messaggi dal bot
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
  
  //invio dati su ThingSpeak

  


  Serial.println("\nStarting connection to server...");

  // if you get a connection, report back via serial:
  if (clientTS.connect(server, 80)) {
    Serial.println("connected to server");
    clientTS.print(F("POST "));
    clientTS.print("/update?key=apiKey&field1="
                 +               (String) t
                 +  "&field2=" + (String) h
                 +  "&field3=" + (String) logLux
                 +  "&field4=" + (String) capacityI2C
                 +  "&field5=" + (String) temperatureI2C
                 +  "&field6=" + (String) lightI2C
                 +  "&field7=" + (String) stato_rele
                );
    String tsData = "field1="   //need the length to give to ThingSpeak
                    +             (String)  t
                    +  "&field2=" + (String) h
                    +  "&field3=" + (String) logLux
                    +  "&field4=" + (String) capacityI2C
                    +  "&field5=" + (String) temperatureI2C
                    +  "&field6=" + (String) lightI2C
                    +  "&field7=" + (String) stato_rele
                    ;
    clientTS.print("POST /update HTTP/1.1\n");
    clientTS.print("Host: api.thingspeak.com\n");
    clientTS.print("Connection: close\n");
    clientTS.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    clientTS.print("Content-Type: application/x-www-form-urlencoded\n");
    clientTS.print("Content-Length: ");
    clientTS.print(tsData.length());  //send out data string legth to ts
    clientTS.print("\n\n");
    clientTS.print(tsData);
    clientTS.stop();
    delay(1000);
  }

  Serial.println("---------------------------------------");
}
