int inByte = 0;
int ledPin = 6;

int pitches[] = {784, 880, 1047, 1319, 1568};

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

void playTone(char inByte){
  tone(4, pitches[(int) inByte-48], 500);
 }

void loop() {
   
   if (Serial1.available() > 0) {
    char inByte = Serial1.read();
    Serial.println(inByte); //DEBUG
    playTone(inByte);
    blinkLED();
    delay(20);
   }

}
