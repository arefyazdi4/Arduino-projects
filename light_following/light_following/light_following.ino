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

const int blackLineThreshold = 45;

int trigger_right = 4;
int trigger_left = 8;


const int ldr_left = A0;
const int ldr_center = A1;
const int ldr_right = A2;

const int cny_right = A3;
const int cny_center = A4;
const int cny_left = A5;


const int enum_left = 1;
const int enum_center = 2;
const int enum_right = 3;

void leftWheelForward(){
  digitalWrite(outLp, HIGH);
  digitalWrite(outLn, LOW);
}

void leftWheelBackrward(){
  digitalWrite(outLn, HIGH);
  digitalWrite(outLp, LOW);
}

void rightWheelForward(){
  digitalWrite(outRp, HIGH);
  digitalWrite(outRn, LOW);
}

void rightWheelBackrward(){
  digitalWrite(outRn, HIGH);
  digitalWrite(outRp, LOW);
}

void moveForward(){
  rightWheelForward();
  leftWheelForward();
}


void moveRight(){
  rightWheelBackrward();
  leftWheelForward();
}


void moveLeft(){
  leftWheelBackrward();
  rightWheelForward();
}


int getWallState(){
  // read the state of the pushbutton value
  int triggerStateR = digitalRead(trigger_right);
  int triggerStateL = digitalRead(trigger_left);
  // check if pushbutton is pressed.  if it is, the
  if (triggerStateL == HIGH && triggerStateR == LOW) {
      return enum_right;
  } 
  else if (triggerStateR == HIGH && triggerStateL == LOW) {
       return enum_left;
  }
  else if (triggerStateR == HIGH && triggerStateL == HIGH){
      return enum_center;
  }
  else{
      return enum_center;
  }
}


int getLightState(){
   //get sensor readings from our 3 photoresistors
   int left = analogRead(ldr_left);
   int center = analogRead(ldr_center);
   int right = analogRead(ldr_right);

   if(left < center && left < right){
      return enum_left;
   }
   else if(right < center && right < left){
      return enum_right; 
   }
   else{
     return enum_center;
   }
}


int getLineState(){
     //get sensor readings from our 3 photoresistors
   int left = analogRead(cny_left);
   int center = analogRead(cny_center);
   int right = analogRead(cny_right);

   if(left < center && left < right){
      return enum_left;
   }
   else if(right < center && right < left){
      return enum_right; 
   }
   else{
     return enum_center;
   }
}


void moveToState(int state){
  switch(state){
    case enum_left:
      moveLeft();
      break;
    case enum_center:
      moveForward();
      break;
    case enum_right:
      moveRight();
      break;
    default:
      moveForward();
      break;
  }
}


void setup() {
  pinMode(trigger_right, INPUT);
  pinMode(trigger_left, INPUT);

  pinMode(outRp, OUTPUT);
  pinMode(outRn, OUTPUT);

  pinMode(outLp, OUTPUT);
  pinMode(outLn, OUTPUT);

  // pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
}

int getState(){
  int state = getWallState();
  if (state == enum_center){ 
    state = getLineState();
  }
  if (state == enum_center){ 
    state = getLightState();
  }
}

void moveRobot(){
  int state = getState();
  moveToState(state);
}

void loop() {
  moveRobot();
  delay(moveDeley);
}