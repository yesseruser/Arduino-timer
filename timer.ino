enum {
  DIGIT_G = 7,
  DIGIT_F,
  DIGIT_E,
  DIGIT_D,
  DIGIT_C,
  DIGIT_B,
  DIGIT_A
}DigitsPins;
#define NUMBER_1 5
#define NUMBER_2 6
#define SIREN 4 
int secondsToBoom = 0;
bool ticking = false;
bool table[10][7] = {
  {1, 1, 1, 1, 1, 1, 0},
  {0, 1, 1, 0, 0, 0, 0},
  {1, 1, 0, 1, 1, 0, 1},
  {1, 1, 1, 0, 1, 0, 1},
  {0, 1, 1, 0, 0, 1, 1},
  {1, 0, 1, 0, 1, 1, 1},
  {1, 0, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 0, 0, 0},
  {1, 1, 1, 1, 1, 1, 1},
  {1, 1, 1, 0, 1, 1, 1}
  };
void setup() {
  for(int i = 4; i <= 13; i++){
    pinMode(i, OUTPUT);
  }
  Serial.begin(9600);
}

void loop() {
  if(Serial.available() == 2){
    char num1 = Serial.read();
    char num2 = Serial.read();
    secondsToBoom = (num1 - '0') * 10 + (num2 - '0');
    ticking = true;
  }
  tickingBySecond();
  showBoomTime(secondsToBoom);
  checkForSiren(true);
}

 void showNumber (int number){
  if (number < 0 || number > 9){
    return;
  }
  int actualPin = DIGIT_A;
  int actualRow = 0;
  for(; actualPin >= DIGIT_G; actualPin--){
    digitalWrite(actualPin, table[number][actualRow]);
    actualRow++;
  }
 }

void showBoomTime(int boomTime){
  if(ticking && boomTime == 0){
    digitalWrite(NUMBER_1, LOW);
    digitalWrite(NUMBER_2, LOW);
    return;
  }
  if(boomTime >= 0){
    showNumber(boomTime/10);
    digitalWrite(NUMBER_2, HIGH);
    delay(1);
    digitalWrite(NUMBER_2, LOW);
    showNumber(boomTime%10);
    digitalWrite(NUMBER_1, HIGH);
    delay(1);
    digitalWrite(NUMBER_1, LOW);
  } 
}

void tickingBySecond(){
  if(!ticking){
    return;
  }
  for(int i = 0; i < 100; i++){
    delay(8);
    showBoomTime(secondsToBoom);
  }
  secondsToBoom --;
}

void playSiren(){
  for(int i = 0; i < 10; i++){
    digitalWrite(SIREN, HIGH);
    delay(1000);
    digitalWrite(SIREN, LOW);
    delay(1000);
  }
  ticking = false;
}

bool checkForSiren(bool autostart){
  if(secondsToBoom == 0 && ticking){
    if(autostart){
      playSiren();
    }
    return true;
  }
  else{
    return false;
  }
}
//note: This is not going to explode
