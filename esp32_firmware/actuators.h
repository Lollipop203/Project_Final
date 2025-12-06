#ifndef ACTUATORS_H
#define ACTUATORS_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"

// ============================================
// Global Variables
// ============================================
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLS, LCD_ROWS);

// Actuator states
bool pumpState = false;
bool ledState = false;

// ============================================
// Initialize Actuators
// ============================================
void initActuators() {
  // Initialize relay pins
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(LED_RELAY_PIN, OUTPUT);
  
  // Turn off relays initially
  digitalWrite(PUMP_RELAY_PIN, LOW);
  digitalWrite(LED_RELAY_PIN, LOW);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  
  Serial.println("✓ Actuators initialized");
}

// ============================================
// Control Water Pump
// ============================================
void controlPump(bool state) {
  pumpState = state;
  digitalWrite(PUMP_RELAY_PIN, state ? HIGH : LOW);
  
  Serial.printf("💧 Pump: %s\n", state ? "ON" : "OFF");
}

// ============================================
// Control LED Light
// ============================================
void controlLED(bool state) {
  ledState = state;
  digitalWrite(LED_RELAY_PIN, state ? HIGH : LOW);
  
  Serial.printf("💡 LED: %s\n", state ? "ON" : "OFF");
}

// ============================================
// Update LCD Display
// ============================================
void updateLCD(float temp, float hum, int soil, int light) {
  static unsigned long lastUpdate = 0;
  static int displayMode = 0;
  
  // Update every 3 seconds, rotate display
  if (millis() - lastUpdate > 3000) {
    lastUpdate = millis();
    lcd.clear();
    
    switch (displayMode) {
      case 0:
        // Display temperature and humidity
        lcd.setCursor(0, 0);
        lcd.printf("Temp: %.1fC", temp);
        lcd.setCursor(0, 1);
        lcd.printf("Hum: %.1f%%", hum);
        break;
        
      case 1:
        // Display soil moisture and light
        lcd.setCursor(0, 0);
        lcd.printf("Soil: %d%%", soil);
        lcd.setCursor(0, 1);
        lcd.printf("Light: %d", light);
        break;
        
      case 2:
        // Display actuator status
        lcd.setCursor(0, 0);
        lcd.printf("Pump: %s", pumpState ? "ON " : "OFF");
        lcd.setCursor(0, 1);
        lcd.printf("LED: %s", ledState ? "ON " : "OFF");
        break;
    }
    
    displayMode = (displayMode + 1) % 3;
  }
}

// ============================================
// Display WiFi Status on LCD
// ============================================
void displayWiFiStatus(bool connected) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi:");
  lcd.setCursor(6, 0);
  lcd.print(connected ? "Connected" : "Connecting");
}

// ============================================
// Display Firebase Status on LCD
// ============================================
void displayFirebaseStatus(bool connected) {
  lcd.setCursor(0, 1);
  lcd.print("Firebase:");
  lcd.setCursor(10, 1);
  lcd.print(connected ? "OK" : "...");
}

// ============================================
// Display Error on LCD
// ============================================
void displayError(const char* message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ERROR:");
  lcd.setCursor(0, 1);
  lcd.print(message);
}

// ============================================
// Get Pump State
// ============================================
bool getPumpState() {
  return pumpState;
}

// ============================================
// Get LED State
// ============================================
bool getLEDState() {
  return ledState;
}

#endif // ACTUATORS_H
