#include <ir_Sharp.h>
#include "IRSharp.h"

IRSharpAc ac(27);  // Initialize IRSharpAc object with GPIO 27

bool acState = false;  // Initialize acState here to avoid extern complications

void turnOn() {
  ac.setMode(kSharpAcCool);
  ac.setTemp(20);
  ac.setFan(kSharpAcFanAuto);
  ac.on();
  ac.send();
  acState = true;
}

void turnOff() {
  ac.off();
  ac.send();
  acState = false;
}
