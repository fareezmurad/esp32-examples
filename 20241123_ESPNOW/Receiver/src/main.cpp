#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#define LED_PIN 2
#define PUSH_BUTTON 32

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  bool ledState;
} struct_message;

// Create a struct_message called myData
struct_message myData;

int ledState = 0;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("button state: ");
  Serial.println(myData.ledState);
  
  if (myData.ledState == 0) ledState = !ledState;
}
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  uint8_t buttonState = digitalRead(PUSH_BUTTON);
  if(buttonState == LOW) ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
}