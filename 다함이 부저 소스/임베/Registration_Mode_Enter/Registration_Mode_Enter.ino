#define C5 523
#define D5 587
#define E5 659
#define F5 698
#define G5 783
int piezoPin=9;
int tempo=100;
int note[5]={C5,D5,E5,F5,G5}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  //지문등록모드 진입시
  for(int i=0; i<5; i++) {
    tone(piezoPin, note[i], tempo);
    delay(1000);
  }
}
