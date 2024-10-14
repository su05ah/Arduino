#include <Servo.h>
#define PIN_SERVO 10

Servo myservo;  //전역변수 
int angle = 0;

void setup() {
  myservo.attach(PIN_SERVO); 
  myservo.write(90);
  delay(1000);
}

void loop() {
    // add code here.
      myservo.write(angle);
      angle = angle + 90;
      if(angle >= 270){
        angle = 0;
      }
  delay(1000);
}
