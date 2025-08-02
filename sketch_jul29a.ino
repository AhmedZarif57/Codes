const int soundPin = 2;
const int led1 = 6; // Toggles on 1 clap
const int led3 = 3; // Toggles on 3 claps

int claps = 0;
unsigned long lastClap = 0;

void setup() {
  pinMode(soundPin, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led3, OUTPUT);
}

void loop() {
  if (digitalRead(soundPin) == HIGH && millis() - lastClap > 300) {
    claps++;
    lastClap = millis();
  }

  if (claps > 0 && millis() - lastClap > 500) {
    if (claps == 1)
      digitalWrite(led1, !digitalRead(led1));
    else if (claps == 3)
      digitalWrite(led3, !digitalRead(led3));
    claps = 0;
  }
}
