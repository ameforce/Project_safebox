#include <Servo.h>
Servo servo;
int value = 0;
const int servoPin = 4;
#define lockerPin 5           //HIGH일때 off, LOW일 때 on
#define greenPin A0         //130부터 켜짐
#define yellowPin A1         //130부터 켜짐
#define redPin A2         //130부터 켜짐

void setup() {
  servo.attach(servoPin);
  pinMode(lockerPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  digitalWrite(lockerPin, HIGH);
  analogWrite(greenPin, 180);
  analogWrite(yellowPin, 130);
  analogWrite(redPin, 130);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available()){
    char in_data = Serial.read();
    if (in_data == 'O'){
      Serial.println("모터를 180 가동합니다");
      digitalWrite(lockerPin, LOW);
      delay(1000);
      for (int i = 0; i < 180; i++){
        servo.write(i);
        delay(2);
      }
      Serial.println("가동완료\n");
    }else if (in_data == 'C'){
      Serial.println("모터를 0 가동합니다");
      digitalWrite(lockerPin, HIGH);
      delay(1000);
      for (int i = 180; i > 0; i--){
        servo.write(i);
        delay(2);
      }
      Serial.println("가동완료\n");
    }
  }
}
