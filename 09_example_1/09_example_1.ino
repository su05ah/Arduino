// Arduino pin assignment
#define PIN_LED  9
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0     // sound velocity at 24 celsius degree (unit: m/sec)
#define INTERVAL 25       // sampling interval (unit: msec)
#define PULSE_DURATION 10 // ultra-sound Pulse Duration (unit: usec)
#define _DIST_MIN 100     // minimum distance to be measured (unit: mm)
#define _DIST_MAX 300     // maximum distance to be measured (unit: mm)

#define TIMEOUT ((INTERVAL / 2) * 1000.0) // maximum echo waiting time (unit: usec)
#define SCALE (0.001 * 0.5 * SND_VEL)     // coefficent to convert duration to distance

#define _EMA_ALPHA 0.5    // EMA weight of new sample (range: 0 to 1)
                          // Setting EMA to 1 effectively disables EMA filter.
// 전역 변수 선언
float dist_raw;        // 센서로부터 측정한 원본 값
float dist_prev = 0;   // 이전 측정값
float dist_ema = 0;    // EMA 계산 값
const float alpha = 0.5;  // EMA의 가중치 값 (PDF 실습에서 _EMA_ALPHA에 해당)

// 범위 필터링에 사용할 최소, 최대 거리
const int DIST_MIN = 10;   // 최소 유효 거리 (단위: mm)
const int DIST_MAX = 350;  // 최대 유효 거리 (단위: mm)

// 센서 데이터 측정 함수
float getDistance() {
  // 센서로부터 데이터를 읽어오는 코드 구현
  // 예: 초음파 센서의 경우, 거리를 측정하여 반환
  float distance = analogRead(A0); // 예시: 아날로그 핀에서 거리 측정
  return distance;
}

void setup() {
  Serial.begin(9600);  // 시리얼 모니터 시작
}

void loop() {
  // 센서로부터 거리값을 측정
  dist_raw = getDistance();

  // 범위 필터링: 측정값이 범위를 벗어나면 이전 값을 사용
  if (dist_raw < DIST_MIN || dist_raw > DIST_MAX) {
    dist_raw = dist_prev;  // 설정 범위 밖이면 이전 값 사용
  } else {
    dist_prev = dist_raw;  // 유효한 값이면 업데이트
  }

  // 지수가중 이동평균(EMA) 계산
  dist_ema = alpha * dist_raw + (1 - alpha) * dist_ema;

  // 시리얼 모니터에 결과 출력
  Serial.print("Raw: "); Serial.print(dist_raw);
  Serial.print(", EMA: "); Serial.println(dist_ema);

  delay(100);  // 100ms 딜레이
}


  // Pulse duration to distance conversion example (target distance = 17.3m)
  // - pulseIn(ECHO, HIGH, timeout) returns microseconds (음파의 왕복 시간)
  // - 편도 거리 = (pulseIn() / 1,000,000) * SND_VEL / 2 (미터 단위)
  //   mm 단위로 하려면 * 1,000이 필요 ==>  SCALE = 0.001 * 0.5 * SND_VEL
  //
  // - 예, pusseIn()이 100,000 이면 (= 0.1초, 왕복 거리 34.6m)
  //        = 100,000 micro*sec * 0.001 milli/micro * 0.5 * 346 meter/sec
  //        = 100,000 * 0.001 * 0.5 * 346
  //        = 17,300 mm  ==> 17.3m
