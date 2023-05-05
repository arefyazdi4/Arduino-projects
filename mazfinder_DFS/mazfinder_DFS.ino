// C++ code
//
/*
  Mazz solver
  This code is resposible to get two signal from push buttoms
  and accourding to resault (1 or 0) it will swith the voltage in motors 
  for limited time it will cuz the motor to routate
  then it will come back to normal state
*/

// #define Arduino_h

// #include <stdlib.h>
// #include <stdbool.h>
// #include <string.h>
// #include <math.h>

// #include <avr/pgmspace.h>
// #include <avr/io.h>
// #include <avr/interrupt.h>

// #include "binary.h"

#define ARDUINO_MAIN
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


// definning pins i/o
int buttonStateR = 0;
int buttonStateL = 0;


const int inR = 4;
const int outRp = 6;
const int outRn = 5;

const int inL = 8;
const int outLp = 9;
const int outLn = 10;

// creating n*n matrix for drawing mapp   
const int H = 8;
const int W = 16;
int mazz[H][W];

// directions value  
const int up = 0;
const int right = 1;
const int down = 2;
const int left = 3;

// turn or moving time quantom 
const int small_block = 210;  
const int full_block = 1100; //1980
const int small_turn = 100;
const int full_turn = 320;

// variables
int direction = 0;
int block = 0;          
int turn = 0;
int hi = H/2;
int hj = W/2;      

void print_mazz (){
  for (int i=0 ; i < H ;i++)
  {
  Serial.println();
  for (int j = 0 ; j < W ; j++ )
    {
      switch(mazz [i][j])
        {
          case -1 :Serial.print('o'); break;
          case 0 :Serial.print('u'); break;
          case 1 :Serial.print('e'); break;
          case 2 :Serial.print('s'); break;
          case 3 :Serial.print('w'); break;
          default :  Serial.println();
        }
    }
  }
  Serial.println();
  Serial.println();
}  

void hand_on_wall (){
    digitalWrite(outRn, HIGH);
    digitalWrite(outRp, LOW);
    digitalWrite(outLp, HIGH);
    digitalWrite(outLn, LOW);
    delay(small_turn);
    turn += small_turn;
    digitalWrite(outRp, HIGH);
    digitalWrite(outRn, LOW);
    digitalWrite(outLp, HIGH);
    digitalWrite(outLn, LOW);  
}

void setup()
{

  pinMode(inR, INPUT);
  pinMode(inL, INPUT);


  pinMode(outRp, OUTPUT);
  pinMode(outRn, OUTPUT);

  pinMode(outLp, OUTPUT);
  pinMode(outLn, OUTPUT);

  // pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  // set a mazz default value -1   
  for (int j = 0 ; j < W ; j++ )
  {
  for (int i = 0 ; i < H ; i++)
    {
      mazz [i][j] = -1;
    }
  }

}



void loop()
{
    Serial.print("block: "); Serial.println(block);
    Serial.print("turn: "); Serial.println(turn);
    Serial.print("dir: "); Serial.println(direction);
    Serial.print("i: ");Serial.println(hi);
    Serial.print("j: "); Serial.println(hj);

  if (turn >= full_turn) {
    turn %=  full_turn;
    direction = (direction + 1) % 4;       
  }
  if (turn <= -full_turn) {
    turn %= full_turn;
    turn -= full_turn;
    direction = (direction - 1) % 4;        
  }

    
  // drawing a mazz map  
  if (block >= full_block) {
    print_mazz();

    block %= full_block;

    mazz[hi][hj] = direction; 

    switch (direction) {
      case 0:        
        hi -= 1;
        break;
      case 1:
        hj += 1;
        break;
      case 2:
        hi += 1;
        break;
      case 3:
        hj -= 1;
        break;
    }       
    hand_on_wall() ;             
  }

    

  // read the state of the pushbutton value
  buttonStateR = digitalRead(inR);
  buttonStateL = digitalRead(inL);
  // check if pushbutton is pressed.  if it is, the
  // buttonState is HIGH
  if (buttonStateL == HIGH) {
    // turn LED on
    // digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(outRn, HIGH);
    digitalWrite(outRp, LOW);
    
    digitalWrite(outLp, HIGH);
    digitalWrite(outLn, LOW);
    
    delay(small_turn);
    turn += small_turn;     
  } 
  if (buttonStateR == HIGH) {
    // turn LED on
    // digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(outLn, HIGH);
    digitalWrite(outLp, LOW);

    digitalWrite(outRp, HIGH);
    digitalWrite(outRn, LOW);
    
    delay(small_turn);
    turn -= small_turn;     
  }
  
  if (buttonStateR == LOW && buttonStateL == LOW) {
    // start the engines
    digitalWrite(outRp, HIGH);
    digitalWrite(outRn, LOW);
    
    digitalWrite(outLp, HIGH);
    digitalWrite(outLn, LOW);
  
    delay(small_block); // Delay a little bit to improve simulation performance
    block += small_block;
  }

  if (buttonStateR == HIGH && buttonStateL == HIGH){
    
    digitalWrite(outLn, HIGH);
    digitalWrite(outLp, LOW);
    digitalWrite(outRn, HIGH);
    digitalWrite(outRp, LOW);
    
    delay(small_block*2); 
    block += small_block*2;
    
    digitalWrite(outLn, HIGH);
    digitalWrite(outLp, LOW);
    digitalWrite(outRp, HIGH);
    digitalWrite(outRn, LOW);
    
    delay(full_turn+small_turn);
    turn -= (full_turn+small_turn);      
  }
}