#ifndef BUTTON_CONTROL_H
#define BUTTON_CONTROL_H
#include <Arduino.h>

// Flag to indicate train status
extern volatile bool trainIsApproaching;

// Declare the interrupt service routine (ISR)
void IRAM_ATTR handleButtonPress();
void buttonControlTask(void *pvParameters);
#endif
