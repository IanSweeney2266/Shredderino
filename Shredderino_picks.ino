// ME 507 Shredderino project
// Picking code for Teensy 3.6
// @author Ian Sweeney
// 
// Adapted from AccelStepper Multiple Stepper example:
// Copyright (C) 2009 Mike McCauley
// $Id: MultiStepper.pde,v 1.1 2011/01/05 01:51:01 mikem Exp mikem $

#include <AccelStepper.h>
#include <TimerThree.h>

const int led = 13;
const int led2 = 2;

// Define some steppers and the pins the will use
AccelStepper s4(AccelStepper::HALF4WIRE, 23, 22, 21, 20); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5
AccelStepper s3(AccelStepper::HALF4WIRE, 5, 6, 7, 8);
AccelStepper s1(AccelStepper::HALF4WIRE, 9, 10, 11, 12);
AccelStepper s6(AccelStepper::HALF4WIRE, 24, 25, 26, 27);
AccelStepper s2(AccelStepper::HALF4WIRE, 29, 30, 31, 32);
AccelStepper s5(AccelStepper::HALF4WIRE, 36, 35, 34, 33);
AccelStepper picks[6] = {s1, s2, s3, s4, s5, s6};

//AccelStepper stepper3(AccelStepper::FULL2WIRE, 10, 11);

void setup()
{ 
    Serial.begin(9600);
  
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);
    Timer3.initialize(5000);
    Timer3.attachInterrupt(blinkLED); // blinkLED to run every 0.15 seconds
  
    //Set up timer for interrupts
  
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

    pinMode(18, INPUT);
    pinMode(16, INPUT);
    pinMode(14, INPUT);
    pinMode(13, INPUT); 
}

int led2State = LOW;
int ledState = LOW;

int flags[3] = {0, 0, 0};
int go;

void blinkLED(void)
{
  if (led2State == LOW) {
    led2State = HIGH;
  } else {
    //led2State = LOW;
  }
  digitalWriteFast(led2, led2State);
  s1.run();
  s2.run();
  s3.run();
  s4.run();
  s5.run();
  s6.run();
}

void loop()
{
    // Change direction at the limits 
    flags[0] = digitalRead(14);
    flags[1] = digitalRead(16);
    flags[2] = digitalRead(18);
    go = digitalRead(13);

    if (go) {
      if (flags[0] && flags[1] && !flags[2]) { // 6
        digitalWrite(led, LOW);
        if (s6.distanceToGo() == 0) {
          Serial.println("string6");
          pick(6);
        } 
      }
      else if (flags[0] && !flags[1] && flags[2]) { // 5
        digitalWrite(led, LOW);
        if (s5.distanceToGo() == 0) {
          Serial.println("string5");
          pick(5);
        } 
      }
      else if (flags[0] && !flags[1] && !flags[2]) { // 4
        digitalWrite(led, LOW);
        if (s4.distanceToGo() == 0) {
          Serial.println("string4");
          pick(4);
        } 
      }
      else if (!flags[0] && flags[1] && flags[2]) { // 3
        digitalWrite(led, HIGH);
        if (s3.distanceToGo() == 0) {
          Serial.println("string3");
          pick(3);
        } 
      }
      else if (!flags[0] && flags[1] && !flags[2]) { // 2
        digitalWrite(led, HIGH);
        if (s2.distanceToGo() == 0) {
          Serial.println("string2");
          pick(2);
        } 
      }
      else if (!flags[0] && !flags[1] && flags[2]) { // 1
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
    
    delay(500);
    
}

void pick(int string) {
  static int pos_1 = 250;
  static int pos_2 = -100;
  //picks[string-1].moveTo(picks[string-1].targetPosition() == pos ? 0 : pos);
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
