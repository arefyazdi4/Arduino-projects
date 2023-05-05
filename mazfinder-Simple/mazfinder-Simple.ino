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

// #define ARDUINO_MAIN
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

int buttonStateR = 0;
int buttonStateL = 0;


int inR = 4;
int outRp = 6;
int outRn = 5;

int inL = 8;
int outLp = 9;
int outLn = 10;

int turnDeley = 240;

void setup()
{
  pinMode(inR, INPUT);
  pinMode(inL, INPUT);


  pinMode(outRp, OUTPUT);
  pinMode(outRn, OUTPUT);

  pinMode(outLp, OUTPUT);
  pinMode(outLn, OUTPUT);

  // pinMode(LED_BUILTIN, OUTPUT);
}


void loop()
{
  // start the engines
  digitalWrite(outRp, HIGH);
  digitalWrite(outRn, LOW);
  digitalWrite(outLp, HIGH);
  digitalWrite(outLn, LOW);
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
  } 
    if (buttonStateR == HIGH) {
    // turn LED on
    // digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(outLn, HIGH);
    digitalWrite(outLp, LOW);
  }
  delay(turnDeley); // Delay a little bit to improve simulation performance
}