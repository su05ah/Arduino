int led = 9;         // PWM 핀에 연결된 LED 핀
int period = 1000;       // 기본 주기 (1ms)
int duty = 0;            // 기본 듀티 (0% ~ 100%)
int direction = 1;       // 밝기 증가/감소 방향

void setup() {
  // 9번 핀을 출력으로 설정
  pinMode(led, OUTPUT);
}


void loop() {
  // 주기를 10ms로 설정하여 테스트
  set_period(10000);     // 10ms
  for (int i = 0; i <= 100; i++) {
    set_duty(i);         // 0%에서 100%까지 증가
    delay(10);           // 10ms 대기
  }

  for (int i = 100; i >= 0; i--) {
    set_duty(i);         // 100%에서 0%까지 감소
    delay(10);           // 10ms 대기
  }

  delay(1000);           // 1초 대기 후 다음 주기로 넘어감

  // 주기를 1ms로 설정하여 테스트
  set_period(1000);      // 1ms
  for (int i = 0; i <= 100; i++) {
    set_duty(i);         // 0%에서 100%까지 증가
    delay(10);           // 10ms 대기
  }

  for (int i = 100; i >= 0; i--) {
    set_duty(i);         // 100%에서 0%까지 감소
    delay(10);           // 10ms 대기
  }

  delay(1000);           // 1초 대기 후 다음 주기로 넘어감

  // 주기를 0.1ms로 설정하여 테스트
  set_period(100);       // 0.1ms
  for (int i = 0; i <= 100; i++) {
    set_duty(i);         // 0%에서 100%까지 증가
    delay(10);           // 10ms 대기
  }

  for (int i = 100; i >= 0; i--) {
    set_duty(i);         // 100%에서 0%까지 감소
    delay(10);           // 10ms 대기
  }

  delay(1000);           // 1초 대기 후 loop()의 시작으로 돌아감
}

// 주기를 설정하는 함수 (단위: 마이크로초)
void set_period(int new_period) {
  period = constrain(new_period, 100, 10000);  // period를 100us ~ 10000us 사이로 제한
}

// 듀티 사이클을 설정하는 함수 (단위: %)
void set_duty(int new_duty) {
  duty = constrain(new_duty, 0, 100);  // duty를 0% ~ 100% 사이로 제한
  int on_time = (period * duty) / 100;  // on time 계산 (us)
  int off_time = period - on_time;      // off time 계산 (us)

  // 듀티 사이클에 따른 LED 제어
  digitalWrite(led, HIGH);             // LED 켜기
  delayMicroseconds(on_time);          // on time 동안 대기
  digitalWrite(led, LOW);              // LED 끄기
  delayMicroseconds(off_time);         // off time 동안 대기
}
