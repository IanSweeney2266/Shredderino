// ME 507 Shredderino project
// Picking code for Teensy 3.6
// @author Ian Sweeney
// 
// Adapted from AccelStepper Multiple Stepper example:
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h> // A library for running multiple steppers quickly
#include <TimerThree.h> // A library for accessing one of the timers on the Teensy board

const int led = 13; // The pin# for an integrated led on the board
const int led2 = 2; // The pin# for an external breadboarded led (used for debugging)

// Define some steppers and the pins each will use
// Set each to Halfstepping mode for smoother operation
AccelStepper s1(AccelStepper::HALF4WIRE, 9, 10, 11, 12);
AccelStepper s2(AccelStepper::HALF4WIRE, 29, 30, 31, 32);
AccelStepper s3(AccelStepper::HALF4WIRE, 5, 6, 7, 8);
AccelStepper s4(AccelStepper::HALF4WIRE, 23, 22, 21, 20);
AccelStepper s5(AccelStepper::HALF4WIRE, 36, 35, 34, 33);
AccelStepper s6(AccelStepper::HALF4WIRE, 24, 25, 26, 27);

// An array containing all of the steppers for easy access
AccelStepper picks[6] = {s1, s2, s3, s4, s5, s6}; 

// Arduino's setup function; Runs before main
void setup()
{ 
    // Initialize serial for debugging
    Serial.begin(9600);
  
    // Initialize each of our led pins
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);
    
    // Initialize the timer to interrupt every 5ms
    Timer3.initialize(5000);
    Timer3.attachInterrupt(run_steppers);
  
    // Initialize each stepper with run settings
    s1.setMaxSpeed(1500.0);
    s1.setAcceleration(1000.0);
    s1.moveTo(0);
    
    s2.setMaxSpeed(1500.0);
    s2.setAcceleration(1000.0);
    s2.moveTo(0);

    s3.setMaxSpeed(1500.0);
    s3.setAcceleration(1000.0);
    s3.moveTo(0);

    s4.setMaxSpeed(1500.0);
    s4.setAcceleration(1000.0);
    s4.moveTo(0);

    s5.setMaxSpeed(1500.0);
    s5.setAcceleration(1000.0);
    s5.moveTo(0);

    s6.setMaxSpeed(1500.0);
    s6.setAcceleration(1000.0);
    s6.moveTo(0);

    // Initialize inter-board communication pins as inputs
    pinMode(18, INPUT);
    pinMode(16, INPUT);
    pinMode(14, INPUT);
    pinMode(13, INPUT); 
}

// Globals for Led states
int led2State = LOW;
int ledState = LOW;

// Globals for communication
int flags[3] = {0, 0, 0};
int go;

// The ISR for timer3
void run_steppers(void)
{
  // Lets us know if the interrupt is functioning properly
  // We can attach a scope to the high of the led to see the frequency
  if (led2State == LOW) {
    led2State = HIGH;
  } else {
    led2State = LOW;
  }
  digitalWriteFast(led2, led2State);
    
  // Run each of the 6 steppers
  s1.run();
  s2.run();
  s3.run();
  s4.run();
  s5.run();
  s6.run();
}

// Arduino's main loop function; Runs forever
void loop()
{
    // Read from the other board 
    // Three flags represent 3 bits of input (0-7)
    flags[0] = digitalRead(14);
    flags[1] = digitalRead(16);
    flags[2] = digitalRead(18);
    go = digitalRead(13);

    // test the toggle pin
    if (go) {
      if (flags[0] && flags[1] && !flags[2]) { // pick on 6
        digitalWrite(led, LOW);
        if (s6.distanceToGo() == 0) {
          Serial.println("string6");
          pick(6);
        } 
      }
      else if (flags[0] && !flags[1] && flags[2]) { // pick on 5
        digitalWrite(led, LOW);
        if (s5.distanceToGo() == 0) {
          Serial.println("string5");
          pick(5);
        } 
      }
      else if (flags[0] && !flags[1] && !flags[2]) { // pick on 4
        digitalWrite(led, LOW);
        if (s4.distanceToGo() == 0) {
          Serial.println("string4");
          pick(4);
        } 
      }
      else if (!flags[0] && flags[1] && flags[2]) { // pick on 3
        digitalWrite(led, HIGH);
        if (s3.distanceToGo() == 0) {
          Serial.println("string3");
          pick(3);
        } 
      }
      else if (!flags[0] && flags[1] && !flags[2]) { // pick on 2
        digitalWrite(led, HIGH);
        if (s2.distanceToGo() == 0) {
          Serial.println("string2");
          pick(2);
        } 
      }
      else if (!flags[0] && !flags[1] && flags[2]) { // pick on 1
        digitalWrite(led, HIGH);
        if (s1.distanceToGo() == 0) {
          Serial.println("string1");
          pick(1);
        } 
      }
      else {
        digitalWrite(led, LOW);
      }
    }
    
    // Delay 500ms
    delay(500);
    
}

// The pick function
// This function alternates setpoints on for a given stepper when called
void pick(int string) {
  // The two setpoints. One on each side of the string
  // Requires homing before it is turned on
  static int pos_1 = 250;
  static int pos_2 = -100;
    
  // Cool array version but it didn't work due to memory issues with global arrays in interrupts
  //picks[string-1].moveTo(picks[string-1].targetPosition() == pos ? 0 : pos);
    
  // Switch statement for chosing which string to pluck
  switch (string) {
    case 1:
      s1.moveTo(s1.targetPosition() == pos_1 ? pos_2 : pos_1);
      break;
    case 2:
      s2.moveTo(s2.targetPosition() == pos_1 ? pos_2 : pos_1);
      break;
    case 3:
      s3.moveTo(s3.targetPosition() == pos_1 ? pos_2 : pos_1);
      break;
    case 4:
      s4.moveTo(s4.targetPosition() == pos_1 ? pos_2 : pos_1);
      break;
    case 5:
      s5.moveTo(s5.targetPosition() == pos_1 ? pos_2 : pos_1);
      break;
    case 6:
      s6.moveTo(s6.targetPosition() == pos_1 ? pos_2 : pos_1);
      break;
    default:
      break;
  }
}
