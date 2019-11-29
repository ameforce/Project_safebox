#define G4 392
#define B4 494
int piezoPin=9;
int tempo=100 //빠른속도
int note[5]={B4,G4,B4,G4,B4}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  //if 지문인식 실패시
  for(int i=0; i<5; i++) {
    tone(piezoPin, note[i], tempo);
    delay(1000);
  }
}
