#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

// Define LED pins
#define LED1_PIN 25
#define LED2_PIN 26
#define LED3_PIN 27
#define LED4_PIN 14

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x10, 0x06, 0x1C, 0xB5, 0x43, 0xE0};

// Structure to receive data
typedef struct struct_message
{
  bool toggleSwitch[4]; // Control 4 LEDs
} struct_message;

struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// Callback function when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);

  // Print received toggle states
  for (int i = 0; i < 4; i++)
  {
    Serial.print("Switch ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(myData.toggleSwitch[i]);
  }

  // Update LED states
  digitalWrite(LED1_PIN, myData.toggleSwitch[0]);
  digitalWrite(LED2_PIN, myData.toggleSwitch[1]);
  digitalWrite(LED3_PIN, myData.toggleSwitch[2]);
  digitalWrite(LED4_PIN, myData.toggleSwitch[3]);
}

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize LED pins as output
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(LED3_PIN, OUTPUT);
  pinMode(LED4_PIN, OUTPUT);

  // Set all LEDs off initially
  digitalWrite(LED1_PIN, LOW);
  digitalWrite(LED2_PIN, LOW);
  digitalWrite(LED3_PIN, LOW);
  digitalWrite(LED4_PIN, LOW);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register for receive callback
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()
{
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK) Serial.println("Sent with success");
  else Serial.println("Error sending the data");
  delay(50);
}
