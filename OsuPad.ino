#include <EEPROM.h>
#include <Keyboard.h>

//Bindings
int buttonPins[] = {21, 3};
int ledPins[] = {16, 9};

struct Settings {
  char buttonOutputs[4];
};

//Array Length
int btnCount = sizeof(buttonPins) / sizeof(buttonPins[0]);

//Setting
Settings stg;

void setup() {
  //Set pinMode to all keys
  for (int i = 0; i < btnCount; i++){
    pinMode(buttonPins[i], INPUT_PULLUP);
    pinMode(ledPins[i], OUTPUT);
  }
  Keyboard.begin();
  Serial.begin(115200);

  EEPROM.get(0, stg);
  if(stg.buttonOutputs[0] == '\0' && stg.buttonOutputs[1] == '\0'){
    stg.buttonOutputs[0] = 'd';
    stg.buttonOutputs[1] = 'f';
  }
  Serial.println("[] The controller is ready!");
}

void loop() {
  //For loop all keys
  for (int i = 0; i < btnCount; i++){
    //If button down
    if(digitalRead(buttonPins[i]) == LOW){
      digitalWrite(ledPins[i], HIGH);
      Keyboard.press(stg.buttonOutputs[i]);
    }else{
      digitalWrite(ledPins[i], LOW);
      Keyboard.release(stg.buttonOutputs[i]);
    }
  }
  if(Serial.read() != -1){
    String function = Serial.readString();
    if(function.startsWith("connect")){
      Serial.print("1:" + String(stg.buttonOutputs[0]) + String(';') + String(stg.buttonOutputs[1]));
    }
    if(function.startsWith("setkey-left")) {
      String key = getValue(function, ' ', 1);
      stg.buttonOutputs[0] = key[0];
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("setkey-right")) {
      String key = getValue(function, ' ', 1);
      stg.buttonOutputs[1] = key[0];
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("config")) {;
      Serial.println("Left Key: "+ String(stg.buttonOutputs[0]));
      Serial.println("Right Key: "+ String(stg.buttonOutputs[1]));
    }
  }
  delay(1);
}

String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
