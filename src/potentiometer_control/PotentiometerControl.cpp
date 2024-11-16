#include "../setup/ConfigurePins.h"
#include <Arduino.h>

// Create a function to read the value of the potentiometer that will be between 0 and 4095
int readPotentiometerValue() {
  // Read potentiometer value
  int readValue = analogRead(POTENTIOMETER_PIN);
  // Serial.print("Potentiometer Value: ");
  // Serial.println(readValue);
  // Convert the readValue into a value between 10 and 20
  int convertedValue = map(readValue, 0, 4095, 10, 20);
  // Serial.print("Converted Potentiometer Value: ");
  // Serial.println(convertedValue);

  // Set the initialized variable to the converted value
  return convertedValue;
}

