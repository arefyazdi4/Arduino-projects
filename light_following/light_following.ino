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


const int ldr_left = A0;
const int ldr_center = A1;
const int ldr_right = A2;

const int cny_right = A3;
const int cny_center = A4;
const int cny_left = A5;

const int enum_stop = 0;
const int enum_hard_left =1;
const int enum_soft_left = 2;
const int enum_center = 3;
const int enum_soft_right = 4;
const int enum_hard_right = 5;

const int white = 1;
const int black = 0;


void long_move_delay(){
      delay(moveDeley);
      delay(moveDeley);
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
      return enum_soft_right;
  } 
  else if (triggerStateR == HIGH && triggerStateL == LOW) {
       return enum_soft_left;
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

    if(center > right && center > left){
      return enum_center; 
   }
   else if(left > center && left > right){
      return enum_soft_left;
   }
   else if(right > center && right > left){
      return enum_soft_right; 
   }
   else{
     return enum_center;
   }
}


int is_stop_sign(int center, int left, int right){
  if(left == black && right == black){
    return 1;
  }  
  else {
    return 0;
  }
}

int is_stright_line(int center, int left, int right){
  if(center == black && left == white && right == white) {
    return 1;
  } 
  else {
    return 0;
  }
}

int is_soft_left_turn(int center, int left, int right){
  if(center == white && left == black && right == white) {
    return 1; 
  } 
  else {
    return 0;
  }
}

int is_hard_left_turn(int center, int left, int right){
  if(center == black && left == black && right == white) {
     return 1;
  } 
  else {
    return 0;    
  }
}

int is_soft_right_turn(int center, int left, int right){
  if(center == white && left == white && right == black) {
     return 1;
  } 
  else {
    return 0;
  }
}

int is_hard_right_turn(int center, int left, int right){
  if(center == black && left == white && right == black) {
    return 1;
  } 
  else {
    return 0;
  }
}

int getLineState(){
   //get sensor readings from our 3 inferno red sensor
   int center = get_optical_sensor_digital_value(cny_center);
   int left = get_optical_sensor_digital_value(cny_left);
   int right = get_optical_sensor_digital_value(cny_right);

    if(is_stright_line(center, left, right) == true){
      return enum_center; 
   }
   else if(is_soft_left_turn(center, left, right) == true){
      return enum_soft_left;
   }
   else if(is_hard_left_turn(center, left, right) == true){
      return enum_hard_left;
   }
   else if(is_soft_right_turn(center, left, right) == true){
      return enum_soft_right; 
   }
   else if(is_hard_right_turn(center, left, right) == true){
      return enum_hard_right;
   }
   else if(is_stop_sign(center, left, right) == true){
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
      delay(moveDeley);
      break;
    case enum_center:
      moveForward();
      delay(moveDeley);
      break;
    case enum_soft_left:
      moveLeft();
      delay(moveDeley);
      break;
    case enum_hard_left:
      moveLeft();
      long_move_delay();
      break;
    case enum_soft_right:
      moveRight();
      delay(moveDeley);
      break;
    case enum_hard_right:
      moveLeft();
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
  state = getLineState();
  // if (state == enum_center){   
  //   state = getWallState();
  //   if (state == enum_center){ 
  //     state = getLightState();
  //   }
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