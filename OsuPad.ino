#include <Mouse.h>
#include <EEPROM.h>
#include <Keyboard.h>

//Bindings
int buttonPins[] = {21, 3};
int ledPins[] = {16, 9};

struct Settings {
  char buttonOutputs[4];
  bool leds[4];
  bool isUseMouse[4] = {false, false, false, false};
  bool mouseClick[4];
  int mouseDelay[4];
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
  Mouse.begin();
  
  Serial.begin(115200);

  EEPROM.get(0, stg);


  /*
  stg.buttonOutputs[0] = 'd';
  stg.buttonOutputs[1] = 'f';
  stg.isUseMouse[0] = false;
  stg.isUseMouse[1] = false;
  stg.isUseMouse[2] = false;
  stg.isUseMouse[3] = false;
  stg.mouseClick[0] = false;
  stg.mouseClick[1] = false;
  stg.mouseClick[2] = false;
  stg.mouseClick[3] = false;
  stg.mouseDelay[0] = 50;
  stg.mouseDelay[1] = 50;
  stg.mouseDelay[2] = 50;
  stg.mouseDelay[3] = 50;
  EEPROM.put(0, stg);
  */
  
  Serial.println("[System] The controller is ready!");
}

void loop() {
  //For loop all keys
  for (int i = 0; i < btnCount; i++){
    //If button down
    if(digitalRead(buttonPins[i]) == LOW){
      if(stg.leds[i]){
        digitalWrite(ledPins[i], HIGH);
      }
      if(stg.isUseMouse[i]){
        if(stg.mouseClick[i]){
          Mouse.press(MOUSE_RIGHT);
          delay(stg.mouseDelay[i]);
          Mouse.release(MOUSE_RIGHT);
        }else{
          Mouse.press(MOUSE_LEFT);
          delay(stg.mouseDelay[i]);
          Mouse.release(MOUSE_LEFT);
        }
      }else{
        Keyboard.press(stg.buttonOutputs[i]);
      }
    }else{
      digitalWrite(ledPins[i], LOW);
      Keyboard.release(stg.buttonOutputs[i]);
    }
  }
  if(Serial.read() != -1){
    String function = Serial.readString();
    if(function.startsWith("connect")){
      Serial.print("1:" + String(stg.buttonOutputs[0]) + String(';') + String(stg.buttonOutputs[1]) + "!" + String(stg.leds[0]) + "@" + String(stg.leds[1]));
    }
    if(function.startsWith("setkey-left")) {
      String key = getValue(function, ' ', 1);
      if(stg.buttonOutputs[1] == key[0])
        return;
      stg.buttonOutputs[0] = key[0];
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("setkey-right")) {
      String key = getValue(function, ' ', 1);
      if(stg.buttonOutputs[1] == key[0])
        return;
      stg.buttonOutputs[1] = key[0];
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("config")) {;
      Serial.println("Left Key: "+ String(stg.buttonOutputs[0]));
      Serial.println("Right Key: "+ String(stg.buttonOutputs[1]));
    }
    if(function.startsWith("setled-left")) {
      String raw = getValue(function, ' ', 1);
      bool target = false;
      if(raw[0] == '1'){
        target = true;
      }
      if(stg.leds[0] == target){
        Serial.print("0");
        return;
      }
      stg.leds[0] = target;
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("setled-right")) {
      String raw = getValue(function, ' ', 1);
      bool target = false;
      if(raw[0] == '1'){
        target = true;
      }
      if(stg.leds[1] == target){
        Serial.print("0");
        return;
      }
        
      stg.leds[1] = target;
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("setmouse-left")) {
      String raw = getValue(function, ' ', 1);
      bool target = false;
      if(raw[0] == '1'){
        target = true;
      }
      if(stg.isUseMouse[0] == target){
        Serial.print("0");
        return;
      }
        
      stg.isUseMouse[0] = target;
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("setmouse-right")) {
      String raw = getValue(function, ' ', 1);
      bool target = false;
      if(raw[0] == '1'){
        target = true;
      }
      if(stg.isUseMouse[1] == target){
        Serial.print("0");
        return;
      }
        
      stg.isUseMouse[1] = target;
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("setmousekey-left")) {
      String raw = getValue(function, ' ', 1);
      bool target = false;
      if(raw[0] == '1'){
        target = true;
      }
      if(stg.mouseClick[0] == target){
        Serial.print("0");
        return;
      }
        
      stg.mouseClick[0] = target;
      EEPROM.put(0, stg);
      Serial.print("1");
    }
    if(function.startsWith("setmousekey-right")) {
      String raw = getValue(function, ' ', 1);
      bool target = false;
      if(raw[0] == '1'){
        target = true;
      }
      if(stg.mouseClick[1] == target){
        Serial.print("0");
        return;
      }
        
      stg.mouseClick[1] = target;
      EEPROM.put(0, stg);
      Serial.print("1");
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
