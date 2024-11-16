#include <Arduino.h>
#include <LiquidCrystal.h>
#include "../setup/ConfigurePins.h"
#include <FreeRTOS.h>

LiquidCrystal lcd(LCD_PINS);

void displaySystemStatus(bool trainIsApproching) {
  lcd.setCursor(0, 0);
  lcd.print(trainIsApproching? "Train Is Coming ": "No Train Coming ");
}

void displayLightsStatus(int greenDurationA, int greenDurationB) {
  lcd.setCursor(0, 1);
  lcd.print("A: ");
  lcd.print(greenDurationA);

  lcd.setCursor(8, 1);
  lcd.print("B: ");
  lcd.print(greenDurationB / 1000);
}

void configureLCD() {
  lcd.begin(16, 2);
}