#include <Arduino.h>
#include <Bounce2.h>
#include "IRSharp.h"

#define button_pin 19
Bounce2::Button button = Bounce2::Button();

void setup() {
  Serial.begin(115200);

  button.attach(button_pin, INPUT_PULLUP);
  button.interval(5);
  button.setPressedState(LOW);
}

void loop() {
  button.update();
  if (button.pressed()) {
    if (!acState) turnOn();
    else turnOff();
  }
}
