const int SWITCH_BUTTON_PIN = 2;
const int SWITCH_BUTTON_INT = 0;
const int TEST_BUTTON_PIN = 12;

const int FIRST_SEG_PIN = 4;
const int LAST_SEG_PIN = 11;
#define SEG_PIN_BIT(pin) ((pin) - FIRST_SEG_PIN)
#define SEG_PIN_MASK(pin) (1 << SEG_PIN_BIT(pin))
const int SEG_A_PIN = 4; // Top
const int SEG_B_PIN = 5; // Upper Right
const int SEG_C_PIN = 6; // Lower Right
const int SEG_D_PIN = 10; // Bottom
const int SEG_E_PIN = 9; // Lower Left
const int SEG_F_PIN = 7; // Upper Left
const int SEG_G_PIN = 8; // Center
const int SEG_DOT_PIN = 11; // Dot
#define SEG_A SEG_PIN_MASK(SEG_A_PIN)
#define SEG_B SEG_PIN_MASK(SEG_B_PIN)
#define SEG_C SEG_PIN_MASK(SEG_C_PIN)
#define SEG_D SEG_PIN_MASK(SEG_D_PIN)
#define SEG_E SEG_PIN_MASK(SEG_E_PIN)
#define SEG_F SEG_PIN_MASK(SEG_F_PIN)
#define SEG_G SEG_PIN_MASK(SEG_G_PIN)
#define SEG_DOT SEG_PIN_MASK(SEG_DOT_PIN)

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))

const byte digits[10] = {
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F,  // 0
    SEG_B|SEG_C, // 1
    SEG_A|SEG_B|SEG_G|SEG_E|SEG_D, // 2
    SEG_A|SEG_B|SEG_G|SEG_C|SEG_D, // 3
    SEG_F|SEG_B|SEG_G|SEG_C, // 4
    SEG_A|SEG_F|SEG_G|SEG_C|SEG_D, // 5
    SEG_A|SEG_F|SEG_G|SEG_E|SEG_C|SEG_D, // 6
    SEG_A|SEG_B|SEG_C, // 7
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G, // 8,    
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_F|SEG_G, // 9,    
};
    
    
    
void setup() {
  Serial.begin(115200);
  Serial.println("Starting up...");
  pinMode(TEST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SWITCH_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(SWITCH_BUTTON_INT, switcheroo, FALLING);
  int pin;
  for (pin = FIRST_SEG_PIN; pin <= LAST_SEG_PIN; pin++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }
}

typedef void(*loopfunc_t)();

const loopfunc_t loop_function_list[] = {
    blinking_8,
    test_segments,    
    count_up,
    count_down,
    phoxtane,
};

volatile byte cur_loop_function = 0;
volatile byte abort_func = 0;

void switcheroo() {
  Serial.println("switcheroo!");
  abort_func = 1;
  if (++cur_loop_function >= ARRAY_SIZE(loop_function_list))
    cur_loop_function = 0;
}

byte abortable_delay(int ms) {
  int i;
  for (i = 0; i < ms && abort_func == 0; i++)
    delay(1);
 return abort_func;
}

int blinkstate = LOW;

void blinking_8() {
  int pin;
  for (pin = FIRST_SEG_PIN; pin <= LAST_SEG_PIN; pin++) {
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
  while(digitalRead(TEST_BUTTON_PIN) == HIGH) 
    if(abortable_delay(1)) return;
  
  // mow wait for HIGH (button releas)
  while (digitalRead(TEST_BUTTON_PIN) == LOW) 
    if (abortable_delay(1)) return; 
  // turn off the segment
  digitalWrite(test_pin, HIGH);
  if (++test_pin > 10) test_pin = 4;
}

void loop() {
  abort_func = 0;
   // blinking_8();
   //test_segments();
   //count_up();
   //phoxtane();
   loop_function_list[cur_loop_function]();
}

void set_display(byte bits) {
  int pin;
  for (pin = FIRST_SEG_PIN; pin <= LAST_SEG_PIN; pin++)
  {
    digitalWrite(pin, (bits & SEG_PIN_MASK(pin)) ? LOW : HIGH);
  }
}

int counter_digit = 0;
void count_up() {
  set_display(digits[counter_digit]);
  if (++counter_digit >= ARRAY_SIZE(digits))
    counter_digit = 0;
  abortable_delay(500);
}

void count_down() {
  set_display(digits[counter_digit]);
  if (counter_digit == 0) counter_digit = ARRAY_SIZE(digits);
  counter_digit--;
  abortable_delay(500);
}

const byte phox[] = {
    SEG_A|SEG_F|SEG_B|SEG_G|SEG_E,
//    SEG_F|SEG_B|SEG_C,
    SEG_F|SEG_E|SEG_G|SEG_C,
//    SEG_E|SEG_C,
    SEG_E|SEG_G|SEG_C|SEG_D,
//    SEG_E,
    SEG_G|SEG_D|SEG_C,
//    SEG_G|SEG_D|SEG_E,
//    SEG_G|SEG_B|SEG_C,        // first half of 't'
    SEG_G|SEG_F|SEG_E|SEG_D,  // second half of 't'
//    SEG_C,
    SEG_C|SEG_E|SEG_G|SEG_D|SEG_DOT, // 'a'
//    SEG_E|SEG_C,
    SEG_E|SEG_G|SEG_C,
//    SEG_E|SEG_B|SEG_C,
    SEG_E|SEG_F|SEG_G|SEG_A|SEG_D,
    0,
};

int phox_idx = 0;
void phoxtane() {
  set_display(phox[phox_idx]);
  if (++phox_idx >= ARRAY_SIZE(phox))
    phox_idx = 0;
  abortable_delay(500);
}

