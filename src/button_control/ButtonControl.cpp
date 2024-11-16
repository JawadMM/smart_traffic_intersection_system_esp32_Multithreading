#include "ButtonControl.h"
#include <Arduino.h>
#include <FreeRTOS.h>

extern volatile bool trainIsApproaching;
extern SemaphoreHandle_t trainSemaphore;
extern SemaphoreHandle_t trainMutex;

// Interrupt handler for the button press
void IRAM_ATTR handleButtonPress() {
  // Take the mutex before modifying the shared resource
  if (xSemaphoreTakeFromISR(trainMutex, NULL)) {
    trainIsApproaching = true;  // Set the flag that the train is approaching
    xSemaphoreGiveFromISR(trainMutex, NULL);  // Release the mutex
  }
  
  // Give the semaphore to notify other tasks
  xSemaphoreGiveFromISR(trainSemaphore, NULL);  // Allow other tasks to proceed
}