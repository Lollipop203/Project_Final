#ifndef SENSORS_H
#define SENSORS_H

#include <DHT.h>
#include "config.h"

// ============================================
// Global Variables
// ============================================
DHT dht(DHT_PIN, DHT_TYPE);

// Sensor data structure
struct SensorData {
  float temperature;
  float humidity;
  int soilMoisture;
  int lightLevel;
  bool isValid;
};

// ============================================
// Initialize Sensors
// ============================================
void initSensors() {
  // Initialize DHT sensor
  dht.begin();
  
  // Configure ADC pins
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  
  Serial.println("✓ Sensors initialized");
}

// ============================================
// Read DHT Sensor (Temperature & Humidity)
// ============================================
bool readDHT(float &temp, float &hum) {
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  
  // Check if readings are valid
  if (isnan(temp) || isnan(hum)) {
    Serial.println("✗ Failed to read DHT sensor");
    return false;
  }
  
  return true;
}

// ============================================
// Read Soil Moisture Sensor
// ============================================
int readSoilMoisture() {
  int rawValue = analogRead(SOIL_MOISTURE_PIN);
  
  // Convert to percentage (0-100%)
  // Assuming: 4095 = dry (0%), 0 = wet (100%)
  int moisture = map(rawValue, 4095, 0, 0, 100);
  moisture = constrain(moisture, 0, 100);
  
  return moisture;
}

// ============================================
// Read Light Sensor
// ============================================
int readLightSensor() {
  int rawValue = analogRead(LIGHT_SENSOR_PIN);
  
  // Convert to lux or keep as raw value (0-4095)
  // For simplicity, we'll use raw value
  // Higher value = more light
  int lightLevel = map(rawValue, 0, 4095, 0, 1023);
  
  return lightLevel;
}

// ============================================
// Read All Sensors
// ============================================
SensorData readAllSensors() {
  SensorData data;
  
  // Read DHT sensor
  data.isValid = readDHT(data.temperature, data.humidity);
  
  // Read soil moisture
  data.soilMoisture = readSoilMoisture();
  
  // Read light sensor
  data.lightLevel = readLightSensor();
  
  // Print sensor data to Serial
  if (data.isValid) {
    Serial.println("\n--- Sensor Readings ---");
    Serial.printf("Temperature: %.1f°C\n", data.temperature);
    Serial.printf("Humidity: %.1f%%\n", data.humidity);
    Serial.printf("Soil Moisture: %d%%\n", data.soilMoisture);
    Serial.printf("Light Level: %d\n", data.lightLevel);
    Serial.println("----------------------");
  }
  
  return data;
}

#endif // SENSORS_H
