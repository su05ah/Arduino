// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12   // sonar sensor TRIGGER
#define PIN_ECHO 13   // sonar sensor ECHO

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25      // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300.0   // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL) // coefficent to convert duration to distance

unsigned long last_sampling_time;   // unit: msec

void setup() {
  // initialize GPIO pins
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_TRIG, OUTPUT);  // sonar TRIGGER
  pinMode(PIN_ECHO, INPUT);   // sonar ECHO
  digitalWrite(PIN_TRIG, LOW);  // turn-off Sonar 
  
  // initialize serial port
  Serial.begin(57600);
}

void loop() { 
  float distance;

  // wait until next sampling time. // polling
  if (millis() < (last_sampling_time + INTERVAL))
    return;

  distance = USS_measure(PIN_TRIG, PIN_ECHO); // read distance
  
  // LED 밝기 변수
  int led_brightness;

  // 거리 범위에 따른 LED 밝기 제어
  if (distance <= 100.0 || distance >= 300.0) {
      led_brightness = 255;  // 100mm 이하 또는 300mm 이상에서는 LED 꺼짐 (최소 밝기)
  } else if (distance == 150.0 || distance == 250.0) {
      led_brightness = 128;  // 150mm 또는 250mm에서는 50% 밝기
  } else if (distance < 200.0) {
      // 100mm ~ 200mm 범위에서 거리가 가까울수록 밝아짐
      led_brightness = map(distance, 100, 200, 255, 0);  // 100mm에서 어둡고, 200mm에서 가장 밝음
  } else {
      // 200mm ~ 300mm 범위에서 거리가 멀수록 어두워짐
      led_brightness = map(distance, 200, 300, 0, 255);  // 200mm에서 가장 밝고, 300mm에서 어두움
  }

  // LED 밝기 설정 (0: 가장 밝음, 255: 꺼짐)
  analogWrite(PIN_LED, led_brightness);

  // 시리얼 모니터에 거리와 LED 밝기 출력
  Serial.print("Distance: ");  Serial.print(distance);
  Serial.print(" mm, LED Brightness: ");  Serial.println(led_brightness);
  
  // update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter.
float USS_measure(int TRIG, int ECHO) {
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(PULSE_DURATION);
  digitalWrite(TRIG, LOW);
  
  return pulseIn(ECHO, HIGH, TIMEOUT) * SCALE; // unit: mm

  // Pulse duration to distance conversion example (target distance = 17.3m)
  // - pulseIn(ECHO, HIGH, timeout) returns microseconds (음파의 왕복 시간)
  // - 편도 거리 = (pulseIn() / 1,000,000) * SND_VEL / 2 (미터 단위)
  //   mm 단위로 하려면 * 1,000이 필요 ==>  SCALE = 0.001 * 0.5 * SND_VEL
  //
  // - 예, pusseIn()이 100,000 이면 (= 0.1초, 왕복 거리 34.6m)
  //        = 100,000 micro*sec * 0.001 milli/micro * 0.5 * 346 meter/sec
  //        = 100,000 * 0.001 * 0.5 * 346
  //        = 17,300 mm  ==> 17.3m
}
