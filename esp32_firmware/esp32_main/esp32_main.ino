/*
 * Smart Irrigation & Lighting IoT System
 * ESP32 Main Firmware
 * 
 * Features:
 * - Auto irrigation based on soil moisture
 * - Auto lighting based on light level
 * - Real-time data to Firebase
 * - Manual control via web dashboard
 * - LCD display for local monitoring
 */

#include <WiFi.h>
#include <FirebaseESP32.h>
#include "config.h"
#include "sensors.h"
#include "actuators.h"

// ============================================
// Firebase Objects
// ============================================
FirebaseData firebaseData;
FirebaseAuth auth;
FirebaseConfig config;

// ============================================
// Global Variables
// ============================================
bool autoMode = true;
unsigned long lastSensorRead = 0;
unsigned long lastFirebaseUpdate = 0;
unsigned long pumpStartTime = 0;
bool pumpTimerActive = false;

// ============================================
// WiFi Connection
// ============================================
void connectWiFi() {
  Serial.println("\n🌐 Connecting to WiFi...");
  displayWiFiStatus(false);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    displayWiFiStatus(true);
  } else {
    Serial.println("\n✗ WiFi Connection Failed!");
    displayError("WiFi Failed");
  }
}

// ============================================
// Firebase Setup
// ============================================
void setupFirebase() {
  Serial.println("\n🔥 Setting up Firebase...");
  
  // Configure Firebase
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  
  // Initialize Firebase
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  
  // Set timeout
  firebaseData.setBSSLBufferSize(1024, 1024);
  firebaseData.setResponseSize(1024);
  
  Serial.println("✓ Firebase initialized");
  displayFirebaseStatus(true);
  
  delay(2000);
}

// ============================================
// Send Data to Firebase
// ============================================
void sendToFirebase(SensorData data) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("✗ WiFi not connected, skipping Firebase update");
    return;
  }
  
  String basePath = "/devices/" + String(DEVICE_ID);
  
  // Send sensor data
  Firebase.setFloat(firebaseData, basePath + "/sensors/temperature", data.temperature);
  Firebase.setFloat(firebaseData, basePath + "/sensors/humidity", data.humidity);
  Firebase.setInt(firebaseData, basePath + "/sensors/soilMoisture", data.soilMoisture);
  Firebase.setInt(firebaseData, basePath + "/sensors/lightLevel", data.lightLevel);
  Firebase.setInt(firebaseData, basePath + "/sensors/timestamp", millis());
  
  // Send actuator states
  Firebase.setBool(firebaseData, basePath + "/actuators/pump", getPumpState());
  Firebase.setBool(firebaseData, basePath + "/actuators/led", getLEDState());
  
  // Send settings
  Firebase.setBool(firebaseData, basePath + "/settings/autoMode", autoMode);
  
  Serial.println("✓ Data sent to Firebase");
}

// ============================================
// Read Commands from Firebase
// ============================================
void readFirebaseCommands() {
  if (WiFi.status() != WL_CONNECTED) {
    return;
  }
  
  String basePath = "/devices/" + String(DEVICE_ID);
  
  // Read auto mode setting
  if (Firebase.getBool(firebaseData, basePath + "/settings/autoMode")) {
    autoMode = firebaseData.boolData();
  }
  
  // Read manual control commands (only if not in auto mode)
  if (!autoMode) {
    if (Firebase.getBool(firebaseData, basePath + "/actuators/pump")) {
      bool pumpCommand = firebaseData.boolData();
      controlPump(pumpCommand);
    }
    
    if (Firebase.getBool(firebaseData, basePath + "/actuators/led")) {
      bool ledCommand = firebaseData.boolData();
      controlLED(ledCommand);
    }
  }
}

// ============================================
// Auto Control Logic
// ============================================
void autoControl(SensorData data) {
  if (!autoMode) {
    return;
  }
  
  // Auto irrigation logic
  if (data.soilMoisture < SOIL_MOISTURE_THRESHOLD && !getPumpState()) {
    Serial.println("🌱 Soil too dry! Starting irrigation...");
    controlPump(true);
    pumpStartTime = millis();
    pumpTimerActive = true;
  }
  
  // Stop pump after duration
  if (pumpTimerActive && (millis() - pumpStartTime > PUMP_DURATION)) {
    Serial.println("🌱 Irrigation complete");
    controlPump(false);
    pumpTimerActive = false;
  }
  
  // Auto lighting logic
  if (data.lightLevel < LIGHT_THRESHOLD) {
    if (!getLEDState()) {
      Serial.println("💡 Low light detected! Turning on LED...");
      controlLED(true);
    }
  } else {
    if (getLEDState()) {
      Serial.println("💡 Sufficient light! Turning off LED...");
      controlLED(false);
    }
  }
}

// ============================================
// Setup
// ============================================
void setup() {
  // Initialize Serial
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n");
  Serial.println("========================================");
  Serial.println("  Smart Irrigation & Lighting System");
  Serial.println("========================================\n");
  
  // Initialize hardware
  initSensors();
  initActuators();
  
  // Connect to WiFi
  connectWiFi();
  
  // Setup Firebase
  if (WiFi.status() == WL_CONNECTED) {
    setupFirebase();
  }
  
  Serial.println("\n✓ System ready!\n");
}

// ============================================
// Main Loop
// ============================================
void loop() {
  unsigned long currentTime = millis();
  
  // Read sensors periodically
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    lastSensorRead = currentTime;
    
    SensorData data = readAllSensors();
    
    if (data.isValid) {
      // Update LCD
      updateLCD(data.temperature, data.humidity, data.soilMoisture, data.lightLevel);
      
      // Auto control
      autoControl(data);
      
      // Send to Firebase
      if (currentTime - lastFirebaseUpdate >= FIREBASE_UPDATE_INTERVAL) {
        lastFirebaseUpdate = currentTime;
        sendToFirebase(data);
        readFirebaseCommands();
      }
    }
  }
  
  // Small delay to prevent watchdog timer issues
  delay(10);
}
