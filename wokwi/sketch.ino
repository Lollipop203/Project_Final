// WOKWI VERSION (No Firebase) * -

#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>


// DHT Sensor
#define DHT_PIN 4
#define DHT_TYPE DHT22

// Cảm biến độ ẩm đất( joystick trục y)
#define SOIL_MOISTURE_PIN 34

// Light Sensor (LDR)
#define LIGHT_SENSOR_PIN 35

// Relay Controls
#define PUMP_RELAY_PIN 26
#define LED_RELAY_PIN 27

// Điều kiện ánh sáng, độ ẩm
#define SOIL_MOISTURE_THRESHOLD 30
#define LIGHT_THRESHOLD 200
#define PUMP_DURATION 7000

// Thời gian đọc sensor
#define SENSOR_READ_INTERVAL 2000


DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
// Global Variables
bool autoMode = true;
bool pumpState = false;
bool ledState = false;
unsigned long lastSensorRead = 0;
unsigned long pumpStartTime = 0;
bool pumpTimerActive = false;


float temperature = 0;
float humidity = 0;
int soilMoisture = 0;
int lightLevel = 0;


// Control Pump
void controlPump(bool state) {
  pumpState = state;
  digitalWrite(PUMP_RELAY_PIN, state ? HIGH : LOW);
  Serial.printf(" Pump: %s\n", state ? "ON" : "OFF");
}

// Control LED
void controlLED(bool state) {
  ledState = state;
  digitalWrite(LED_RELAY_PIN, state ? HIGH : LOW);
  Serial.printf("LED: %s\n", state ? "ON" : "OFF");
}


// Read Sensors

void readSensors() {
  // Read DHT
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  // Read Soil Moisture (convert ADC to percentage)
  int rawSoil = analogRead(SOIL_MOISTURE_PIN);
  soilMoisture = map(rawSoil, 0, 4095, 0, 100);
  soilMoisture = constrain(soilMoisture, 0, 100);
  
  // Read Light Sensor
  int rawLight = analogRead(LIGHT_SENSOR_PIN);
  lightLevel = map(rawLight, 4095, 0, 0, 1023);
  
  // Print to Serial
  Serial.println("\n--- Sensor Readings ---");
  Serial.printf("Temperature: %.1f°C\n", temperature);
  Serial.printf("Humidity: %.1f%%\n", humidity);
  Serial.printf("Soil Moisture: %d%%\n", soilMoisture);
  Serial.printf("Light Level: %d\n", lightLevel);
  Serial.println("----------------------");
}


// Update LCD

void updateLCD() {
  static unsigned long lastUpdate = 0;
  static int displayMode = 0;
  
  if (millis() - lastUpdate > 3000) {
    lastUpdate = millis();
    lcd.clear();
    
    switch (displayMode) {
      case 0:
        lcd.setCursor(0, 0);
        lcd.printf("Temp: %.1fC", temperature);
        lcd.setCursor(0, 1);
        lcd.printf("Hum: %.1f%%", humidity);
        break;
        
      case 1:
        lcd.setCursor(0, 0);
        lcd.printf("Soil: %d%%", soilMoisture);
        lcd.setCursor(0, 1);
        lcd.printf("Light: %d", lightLevel);
        break;
        
      case 2:
        lcd.setCursor(0, 0);
        lcd.printf("Pump: %s", pumpState ? "ON " : "OFF");
        lcd.setCursor(0, 1);
        lcd.printf("LED: %s", ledState ? "ON " : "OFF");
        break;
    }
    
    displayMode = (displayMode + 1) % 3;
  }
}

// Auto Control Logic

void autoControl() {
  if (!autoMode) {
    return;
  }
  
  // Auto irrigation
  if (soilMoisture < SOIL_MOISTURE_THRESHOLD && !pumpState) {
    Serial.println("Soil too dry! Starting irrigation...");
    controlPump(true);
    pumpStartTime = millis();
    pumpTimerActive = true;
  }
  
  // Stop pump after duration
  if (pumpTimerActive && (millis() - pumpStartTime > PUMP_DURATION)) {
    Serial.println("Irrigation complete");
    controlPump(false);
    pumpTimerActive = false;
  }
  
  // Auto lighting
  if (lightLevel < LIGHT_THRESHOLD) {
    if (!ledState) {
      Serial.println("Low light! Turning on LED...");
      controlLED(true);
    }
  } else {
    if (ledState) {
      Serial.println("Sufficient light! Turning off LED...");
      controlLED(false);
    }
  }
}


// Setup
void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n========================================");
  Serial.println("  Smart Irrigation System - WOKWI");
  Serial.println("========================================\n");
  
  // Initialize 
  dht.begin();
  Serial.println("DHT sensor initialized");
  
  
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Smart Irrigation");
  lcd.setCursor(0, 1);
  lcd.print("Initializing...");
  Serial.println("LCD initialized");
  
  
  pinMode(PUMP_RELAY_PIN, OUTPUT);
  pinMode(LED_RELAY_PIN, OUTPUT);
  digitalWrite(PUMP_RELAY_PIN, LOW);
  digitalWrite(LED_RELAY_PIN, LOW);
  Serial.println("Relays initialized");
  pinMode(SOIL_MOISTURE_PIN, INPUT);
  pinMode(LIGHT_SENSOR_PIN, INPUT);
  Serial.println("Sensors initialized");
  
  delay(2000);
  Serial.println("System ready!\n");
}


// Main Loop

void loop() {
  unsigned long currentTime = millis();
  
  // Read sensors periodically
  if (currentTime - lastSensorRead >= SENSOR_READ_INTERVAL) {
    lastSensorRead = currentTime;
    
    readSensors();
    updateLCD();
    autoControl();
  }
  
  delay(10);
}
