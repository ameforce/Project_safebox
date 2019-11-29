#include <Adafruit_Fingerprint.h>
#include <Keypad.h>
#include <Servo.h>

#define greenPin A0
#define yellowPin A1
#define redPin A2
#define piezoPin A3
#define lightPin A4
#define servoPin 4
#define lockerPin 5   //HIGH가 스위치OFF
#define C4 262
#define E4 330
#define G4 392
#define B4 494
#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 783
#define A5 880
#define B5 988
#define C6 1047
#define ROWS 4
#define COLS 4
char led_State = 'F';



//################keypad_passwd Area#####################
byte rowpins[ROWS] = {9, 8, 7, 6};
byte colpins[COLS] = {10, 11, 12, 13};
byte count = 0;
char key_data[4], master_passwd[16] = {0}; // temp_pw

char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'} 
};
Keypad keypad = Keypad(makeKeymap(keys), rowpins, colpins, ROWS, COLS);

typedef struct linked{
  char data;
  struct linked* next;
}linked;


bool check_passwd_size(byte siz){
  if(siz < 4){
    //Serial.println("4글자 이상 입력해야 합니다.");
    return false;
  }else if(siz > 16){
    //Serial.println("16글자를 초과하였습니다.");
    return false;
  }else{ return true; }
}

void led_state(char state){
  switch(state){
    case 'G':  // greenPin light
    if (led_State != 'G'){
      led_State = 'G';
      digitalWrite(greenPin, 130);
      digitalWrite(yellowPin, 0);
      digitalWrite(redPin, 0);
    }break;
    
    case 'Y':  // yellowPin light
    if (led_State != 'Y'){
      led_State = 'Y';
      digitalWrite(greenPin, 0);
      digitalWrite(yellowPin, 130);
      digitalWrite(redPin, 0);
    }break;
    
    case 'R':  // redPin light
    if (led_State != 'R'){
      led_State = 'R';
      digitalWrite(greenPin, 0);
      digitalWrite(yellowPin, 0);
      digitalWrite(redPin, 130);
    }break;

    case 'A':
    if (led_State != 'A'){
      led_State = 'A';
      digitalWrite(greenPin, 130);
      digitalWrite(yellowPin, 130);
      digitalWrite(redPin, 130);
    }break;
  }
}

bool init_passwd(){ //초기 비밀번호 세팅 함수
  char led_State = 'F';
  int idx = 0;
  
  //Serial.print("초기 비밀번호를 입력하세요 : ");
  while((master_passwd[idx] = keypad.getKey()) != '*'){    //key가 *이면 깨짐
    led_state('Y');
    if(master_passwd[idx]){
      //Serial.print(master_passwd[idx]);
      led_state('R');
      Keypad_Pressing_Recognition();
      idx++;
    }
  }
  if(!check_passwd_size(idx)){ return false; }
  //Serial.println();
  return true;
}

bool checking(){
  bool check = true, admin_check = true;
  char key, led_State = 'F', admin_pw[8] = {'5', '0', '9', '7', 'A', 'B', 'C', 'D'};
  byte idx = 0;
  //Serial.print("Test: 입력한 비밀번호는 ");
  while((key = keypad.getKey())!= '*'){
    led_state('Y');
    if(key){
      led_state('R');
      Keypad_Pressing_Recognition();
      if (key != master_passwd[idx]){ check = false; }
      if (key != admin_pw[idx]){ admin_check = false; }
      idx++;
      Serial.print(key);
    }
  }
  if(!check_passwd_size(idx)) { check = false; }
  if(admin_check) { manage_finger(); }
  return check;
}

int check_keydata(){
  char key[1];
  int idx = 0, num;
  Serial.println("관리자 모드 진입.");
  Serial.print("지문 아이디를 입력하세요 : ");
  Registration_Mode_Enter();
  while((key[0] = keypad.getKey()) != '*'){
     led_state('Y');
    if(key[0]){
      led_state('R');
      if (idx == 0){ num = atoi(key) * 100; }
      else if (idx == 1){ num += atoi(key) * 10; }
      else if (idx == 2){ num += atoi(key); }
      else{ num = -1; }
      idx++;
      //Serial.println(); Serial.println(key[0]); Serial.println(atoi(key));
    }
  }//Serial.println();
  
  while(num < 0 || num >= 128){
    //Serial.println("Rule : 0 ~ 127 사이의 값을 입력해야 합니다.");
    num = check_keydata();
  }
  return num;
}

void fingerprint_detecting(){
  
}

void manage_finger(){
  //비밀번호 감지가 안되고 있을 때 지문감지.
  //지문 감지가 되면 아래 알고리즘 실행.
  // matching finger true --> delete_finger
  // false --> enroll_finger
  fingerprint_detecting();
  fingerprint_enroll(check_keydata());
  
}
//######################################################


//################servo Area#####################
Servo servo;

void door_open(){
  //Serial.println("도어락 해제 중입니다.");
  delay(1000);
  digitalWrite(lockerPin, LOW);
  delay(100);
  digitalWrite(lockerPin, HIGH);
  //Serial.println("도어락 해제 완료.\n");
  delay(100);
  //Serial.println("자동문 오픈 중입니다.");
  servo.write(180);
  delay(500);
  //Serial.println("자동문 오픈 완료.\n");
  Door_Opened_Recognition();
  servo_reset();
}

void servo_reset(){
  //Serial.println("서보모터 리셋중 입니다.");
  servo.write(0);
  //Serial.println("서보모터 리셋 완료.\n");
}
//######################################################


//################System Sound Area###################
void Fingerprint_Recognition_Success(){ //지문인식 성공
  int note[9]={C5,D5,E5,F5,G5,F5,E5,D5,C5};
  peizo_play(note, 100, sizeof(note));
  return;
}

