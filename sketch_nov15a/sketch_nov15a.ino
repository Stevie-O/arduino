const int BUTTON_PIN = 11;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting up...");
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  int pin;
  for (pin = 4; pin <= 10; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}



int blinkstate = LOW;

void blinking_8() {
  int pin;
  for (pin = 4; pin <= 10; pin++) {
    digitalWrite(pin, blinkstate);
  }
  blinkstate ^= LOW ^ HIGH;
  delay(500);
}

int test_pin = 4;
void test_segments() {
  Serial.print("Turning on I/O pin: ");
  Serial.println(test_pin);
  // turn on the segment
  digitalWrite(test_pin, LOW);
  // now wait for LOW (button press)
  while(digitalRead(BUTTON_PIN) == HIGH) delay(1);
  // mow wait for HIGH (button releas)
  while (digitalRead(BUTTON_PIN) == LOW) delay(1);
  // turn off the segment
  digitalWrite(test_pin, HIGH);
  if (++test_pin > 10) test_pin = 4;
}

void loop() {
   // blinking_8();
   test_segments();
}
