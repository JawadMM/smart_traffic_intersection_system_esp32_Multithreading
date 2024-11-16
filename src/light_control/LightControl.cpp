#include "LightControl.h"
#include <Arduino.h>
#include "../setup/ConfigurePins.h"
#include "../button_control/ButtonControl.h"

// initialize variables
int RED_COLOR[] = {255, 0, 14};
int YELLOW_COLOR[] = {255, 119, 0};
int GREEN_COLOR[] = {0, 150, 0};
// int OFF[] = {0, 0, 0};

// Get RGB LED pins
int pinsA[] = RGB_LED_A_PINS;
int pinsB[] = RGB_LED_B_PINS;

// Timing durations in milliseconds
const unsigned long TOTAL_GREEN_DURATION = 30 * 1000; // Total green light duration is 30 seconds (split between the lights using the potentiometer value)
const unsigned long YELLOW_DURATION = 3 * 1000; // Yellow light duration: 3 seconds

unsigned long previousMillis = 0; // Store last time state changed

// Variables to manage state
enum TrafficLightState {GREEN, YELLOW, RED};
TrafficLightState lightAState = GREEN;
TrafficLightState lightBState = RED;

// Create a function to change RGB LED light color given its pins and a color
void setRGBColor(int rgbPins[], int color[]) {
    // Set the light color for each pin of the RGB LED light
    for (int i = 0; i < 3; i++) {
        analogWrite(rgbPins[i], 255 - color[i]); // 255 - [color] because the anode is connected to the voltage
    }
}

// Create a function to change the lights color according to the state
void setTrafficLightsColor() {
    if (lightAState == GREEN) {
        setRGBColor(pinsA, GREEN_COLOR); // Light A Green
        setRGBColor(pinsB, RED_COLOR); // Light B Red
        // Serial.println("A green");

    } else if (lightAState == YELLOW) {
        setRGBColor(pinsA, YELLOW_COLOR); // Light A Yellow
        setRGBColor(pinsB, RED_COLOR); // Light B Red
        // Serial.println("A yellow");

    } else if (lightAState == RED && lightBState == GREEN) {
        setRGBColor(pinsA, RED_COLOR); // Light A Red
        setRGBColor(pinsB, GREEN_COLOR); // Light B Green
        // Serial.println("A red");

    } else if (lightBState == YELLOW) {
        setRGBColor(pinsA, RED_COLOR); // Light A Red
        setRGBColor(pinsB, YELLOW_COLOR); // Light B Yellow
        // Serial.println("B yellow");

    } else if (lightBState == RED) {
        setRGBColor(pinsA, GREEN_COLOR); // Light A Green
        setRGBColor(pinsB, RED_COLOR); // Light B Red
        // Serial.println("B red");
    }
}

// Create a function to start the traffic cycle given a duration for the first traffic light
void startLightsCycle(int greenDurationA) {
    // Update the traffic light colors
    setTrafficLightsColor();

    // Calculate the green duration for light B
    int greenDurationB = TOTAL_GREEN_DURATION - greenDurationA * 1000;

    // Get the current time to change states accordingly
    unsigned long currentMillis = millis();

    // Manage the state transition
    if (lightAState == GREEN && currentMillis - previousMillis >= greenDurationA * 1000) {
        // Switch to yellow after green duration
        lightAState = YELLOW;
        previousMillis = currentMillis;
        Serial.print("Turning A YELLOW for ");
        Serial.print(YELLOW_DURATION / 1000);
        Serial.println("s");


    } else if (lightAState == YELLOW && currentMillis - previousMillis >= YELLOW_DURATION) {
        // Switch A to red after yellow duration
        lightAState = RED;
        // Switch B to green
        lightBState = GREEN;
        previousMillis = currentMillis;

        Serial.print("Turning B GREEN for ");
        Serial.print(greenDurationB / 1000);
        Serial.println("s");
        Serial.print("Turning A RED for ");
        Serial.print((greenDurationB + YELLOW_DURATION) / 1000);
        Serial.println("s");

    } else if (lightBState == GREEN && currentMillis - previousMillis >= greenDurationB) {
        // Switch B to yellow after green duration
        lightBState = YELLOW;
        previousMillis = currentMillis;

        Serial.print("Turning B YELLOW for ");
        Serial.print(YELLOW_DURATION / 1000);
        Serial.println("s");

    } else if (lightBState == YELLOW && currentMillis - previousMillis >= YELLOW_DURATION) {
        // Switch B to red after yellow duration
        lightBState = RED;
        lightAState = GREEN;
        previousMillis = currentMillis;

        Serial.print("Turning A GREEN for ");
        Serial.print(greenDurationA);
        Serial.println("s");
        Serial.print("Turning B RED for ");
        Serial.print((greenDurationA * 1000 + YELLOW_DURATION) / 1000);
        Serial.println("s");

    } else if (lightAState == RED && lightBState == RED) {
        // Back to the inital cycle starting with light A being green
        lightAState = GREEN;
        previousMillis = currentMillis;

        Serial.print("Turning A GREEN for ");
        Serial.print(greenDurationA);
        Serial.println("s");
        Serial.print("Turning B RED for ");
        Serial.print((greenDurationA * 1000 + YELLOW_DURATION) / 1000);
        Serial.println("s");
    }

    // Small delay so the RGB LED lights can switch colors
    delay(10);
    
}

void turnAllLightsRed() {
    // Turn all the lights red
    setRGBColor(pinsA, RED_COLOR);
    setRGBColor(pinsB, RED_COLOR);
    lightAState = RED;
    lightBState = RED;
}

void blinkBuiltInLED(int seconds) {
    unsigned long startMillis = millis();  // Start time of blinking
    unsigned long previousBuilInLEDMillis = 0;
    bool ledState = false;  // Tracks whether LED is on or off
    
    while (millis() - startMillis <= seconds * 1000) {
        unsigned long currentMillis = millis();  // Update current time
        
        if (currentMillis - previousBuilInLEDMillis >= 500) {
            // Toggle LED state every 500ms
            if (ledState) {
                neopixelWrite(RGB_BUILTIN, 0, 0, 0); // Turn off the built-in RGB
            } else {
                neopixelWrite(RGB_BUILTIN, 255, 0, 14); // Turn on the built-in RGB (Red)
            }
            ledState = !ledState;  // Toggle LED state
            previousBuilInLEDMillis = currentMillis; // Reset time tracker for next toggle
        }
    }

    // Ensure LED is turned off when the loop exits
    neopixelWrite(RGB_BUILTIN, 0, 0, 0);
}