void Fingerprint_Recognition_Failure(){ //지문인식 실패
  int note[5]={B4,G4,B4,G4,B4};
  peizo_play(note, 100, sizeof(note)/sizeof(int));
  return;
}

void Registration_Mode_Enter(){        //지문등록모드 진입
  int note[5]={C5,D5,E5,F5,G5};
  peizo_play(note, 100, sizeof(note)/sizeof(int));
  return;
}

void Fingerprint_Checking(){
  int note[1] = {C4};
  peizo_play(note, 100, sizeof(note)/sizeof(int));
  return;
}

void Door_Opened_Recognition(){      //문 열림
  int note[4]={C5,G4,E4,C4};
  peizo_play(note, 200, sizeof(note)/sizeof(int));
  return;
}

void Keypad_Pressing_Recognition(){   //키패드 누름
  int note[1] = {D5};
  peizo_play(note, 400, sizeof(note)/sizeof(int));
  return;
}

void peizo_play(int note[], int tempo, int note_size){
  for(int i=0; i<note_size; i++) {
    tone(piezoPin, note[i], tempo);
    delay(100);
    noTone(piezoPin);
  }
  return;
}
//####################################################


//################FingerPrint Area#####################
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
//uint8_t id;

void fingerprint_enroll(uint8_t num){
  Registration_Mode_Enter();
  Serial.println("지문 등록 시스템 실행중 입니다.");
  finger.begin(57600);
  if (finger.verifyPassword()){
    led_state('G');
    Serial.println("지문 등록 시스템 실행 완료.\n");
    Serial.print("현재 등록 중인 아이디 : ");
    Serial.println(num);
    getFingerprintEnroll(num);
  }else{
    //Serial.println("지문 등록 시스템 실행 실패.\n");
    while(1);
  }
}

boolean Finger_case(int p){
  switch(p){
    led_state('G');
    case FINGERPRINT_OK:
    return true;

    case FINGERPRINT_NOFINGER:
    led_state('R');
    Fingerprint_Checking();
    Serial.print(".");
    return false;

    case FINGERPRINT_PACKETRECIEVEERR:
    led_state('R');
    Serial.println("통신 에러가 발생했습니다.");
    return false;

    case FINGERPRINT_IMAGEFAIL:
    case FINGERPRINT_ENROLLMISMATCH:
    led_state('R');
    Serial.println("지문이 일치하지 않습니다.");
    return false;

    case FINGERPRINT_IMAGEMESS:
    Serial.println("지문 데이터 용량이 너무 큽니다.");
    return false;

    case FINGERPRINT_FEATUREFAIL:
    case FINGERPRINT_INVALIDIMAGE:
    led_state('R');
    Serial.println("지문 시스템에 에러가 발생했습니다.");
    return false;

    case FINGERPRINT_BADLOCATION:
    led_state('R');
    Serial.println("지문을 저장할 공간이 부족합니다.");
    return false;

    case FINGERPRINT_FLASHERR:
    led_state('R');
    Serial.println("지문 쓰기 에러가 발생했습니다.");
    return false;
              
    default:
    led_state('R');
    Serial.println("알 수 없는 에러가 발생했습니다.");
    return false;
  }
}
void getFingerprintEnroll(uint8_t num) {
  int p = -1;
  while ((p = finger.getImage()) != FINGERPRINT_OK){ Finger_case(p); }
  if (Finger_case(p)){
    Serial.println("\n지문이 감지되었습니다.");
    if (Finger_case(finger.image2Tz(1))){
      Serial.println("지문 데이터 변환에 성공했습니다.\n손가락을 떼주세요.");
      p = 0;
      while ((p = finger.getImage()) != FINGERPRINT_NOFINGER){ Finger_case(p); }
      Serial.println("같은 손가락을 다시 대주세요.");
      while (!Finger_case(finger.getImage()));
      Serial.println("\n지문이 감지되었습니다.");
      if (Finger_case(finger.image2Tz(2))){
        Serial.println("지문 데이터 변환에 성공했습니다.\n지문 모델 생성을 시작합니다.");
        if (Finger_case(finger.createModel())){
          Serial.println("모델 생성에 성공했습니다.");
          if (Finger_case(finger.storeModel(num))){
            Serial.println("지문 등록에 성공했습니다!!!");
            Fingerprint_Recognition_Success();
          }
        }
      }
    }
  }
  return;
}
//######################################################



void setup() {
  Serial.begin(9600);
  pinMode(greenPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  led_state('A');

  servo.attach(servoPin);
  pinMode(lockerPin, OUTPUT);
  digitalWrite(lockerPin, HIGH);
  servo_reset();

  
  while (!init_passwd());   //초기 비밀번호 세팅
  //Serial.println("초기 비밀번호 설정 완료!");
  while(true){ loop(); }
}


void loop() {
  //Serial.println(analogRead(lightPin));
  //delay(100);
  led_state('G');
  char key = keypad.getKey();
  if(key != 0){
    //Keypad_Pressing_Recognition();
    bool input_check = checking();
    if(input_check == true){
      //Serial.println("문이 열립니다.");
      led_state('R');
      door_open();
      count = 0;
    }
    else if(input_check == false){
      count++;
      //Serial.print("비밀번호를 틀리셨습니다. (");
      //Serial.print(count);
      //Serial.println(" / 5회)\n");
      if(count >= 5){
        //Serial.println("비밀번호를 5번 틀리셨습니다. 5분간 정지합니다.");
        led_state('R');
        delay(300000);
        count = 0;
      }
    }
  }
  //else{ // Finger_matching
    // true --> door_open(); 
  //}
}
