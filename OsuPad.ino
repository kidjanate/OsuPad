#include <Keyboard.h>

//Bindings
int buttonPins[] = {21, 3};
int ledPins[] = {16, 9};
char buttonOutputs[] = {'d', 'f'};

//Array Length
int btnCount = sizeof(buttonPins) / sizeof(buttonPins[0]);

void setup() {
  //Set pinMode to all keys
  for (int i = 0; i < btnCount; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  Keyboard.begin();
}

void loop() {
  //For loop all keys
  for (int i = 0; i < btnCount; i++){
    //If button down
    if(digitalRead(buttonPins[i]) == LOW){
      digitalWrite(ledPins[i], HIGH);
      Keyboard.press(buttonOutputs[i]);
    }else{
      digitalWrite(ledPins[i], LOW);
      Keyboard.release(buttonOutputs[i]);
    }
  }
}
