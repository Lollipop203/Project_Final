/*
 * ESP32 Simple Test
 * Test basic functionality without Firebase
 */

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n========================================");
  Serial.println("  ESP32 Simple Test");
  Serial.println("========================================\n");
  
  Serial.println("✓ Serial working!");
  Serial.println("✓ ESP32 is running!");
  
  pinMode(2, OUTPUT); // Built-in LED
}

void loop() {
  digitalWrite(2, HIGH);
  Serial.println("LED ON");
  delay(1000);
  
  digitalWrite(2, LOW);
  Serial.println("LED OFF");
  delay(1000);
}
