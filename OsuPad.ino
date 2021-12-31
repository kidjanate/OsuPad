#include <Keyboard.h>


int buttonPins[] = {21, 3};
int ledPins[] = {16, 9};
char buttonOutputs[] = {'f', 'd'};

int btnCount = sizeof(buttonPins) / sizeof(buttonPins[0]);

void setup() {
  for (int i = 0; i < btnCount; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  Keyboard.begin();
}

void loop() {
  for (int i = 0; i < btnCount; i++){
    if(digitalRead(buttonPins[i]) == LOW){
      digitalWrite(ledPins[i], HIGH);
      Keyboard.press(buttonOutputs[i]);
    }else{
      digitalWrite(ledPins[i], LOW);
      Keyboard.release(buttonOutputs[i]);
    }
  }
}
