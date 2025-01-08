#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <ESP32Encoder.h>
#include <Bounce2.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); // OLED Constructor
ESP32Encoder encoder; // Encoder constructor
Bounce2::Button encoderSW = Bounce2::Button();

#define CLK 26 // CLK ENCODER 
#define DT 27 // DT ENCODER 
#define SW 17 // SW ENCODER

// 'selected_item', 128x21px
const unsigned char selected_item [] PROGMEM = {
  0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x03
};

// 'scrollbar_bg', 8x64px
const unsigned char scrollbar_bg [] PROGMEM = { 
  0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x40, 0x00, 0x00
};
/* END OF BITMAP VARIABLES*/


const int MAX_ITEM_LENGTH = 20; // maximum characters for the item name
const char menu_items [] [MAX_ITEM_LENGTH] = {  // array with item names
  "Home Automation",
  "OLED Animation",
  "Progress Bar",
  "IR Transmitter",
  "IR Receiver",
  "IR Remote",
  "Rotary Encoder"
 };

const int itemCounts = sizeof(menu_items) / sizeof(menu_items[0]); // Formula to get the size of Array


 /* Initialize all items state in here */
int itemSelected = 0;
int itemSelPrevious;
int itemSelNext;

int currentScreen = 0;

int encoderCurrentRead = 0;
int encoderPrevRead;

int iconWidth = 50;
int iconHeight = 50;
int screenWidth = 128;

int progress = 0; // For Progress Bar item
/* END OF INITIALIZE ITEMS */

void setup () {
  Serial.begin(115200); // Just in case for debugging

  u8g2.begin(); // Begin the OLED
  u8g2.setBitmapMode(1); // Set transparent background for bitmap
  u8g2.setFontMode(1); // Set transparent background for text

  encoder.attachFullQuad(DT, CLK); // Set input pin for encoder
  encoder.setCount(0); // Start encoder step count

  encoderSW.attach(SW, INPUT_PULLUP); // Set SW as input
  encoderSW.interval(5); // Debounce time
  encoderSW.setPressedState(LOW); // SW active state
}

void loop (){
  int stateSW = digitalRead(SW); // Variable to read SW state
  encoderCurrentRead = encoder.getCount() / 2; // Get current encoder step
  encoderSW.update();

  // Switching between screen
  if (encoderSW.pressed()) {
    if (currentScreen == 0) currentScreen = 1;
    else currentScreen = 0;
  }

  u8g2.clearBuffer();

  if (currentScreen == 0) {
    // Scroll through the menu
    if (encoderCurrentRead > encoderPrevRead) {
      itemSelected += 1;
      if (itemSelected >= itemCounts) itemSelected = 0;
    }

    if (encoderPrevRead > encoderCurrentRead) {
      itemSelected -= 1;
      if (itemSelected < 0) itemSelected = itemCounts - 1;
    }
    
    // Draw the selected item bitmap
    u8g2.drawXBMP(0, 22, 128, 21, selected_item);
    u8g2.drawXBMP(128-8, 0, 8, 64, scrollbar_bg);

    // Draw the previous item
    u8g2.setFont(u8g_font_7x14);
    u8g2.drawStr(7, 15, menu_items[itemSelPrevious]); 

    // Draw the selected item in bold font
    u8g2.setFont(u8g_font_7x14B);    
    u8g2.drawStr(8, 15+20+2, menu_items[itemSelected]);   

    // Draw the next item
    u8g2.setFont(u8g_font_7x14);     
    u8g2.drawStr(7, 15+20+20+2+2, menu_items[itemSelNext]);  

    // Draw scrollbar handle
    u8g2.drawBox(125, 64/itemCounts * itemSelected, 3, 64/itemCounts); 
  }

  else if (currentScreen == 1) {

    // Sub menu for item 1
    if (menu_items[itemSelected] == menu_items[0]) {
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(13, 37, "Under Development");
    }

    // Sub menu for item 2
    if (menu_items[itemSelected] == menu_items[1]) {
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(13, 37, "Under Development");
    }

    // Sub menu for Progress Bar
    if (strcmp(menu_items[itemSelected], "Progress Bar") == 0) { // The right way to compare char Array
      if (encoderCurrentRead > encoderPrevRead) {
        progress += 5;
        if (progress > 120) progress = 120;
      }
      if (encoderPrevRead > encoderCurrentRead) {
        progress -= 5;
        if (progress < 0) progress = 0;
      }

      u8g2.drawFrame(2, 10, 124, 24);
      u8g2.setFont(u8g2_font_profont15_tr);
      u8g2.drawStr(24, 50, "Progress Bar");
      u8g2.drawBox(4, 12, progress, 20);

      // Calculate the percentage based on the progress
      int percentage = map(progress, 0, 120, 0, 100); // Map progress to a percentage (0 to 100)
      // Convert the percentage to a string and draw it
      char buffer[8]; // Buffer to hold the string representation of percentage
      sprintf(buffer, "%d%%", percentage); // Convert integer to string and add percentage sign
      // Set the draw color and font for the dynamic progress value
      u8g2.setDrawColor(2); // XOR mode
      u8g2.setFont(u8g2_font_t0_15b_tr);
      u8g2.drawStr(52, 28, buffer); // Draw the dynamic progress value with percentage
    }

    // Sub menu for item 4
    if (menu_items[itemSelected] == menu_items[3]) {
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(13, 37, "Under Development");
    }

    // Sub menu for item 5
    if (menu_items[itemSelected] == menu_items[4]) {
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(13, 37, "Under Development");
    }

    // Sub menu for item 6
    if (menu_items[itemSelected] == menu_items[5]) {
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(13, 37, "Under Development");
    }

    // Sub menu for item 7
    if (menu_items[itemSelected] == menu_items[6]) {
      u8g2.setFont(u8g2_font_6x13_tr);
      u8g2.drawStr(13, 37, "Under Development");
    }
  }

  // Update the previous and next item indices
  itemSelPrevious = itemSelected - 1;
  if (itemSelPrevious < 0) {
    itemSelPrevious = itemCounts - 1;  // Wrap around to the last item
  }
  
  itemSelNext = itemSelected + 1;
  if (itemSelNext >= itemCounts) {
    itemSelNext = 0;  // Wrap around to the first item
  }

  encoderPrevRead = encoderCurrentRead; // Update the encoder step
  u8g2.sendBuffer();
}