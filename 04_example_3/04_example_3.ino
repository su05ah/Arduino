#define PIN_LED 7
unsigned int blinkCount;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("Hello World!");
  blinkCount = 0;
}

void loop() {
  // 1초 동안 LED를 켬
  digitalWrite(PIN_LED, HIGH);
  delay(1000);

  // 1초 동안 5회 깜빡이기 (100ms ON, 100ms OFF)
  for (blinkCount = 0; blinkCount < 5; blinkCount++) {
    digitalWrite(PIN_LED, LOW); // Turn LED off
    delay(100);                 // Wait for 100ms
    digitalWrite(PIN_LED, HIGH); // Turn LED on
    delay(100);                 // Wait for 100ms
  }

  // LED 끄고 무한루프
  digitalWrite(PIN_LED, LOW);
  while (1) {} // Infinite loop
}
