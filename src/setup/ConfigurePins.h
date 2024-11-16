#ifndef INPUT_SETUP_H
#define INPUT_SETUP_H

// Assign pins
#define RGB_LED_A_PINS {4, 5, 6} // {R, G, B}
#define RGB_LED_B_PINS {15, 16, 17} // {R, G, B}
#define POTENTIOMETER_PIN 1
#define PUSH_BUTTON_PIN 21
#define BUZZER_PIN 19
#define LCD_PINS {42, 41, 40, 39, 38, 37}

// Declare functions
void configurePins();

#endif 