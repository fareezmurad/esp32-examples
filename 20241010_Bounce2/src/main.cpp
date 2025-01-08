#include <Arduino.h>
/* 
 DESCRIPTION
 ====================
 This is an example of the Bounce2::Button class. 
 When the user presses a physical button, it toggles a LED on or off.
 The Button class matches an electrical state to a physical action. 
 Use .setPressedState(LOW or HIGH) to set the detection state for when the button is pressed.

 INSTRUCTIONS
 ====================
 Set BUTTON_PIN to the pin attached to the button.
 Set LED_PIN to the pin attached to a LED.
 
 */
 
// Include the Bounce2 library found here :
// https://github.com/thomasfredericks/Bounce2
#include <Bounce2.h>

// INSTANTIATE A Button OBJECT
Bounce2::Button button = Bounce2::Button();

// WE WILL attach() THE BUTTON TO THE FOLLOWING PIN IN setup()
#define BUTTON_PIN 19

// DEFINE THE PIN FOR THE LED :
// 1) SOME BOARDS HAVE A DEFAULT LED (LED_BUILTIN)
//#define LED_PIN LED_BUILTIN
// 2) OTHERWISE SET YOUR OWN PIN
#define LED_PIN 23

// SET A VARIABLE TO STORE THE LED STATE
bool ledState = LOW;

void setup() {

  // BUTTON SETUP 
  
  // SELECT ONE OF THE FOLLOWING :
  // 1) IF YOUR BUTTON HAS AN INTERNAL PULL-UP
  // button.attach( BUTTON_PIN ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  // 2) IF YOUR BUTTON USES AN EXTERNAL PULL-UP
  button.attach( BUTTON_PIN, INPUT_PULLUP ); // USE EXTERNAL PULL-UP

  // DEBOUNCE INTERVAL IN MILLISECONDS
  button.interval(5); 

  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button.setPressedState(LOW); 
  
  // LED SETUP
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,ledState);

}

void loop() {
  // UPDATE THE BUTTON
  // YOU MUST CALL THIS EVERY LOOP
  button.update();

  if ( button.pressed() ) {
    
    // TOGGLE THE LED STATE : 
    ledState = !ledState; // SET ledState TO THE OPPOSITE OF ledState
    digitalWrite(LED_PIN,ledState);

  }
}