#define D5 587
int piezoPin=9;
int tempo=400;
int note=D5

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  //if 키패드 눌리면
  tone(piezoPin, note, tempo);
  delay(1000);
}
