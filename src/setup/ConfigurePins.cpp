#include "ConfigurePins.h"
#include "../button_control/ButtonControl.h"
#include <Arduino.h>


// Create a function to do the initial pins setup
void configurePins() {
  // INPUT PINS SETUP
  // Setup for potentiometer
  pinMode(POTENTIOMETER_PIN, INPUT);

  // Setup for button
  pinMode(PUSH_BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PUSH_BUTTON_PIN), handleButtonPress, FALLING);

  //OUTPUT PINS SETUP
  // Setup for RGB LED Lights
  int pinsA[] = RGB_LED_A_PINS;
  for (int i = 0; i < 3; i++) {
    pinMode(pinsA[i], OUTPUT);
  }

  int pinsB[] = RGB_LED_B_PINS;
  for (int i = 0; i < 3; i++) {
    pinMode(pinsB[i], OUTPUT);
  }
  
  // Setup for buzzer
  pinMode(BUZZER_PIN, OUTPUT);
}
