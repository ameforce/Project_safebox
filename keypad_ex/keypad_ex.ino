//정방향에서 맨 위부터 c4 c3 c2 c1 r1 r2 r3 r4
//rowPins[ROWS] = {r1, r2, r3, r4}
//colPins[COLS] = {c1, c2, c3, c4}
//노란선부터 13 ~ 마지막 회색선이 6
#include <Servo.h>
#include <Keypad.h>
Servo servo;

const int button16[8] = {6, 7, 8, 9, 10, 11, 12, 13};

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {10, 11, 12, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  char key = keypad.getKey();
  if(key){
    Serial.println(key);
  }
}
