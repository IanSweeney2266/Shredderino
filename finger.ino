/** @file finger.ino */
/* ME 507 Project
 * @author Julian Rodkiewicz
 *
 * Adapted from MIDIUSB example by:
 * Created: 4/6/2015 10:47:08 AM
 * Author: gurbrinder grewal
 * Modified by Arduino LLC (2015)
 */ 
#include <AccelStepper.h>
#include "MIDIUSB.h"
using namespace std;

int keyDown = 0; /*!< flag for pressing a key */
int strumCount = 1; /*!< flag for how many times to strum */

int fret1 = 0;    /*!< y location for fret 1 */
int fret2 = -175; /*!< y location for fret 2 */
int fret3 = -340; /*!< y location for fret 3 */
int fret4 = -485; /*!< y location for fret 4 */
int fret5 = -645; /*!< y location for fret 5 */
int fret6 = -775; /*!< y location for fret 6 */

int string = 0; /*!< stepper for X */

AccelStepper xStepper(AccelStepper::FULL4WIRE, 2, 5, 4, 3); /*!< stepper for X */
AccelStepper yStepper(AccelStepper::FULL4WIRE, 6, 9, 8, 7); /*!< stepper for Y */

/**
 * @brief detects when a note is on on the usb 
 * @param channel the channel on which to send the packet (0-15)
 * @param pitch the pitch of the note (48 = middle C)
 * @param velocity the veloctiy (64 = normal, 127 = fastest)
 */
void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

/**
 * @brief detects when a note is off on the usb 
 * @param channel the channel on which to send the packet (0-15)
 * @param pitch the pitch of the note (48 = middle C)
 * @param velocity the veloctiy (64 = normal, 127 = fastest)
 */
void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

/**
 * @brief A struct to hold x and y positions in steps
 */
struct point {
  int x; /*!< x location */
  int y; /*!< y location */
};

int solenoid = 15; /*!< solenoid pin */
int strum1 = A0;   /*!< pin for bit1 of com */
int strum2 = A1;   /*!< pin for bit2 of com */
int strum3 = A2;   /*!< pin for bit3 of com */

//---------------------------------------------------------------------

void setup() {
  /*!
   * @brief Sets up serial, IO pins and stepper settings
   */
  Serial.begin(115200);

  xStepper.setMaxSpeed(2000.0);
  xStepper.setAcceleration(1000.0);

  yStepper.setMaxSpeed(2000.0);
  yStepper.setAcceleration(1000.0);

  pinMode(solenoid, OUTPUT);
  pinMode(strum1, OUTPUT);
  pinMode(strum2, OUTPUT);
  pinMode(strum3, OUTPUT);
  digitalWrite(solenoid, LOW);
  digitalWrite(strum1, LOW);
  digitalWrite(strum2, LOW);
  digitalWrite(strum3, LOW);
}

/**
 * @brief channel the event type (0x0B = control change).
 * @param control the control number number (0-119).
 * @param value the control value (0-127).
 */
void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}

//---------------------------------------------------------------------

void loop() {
  /*!
   * @brief main loop. gets input from midi, converts to x,y
   *        ouputs solenoid and plucking
   */
  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    if (rx.header != 0) {
      //Serial.print("Received: ");
      //Serial.print(rx.header, HEX);
      //Serial.print("-");
      //Serial.print(rx.byte1, HEX);
      //Serial.print("-");
      Serial.println(rx.byte2, HEX);
      //Serial.print("-");
      //Serial.println(rx.byte3, HEX);

      switch(rx.byte2) {
        case 53:
          Serial.println("note is F");
          xStepper.moveTo(0);
          yStepper.moveTo(-560);
          keyDown = 1;
          string = 1;
          break;
        case 54:
          Serial.println("note is F#");
          xStepper.moveTo(-175);
          yStepper.moveTo(-554);
          keyDown = 1;
          string = 1;
          break;
        case 55:
          Serial.println("note is G");
          xStepper.moveTo(-340);
          yStepper.moveTo(-548);
          keyDown = 1;
          string = 1;
          break;
        case 56:
          Serial.println("note is G#");
          xStepper.moveTo(-485);
          yStepper.moveTo(-542);
          keyDown = 1;
          string = 1;
          break;
        case 57:
          Serial.println("note is A");
          xStepper.moveTo(-645);
          yStepper.moveTo(-536);
          keyDown = 1;
          string = 1;
          break;
        case 58:
          Serial.println("note is A#");
          xStepper.moveTo(0);
          yStepper.moveTo(-595);
          keyDown = 1;
          string = 2;
          break;
        case 59:
          Serial.println("note is B");
          xStepper.moveTo(-175);
          yStepper.moveTo(-590);
          keyDown = 1;
          string = 2;
          break;
        case 60:
          Serial.println("note is C");
          xStepper.moveTo(-340);
          yStepper.moveTo(-585);
          keyDown = 1;
          string = 2;
          break;
        case 61:
          Serial.println("note is C#");
          xStepper.moveTo(fret4);
          yStepper.moveTo(-580);
          keyDown = 1;
          string = 2;
          break;
        case 62:
          Serial.println("note is D");
          xStepper.moveTo(fret5);
          yStepper.moveTo(-575);
          keyDown = 1;
          string = 2;
          break;
        case 63:
          Serial.println("note is D#");
          xStepper.moveTo(fret1);
          yStepper.moveTo(-630);
          keyDown = 1;
          string = 3;
          break;
        case 64:
          Serial.println("note is E");
          xStepper.moveTo(fret2);
          yStepper.moveTo(-626);
          keyDown = 1;
          string = 3;
          break;
        case 65:
          Serial.println("note is F");
          xStepper.moveTo(fret3);
          yStepper.moveTo(-622);
          keyDown = 1;
          string = 3;
          break;
        case 66:
          Serial.println("note is F#");
          xStepper.moveTo(fret4);
          yStepper.moveTo(-618);
          keyDown = 1;
          string = 3;
          break;
        case 67:
          Serial.println("note is G");
          xStepper.moveTo(fret5);
          yStepper.moveTo(-614);
          keyDown = 1;
          string = 3;
          break;
        case 68:
          Serial.println("note is G#");
          xStepper.moveTo(fret1);
          yStepper.moveTo(-665);
          keyDown = 1;
          string = 4;
          break;
        case 7:
          Serial.println("HOME!");
          xStepper.moveTo(fret1);
          yStepper.moveTo(0);
          break;
      
      }

      if(rx.byte2 == 79)
      {
        strumCount++;
        
        if(strumCount%2 == 0)
        {
          Serial.println("strum");
          digitalWrite(strum1, !digitalRead(strum1)); 

          switch(string) {
            case 1:
              Serial.println("strum string 1");
              digitalWrite(strum2, LOW);
              digitalWrite(strum3, HIGH);
              break;
            case 2:
              Serial.println("strum string 2");
              digitalWrite(strum2, HIGH);
              digitalWrite(strum3, LOW);
              break;
            case 3:
              Serial.println("strum string 3");
              digitalWrite(strum2, HIGH);
              digitalWrite(strum3, HIGH);
              break; 
          }
        }
      } 

      if(rx.byte3 == 0 && rx.byte2 != 79) {
        keyDown = 0;
        Serial.println("key lifted");
      }

    }
  } while (rx.header != 0);

  xStepper.run();
  yStepper.run();

  if(!xStepper.isRunning() && !yStepper.isRunning() && keyDown) {
    digitalWrite(solenoid, HIGH);
    Serial.println("solenoid should be down");  
  }

  digitalWrite(solenoid, LOW);

}
