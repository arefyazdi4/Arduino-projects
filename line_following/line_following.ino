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

int outRp = 11;
int outRn = 10;

int outLp = 12;
int outLn = 13;

int moveDeley = 40;

const int blackLineThreshold = 100;

int trigger_right = 8;
int trigger_left = 9;

const int cny_hard_right = A3;
const int cny_soft_right = A2;
const int cny_center = A4;
const int cny_soft_left = A1;
const int cny_hard_left = A5;


const int enum_stop = 0;
const int enum_hard_left =1;
const int enum_soft_left = 2;
const int enum_center = 3;
const int enum_soft_right = 4;
const int enum_hard_right = 5;

const int white = 1;
const int black = 0;


void long_move_delay(){
      move_delay();
      move_delay();
      move_delay();
      move_delay();
      move_delay();

}

void move_delay(){
      delay(moveDeley);
}


int get_optical_sensor_digital_value(int sensor){
  int val = analogRead(sensor);
  if (val > blackLineThreshold){
    return white;
  } else {
    return black;
  }

}

void leftWheelForward(){
  digitalWrite(outLp, HIGH);
  digitalWrite(outLn, LOW);
}

void rightWheelForward(){
  digitalWrite(outRp, HIGH);
  digitalWrite(outRn, LOW);
}

void leftWheelBackrward(){
  digitalWrite(outLp, LOW);
  digitalWrite(outLn, HIGH);
}

void rightWheelBackrward(){
  digitalWrite(outRp, LOW);
  digitalWrite(outRn, HIGH);
}

void leftWheelStop(){
  digitalWrite(outLp, LOW);
  digitalWrite(outLn, LOW);
}

void rightWheelStop(){
  digitalWrite(outRp, LOW);
  digitalWrite(outRn, LOW);
}

void doNotMove(){
  rightWheelStop();
  leftWheelStop();
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
      return enum_hard_right;
  } 
  else if (triggerStateR == HIGH && triggerStateL == LOW) {
       return enum_hard_left;
  }
  else if (triggerStateR == HIGH && triggerStateL == HIGH){
      return enum_center;
  }
  else{
      return enum_center;
  }
}


int is_stop_sign(int hard_left, int soft_left, int center, int soft_right, int hard_right){
  if((hard_left == black && soft_left == white && center == white && soft_right == white && hard_right == black) ||  // 01110
     (hard_left == white && soft_left == white && center == white && soft_right == white && hard_right == white) ||   // 11111
     (hard_left == black && soft_left == black && center == black && soft_right == black && hard_right == black) )  {  // 00000
    return 1;
  }  
  else {
    return 0;
  }
}

int is_stright_line(int hard_left, int soft_left, int center, int soft_right, int hard_right){ 
  if((hard_left == white && soft_left == white && center == black && soft_right == white && hard_right == white) ||  // 11011
     (hard_left == white && soft_left == black && center == black && soft_right == white && hard_right == white) ||  // 10011
     (hard_left == white && soft_left == white && center == black && soft_right == black && hard_right == white)) {  // 11001
    return 1;
  } 
  else {
    return 0;
  }
}

int is_soft_left_turn(int hard_left, int soft_left, int center, int soft_right, int hard_right){
  if((hard_left == white && soft_left == black && center == white && soft_right == white && hard_right == white) ||  // 10111
     (hard_left == black && soft_left == black && center == white && soft_right == white && hard_right == white) ||  // 00111
     (hard_left == black && soft_left == black && center == black && soft_right == white && hard_right == white)) {  // 00011
    return 1; 
  } 
  else {
    return 0;
  }
}

int is_soft_right_turn(int hard_left, int soft_left, int center, int soft_right, int hard_right){
  if((hard_left == white && soft_left == white && center == white && soft_right == black && hard_right == black) ||  // 11101
     (hard_left == white && soft_left == white && center == white && soft_right == black && hard_right == black) ||  // 11100
     (hard_left == white && soft_left == white && center == black && soft_right == black && hard_right == black)) {  // 11000
     return 1;
  } 
  else {
    return 0;
  }
}

int is_hard_left_turn(int hard_left, int soft_left, int center, int soft_right, int hard_right){
  if(hard_left == black && soft_left == white && center == white && soft_right == white && hard_right == white) {  // 01111
    return 1; 
  } 
  else {
    return 0;
  }
}

int is_hard_right_turn(int hard_left, int soft_left, int center, int soft_right, int hard_right){
  if(hard_left == white && soft_left == white && center == white && soft_right == white && hard_right == black) {  // 11110
     return 1;
  } 
  else {
    return 0;
  }
}

int getLineState(){
   //get sensor readings from our 3 inferno red sensor
   int center = get_optical_sensor_digital_value(cny_center);
   int soft_left = get_optical_sensor_digital_value(cny_soft_left);
   int soft_right = get_optical_sensor_digital_value(cny_soft_right);
   int hard_left = get_optical_sensor_digital_value(cny_hard_left);
   int hard_right = get_optical_sensor_digital_value(cny_hard_right);

  Serial.print(hard_left);
  Serial.print(soft_left);
  Serial.print(center);
  Serial.print(soft_right);
  Serial.println(hard_right);


   if(is_stright_line(hard_left, soft_left, center, soft_right, hard_right) == true){
      return enum_center; 
   }
   else if(is_soft_left_turn(hard_left, soft_left, center, soft_right, hard_right) == true){
      return enum_soft_left;
   }

   else if(is_soft_right_turn(hard_left, soft_left, center, soft_right, hard_right) == true){
      return enum_soft_right; 
   }
   else if(is_hard_left_turn(hard_left, soft_left, center, soft_right, hard_right) == true){
      return enum_hard_left;
   }
   else if(is_hard_right_turn(hard_left, soft_left, center, soft_right, hard_right) == true){
      return enum_hard_right;
   }
   else if(is_stop_sign(hard_left, soft_left, center, soft_right, hard_right) == true){
      return enum_stop;
   }
   else{
     return enum_center;
   }
}


void moveToState(int state){
  switch(state){
    case enum_stop:
      doNotMove();
      move_delay();
      break;
    case enum_center:
      moveForward();
      move_delay();
      break;
    case enum_soft_left:
      moveLeft();
      move_delay();
      break;
    case enum_soft_right:
      moveRight();
      move_delay();
      break;
    case enum_hard_left:
      moveLeft();
      long_move_delay();
      break;
    case enum_hard_right:
      moveRight();
      long_move_delay();
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
  int state = enum_stop;
  // state = getWallState();
  // if (state == enum_center){   
    state = getLineState();
  // }
  Serial.println(state);
  return state;
}

void moveRobot(){
  int state = getState();
  moveToState(state);
}

void loop() {
  moveRobot();
}