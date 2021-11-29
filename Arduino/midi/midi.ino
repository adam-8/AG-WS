#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

int buttonApin = 12;  //footswitch A

int analogpot1 = A0;  //knob 1

int analogpot1Old = 0;
int analogpot1New = 0;

#define analogpot1CC 54

void setup() {
  // put your setup code here, to run once:
  MIDI.begin (1); // MIDI START
  Serial.begin(115200);
  pinMode(buttonApin, INPUT_PULLUP);
  pinMode(analogpot1, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  static bool buttonAvalueOld = HIGH;

  //footswitches
  bool buttonAvalueNew = digitalRead(buttonApin);
  if (buttonAvalueNew != buttonAvalueOld) {
    if (buttonAvalueNew == LOW) {
      MIDI.sendNoteOn(60, 127, 1);
      Serial.println("Note C On");
    }
    else {
      MIDI.sendNoteOff(60, 0, 1);
      Serial.println("Note C Off");
    }
    buttonAvalueOld = buttonAvalueNew;
  }

  //potentiometers

  int pot1 = analogRead(A0);
  int analogpot1New = analogRead(A0);

  if (analogpot1New - analogpot1Old >= 35 || analogpot1Old - analogpot1New >= 35) {
    analogpot1Old = analogpot1New;
    analogpot1New = (map(analogpot1New, 1023, 0, 0, 127));
    analogpot1New = (constrain(analogpot1New, 0, 127));
    MIDI.sendControlChange(analogpot1CC, analogpot1New, 1);
    Serial.print ("pot: ");
    Serial.println(pot1);
    Serial.print("potread: ");
    Serial.println(analogpot1New);
  }
  delay(25);
}
