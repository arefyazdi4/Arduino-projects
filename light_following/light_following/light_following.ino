#define HIGH 0x1
#define LOW  0x0

#define INPUT 0x0
#define OUTPUT 0x1

void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int digitalRead(uint8_t pin);
void delay(unsigned long ms);
void setup(void);
void loop(void);

int outRp = 6;
int outRn = 5;

int outLp = 9;
int outLn = 10;

int moveDeley = 40;

const int ldr_left = A1;
const int ldr_center = A2;
const int ldr_right = A3;

const int enum_left = 1;
const int enum_center = 2;
const int enum_right = 3;


void moveForward(){
  // start the engines
  digitalWrite(outRp, HIGH);
  digitalWrite(outRn, LOW);
  digitalWrite(outLp, HIGH);
  digitalWrite(outLn, LOW);
}


void moveRight(){
  digitalWrite(outRn, HIGH);
  digitalWrite(outRp, LOW);
}


void moveLeft(){
  digitalWrite(outLn, HIGH);
  digitalWrite(outLp, LOW);
}


int getHighest(){
   //get sensor readings from our 3 photoresistors
   int left = analogRead(ldr_left);
   int center = analogRead(ldr_center);
   int right = analogRead(ldr_right);
   if(left > center && left > right){
      return enum_left; //1 will signify the left sensor is highest
   }
   else if(center > right){
      return enum_center; //2 will signify the center sensor is highest
   }
   else{
     return enum_right; //if left or center isn't highest, then right probably is.
   }
}


int moveToLight(){
  int highest = getHighest();
  switch(highest){
    case enum_left: //light is on the left
      moveLeft(); //move left
      break;
    case enum_center: //light is center
      moveForward(); //move foward
      break;
    case enum_right: //light is on the right
      moveRight(); //move right
      break;
    default: //no light detected move foward
      moveForward(); //move ahead
      break;
  }
}


void setup() {
  pinMode(outRp, OUTPUT);
  pinMode(outRn, OUTPUT);

  pinMode(outLp, OUTPUT);
  pinMode(outLn, OUTPUT);

  // pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  moveToLight();
  delay(moveDeley);
}