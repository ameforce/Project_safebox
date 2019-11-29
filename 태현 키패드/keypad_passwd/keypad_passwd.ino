#include <Keypad.h>
#define greenPin A0         //resist == 130 closed
#define yellowPin A1       //resist == 130 unlocking
#define redPin A2         //resist == 130 opened
#define soundPin A3

bool init_check = false;
const byte ROWS = 4;
const byte COLS = 4;
byte rowpins[ROWS] = {9, 8, 7, 6};
byte colpins[COLS] = {10, 11, 12, 13};
char passwd[16] = {0};
int count = 0;

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'} 
};
Keypad keypad = Keypad(makeKeymap(keys), rowpins, colpins, ROWS, COLS);


bool check_passwd_size(int siz){
  if(siz < 4){
    Serial.println("4글자 이상 입력해야 합니다.");
    return false;
  }
  else if(siz > 16){
    Serial.println("16글자를 초과하였습니다.");
    return false;
  }
  else
    return true;
}

void led_state(int state){
  switch(state){
  case 0:  // greenPin light
    digitalWrite(greenPin, 130);
    digitalWrite(yellowPin, 0);
    digitalWrite(redPin, 0);
    break;
  case 1:  // yellowPin light
    digitalWrite(greenPin, 0);
    digitalWrite(yellowPin, 130);
    digitalWrite(redPin, 0);
    break;
  case 2:  // redPin light
    digitalWrite(greenPin, 0);
    digitalWrite(yellowPin, 0);
    digitalWrite(redPin, 130);
    tone(soundPin, 1500);
    delay(50);
    noTone(soundPin);
    break;
  }
}

void init_passwd(){
  char key;
  int idx = 0;
  Serial.print("Test: 초기 비밀번호는 ");
  while((key = keypad.getKey()) != '*'){
    led_state(1);
    if(key == 0) continue;
    else {
      led_state(2);
      delay(100);
    }
    passwd[idx] = key;
    Serial.print(passwd[idx]);
    idx++;
  }
  if(!check_passwd_size(idx)){
    for(int i = 0; i<idx; i++)
      passwd[i] = 0;
    }
   Serial.println();
}

bool checking(){
  bool check = true;
  char key;
  int idx = 0;
  Serial.print("Test: 입력한 비밀번호는 ");
  while((key = keypad.getKey())!= '*'){
    led_state(1);
    if(key == 0) continue;
    else {
      led_state(2);
      delay(100);
    }
    if(key != passwd[idx]){
      check = false;
    }
    Serial.print(key);
    idx++;
  }
  if(!check_passwd_size(idx)) check = false;
  return check;
}

void setup() {
  Serial.begin(9600);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  digitalWrite(greenPin, 130);
  digitalWrite(yellowPin, 130);
  digitalWrite(redPin, 130);
}

void loop() {
  led_state(0);
  if(init_check == false){
    init_passwd();
    init_check = true;
  }
  char key = keypad.getKey();
  if(key != 0){
    bool input_check = checking();
    if(input_check == true){
      Serial.println("문이 열립니다.");
      //문이 열림, open_door 구현 -> led_state(2);
      count = 0;
    }
    else if(input_check == false){
      count++;
      Serial.print("비밀번호를 틀리셨습니다. (");
      Serial.print(count);
      Serial.println(" / 5회)");
      if(count >= 5){
        Serial.println("비밀번호를 5번 틀리셨습니다. 5분간 정지합니다.");
        led_state(2);
        delay(300000);
        count = 0;
      }
    }
  }
}
