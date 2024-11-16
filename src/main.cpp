#include <Arduino.h>
#include <FreeRTOS.h>
#include "./button_control/ButtonControl.h"
#include "./buzzer_control/BuzzerControl.h"
#include "./light_control/LightControl.h"
#include "./potentiometer_control/PotentiometerControl.h"
#include "./lcd_control/LCDControl.h"
#include "./setup/ConfigurePins.h"

// Task Handles
TaskHandle_t buttonControlTaskHandle = NULL;
TaskHandle_t trafficCycleTaskHandle = NULL;
TaskHandle_t potentiometerReadingTaskHandle = NULL;
TaskHandle_t potentiometerPrintTaskHandle = NULL;
TaskHandle_t systemStatusTaskHandle = NULL;

// Flag to indicate train status
volatile bool trainIsApproaching = false;

// Potentiometer value
int potentiometerValue;

// Set train interval
const int trainAppraochInterval = 15;

// Semaphore and Mutex
SemaphoreHandle_t trainSemaphore;
SemaphoreHandle_t trainMutex;

// Task to handle the button press
void buttonControlTask(void *pvParameters) {
  // Wait for semaphore indicating train is approaching
  while (true) {
    if (xSemaphoreTake(trainSemaphore, portMAX_DELAY) == pdTRUE) {
      Serial.println("Train Approaching...");
      Serial.println("TURNING LIGHTS RED...");
      // Use mutex to safely update train status
      xSemaphoreTake(trainMutex, portMAX_DELAY);
      trainIsApproaching = true;
      xSemaphoreGive(trainMutex);

      activateTrainSound(trainAppraochInterval);  // Start the sound sequence

      // Use mutex to safely update train status after train has passed
      xSemaphoreTake(trainMutex, portMAX_DELAY);
      trainIsApproaching = false;
      xSemaphoreGive(trainMutex);

      Serial.println("Train Passed. Lights back to cycle...");
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

// Task to handle traffic cycle (based on potentiometer value)
void trafficCycleTask(void *pvParameters) {
  while (true) {
    xSemaphoreTake(trainMutex, portMAX_DELAY);  // Take mutex for safe access to train status
    if (!trainIsApproaching) {
      startLightsCycle(potentiometerValue);  // Control traffic lights with potentiometer value
    } else {
      turnAllLightsRed();  // Stop the traffic cycle by turning all lights red
    }
    xSemaphoreGive(trainMutex);  // Release mutex after checking train status

    vTaskDelay(500 / portTICK_PERIOD_MS);  // Update traffic cycle every 500 ms
  }
}

// Task to read potentiometer values
void potentiometerReadingTask(void *pvParameters) {
  while (true) {
    // Continuously update potentiometer value
    potentiometerValue = readPotentiometerValue();
    vTaskDelay(200 / portTICK_PERIOD_MS);  // Read every 200ms
  }
}


// Task to print the potentiometer value
void potentiometerPrintTask(void *pvParameters) {
  while (true) {
    Serial.print("Potentiometer Value: ");
    Serial.println(potentiometerValue);  // Print the potentiometer value
    vTaskDelay(5000 / portTICK_PERIOD_MS);  // Print every 5 second
  }
}

// Task to monitor and log system status
void systemStatusTask(void *pvParameters) {
  while (true) {

    displaySystemStatus(trainIsApproaching);
    
    vTaskDelay(1000 / portTICK_PERIOD_MS);  // Print every 1 second
  }
}

void setup() {
  Serial.begin(9600);
  
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  // Initialize the semaphore and mutex
  trainSemaphore = xSemaphoreCreateBinary();
  trainMutex = xSemaphoreCreateMutex();
  
  // Configure Pins
  configurePins();
  
  // Create Tasks
  xTaskCreate(buttonControlTask, "Button Control Task", 2048, NULL, 3, NULL);
  xTaskCreate(trafficCycleTask, "Traffic Cycle Task", 2048, NULL, 2, NULL);
  xTaskCreate(potentiometerReadingTask, "Potentiometer Reading Task", 2048, NULL, 1, NULL);
  xTaskCreate(potentiometerPrintTask, "Potentiometer Print Task", 2048, NULL, 1, &potentiometerPrintTaskHandle);
  xTaskCreate(systemStatusTask, "System Status Task", 2048, NULL, 1, &systemStatusTaskHandle);


  Serial.println("Setup complete.");
}

void loop() {
  
}
