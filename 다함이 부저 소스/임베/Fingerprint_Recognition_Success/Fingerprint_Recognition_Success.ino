#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 783
#define A5 880
#define B5 988
#define C6 1047
int piezoPin=9;
int tempo=100; //빠르고 연속되게
int note[15]={C5,D5,E5,F5,G5,A5,B5,C6,B5,A5,G5,F5,E5,D5,C5};

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  //if 지문인식 성공시
  for(int i=0; i<; i++) {
    tone(piezoPin, note[i], tempo);
    delay(1000);
  }
}
