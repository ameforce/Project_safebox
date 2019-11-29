#define C4 262
#define E4 330
#define G4 392
#define C5 523
int piezoPin=9;
int tempo = 200;
int note[4]={C5,G4,E4,C4}

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //문이 폐쇄인식되면
  for(int i=0; i<4; i++) {
    tone(piezoPin, note[i], tempo);
    delay(100);
  }
  
}
