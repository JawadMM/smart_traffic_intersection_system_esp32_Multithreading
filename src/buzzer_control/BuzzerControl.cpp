#include "BuzzerControl.h"
#include "../setup/ConfigurePins.h"
#include "../button_control/ButtonControl.h"
#include "../light_control/LightControl.h"
#include <Arduino.h>

void activateTrainSound(int seconds) {
    unsigned long startMillis = millis();  // Start time for duration
    unsigned long currentMillis;
    int ledBrightness = 0;  // Initial brightness for PWM
    bool increasing = true;  // Flag to track brightness direction
    int buzzerFrequency = 500; // Initial buzzer frequency

    while (millis() - startMillis < seconds * 1000) {
        currentMillis = millis();

        // Change LED brightness using PWM
        if (increasing) {
            ledBrightness += 5;  // Increase brightness
            if (ledBrightness >= 255) {
                ledBrightness = 255;  // Cap at maximum brightness
                increasing = false;  // Change direction
            }
        } else {
            ledBrightness -= 5;  // Decrease brightness
            if (ledBrightness <= 0) {
                ledBrightness = 0;  // Cap at minimum brightness
                increasing = true;  // Change direction
            }
        }

        // Update buzzer frequency based on LED brightness
        buzzerFrequency = map(ledBrightness, 0, 255, 300, 800); // Map brightness to frequency (300Hz to 800Hz)
        tone(BUZZER_PIN, buzzerFrequency);  // Set buzzer frequency based on brightness

        // Set RGB LED color using PWM 
        // NOTE: The red color in this project is not pure red, but a mixture between red and blue
        int blueChannel = map(ledBrightness, 0, 255, 0, 14); // Map brightness to blue channel
        neopixelWrite(RGB_BUILTIN, ledBrightness, 0, blueChannel);

        delay(50);  // Small delay for smooth brightness change
    }

    // Deactivate the buzzer
    noTone(BUZZER_PIN);
    
    // Ensure the LED is turned off when done
    neopixelWrite(RGB_BUILTIN, 0, 0, 0);  // Turn off the built-in RGB LED
}

