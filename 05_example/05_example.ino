int led = 9;         // PWM 핀에 연결된 LED 핀
int brightness = 0;  // LED 밝기 (0 ~ 255)
int fadeAmount = 5;  // 밝기를 변화시키는 양 (한 번에 5씩 변화)

void setup() {
  // 9번 핀을 출력으로 설정
  pinMode(led, OUTPUT);
}

void loop() {
  // 현재 밝기로 LED를 켭니다
  analogWrite(led, brightness);

  // 다음 루프에서 밝기를 fadeAmount 만큼 변경
  brightness = brightness + fadeAmount;

  // 밝기가 0이거나 255에 도달하면 fadeAmount의 부호를 바꿔서 반대로 변화
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;  // 방향 전환
  }

  // 부드러운 페이드 효과를 위해 30ms 지연
  delay(30);
}
