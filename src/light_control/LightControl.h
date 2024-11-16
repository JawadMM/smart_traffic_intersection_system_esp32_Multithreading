#ifndef LIGHT_CONTROL_H
#define LIGHT_CONTROL_H

// Declare functions
void setRGBColor(int rgbPins[], const int color[]);
void turnAllLightsRed();
void startLightsCycle(int greenDurationA);
void blinkBuiltInLED(int seconds);
#endif