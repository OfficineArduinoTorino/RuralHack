int inByte = 0;
int ledPin = 6;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(6, OUTPUT);

}

void blinkLED(){
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void loop() {
   
   if (Serial1.available() > 0) {
    byte inByte = Serial1.read();
    Serial.println(inByte); //DEBUG
    blinkLED();
    delay(20);
   }

}
