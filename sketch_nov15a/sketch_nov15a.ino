void setup() {
  int pin;
  for (pin = 4; pin <= 10; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}

int newstate = LOW;

void loop() {
  int pin;
  for (pin = 4; pin <= 10; pin++) {
    digitalWrite(pin, newstate);
  }
  newstate ^= LOW ^ HIGH;
  delay(500);
}
