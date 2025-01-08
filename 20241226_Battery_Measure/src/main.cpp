#include <Arduino.h>

// Define the ADC pin
const int analogPin = 36; // GPIO34 (ADC1 Channel 6)
const float OFFSET = 0.11;
// ADC characteristics
const float referenceVoltage = 3.3; // ESP32 ADC reference voltage (3.3V)
const int adcResolution = 4095;     // 12-bit ADC resolution (0-4095)

void setup() {
  Serial.begin(115200);
  delay(1000); // Allow time for the Serial Monitor to open
  Serial.println("ESP32 Voltage Reading Example");
}

void loop() {
  // Read raw ADC value (0 - 4095)
  int adcValue = analogRead(analogPin);

  // Convert the raw ADC value to a voltage
  float voltage = ((adcValue * referenceVoltage) / adcResolution) - OFFSET;

  // Print the results
  Serial.print("ADC Value: ");
  Serial.print(adcValue);
  Serial.print("\tVoltage: ");
  Serial.print(voltage, 2); // Print with 2 decimal places
  Serial.println(" V");

  delay(1000); // Update every second
}
