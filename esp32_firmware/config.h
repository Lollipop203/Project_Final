#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// WiFi Configuration
// ============================================
#define WIFI_SSID "Cafe Tron 5G"          // Thay bằng tên WiFi của bạn
#define WIFI_PASSWORD "39393939"  // Thay bằng mật khẩu WiFi

// ============================================
// Firebase Configuration
// ============================================
#define FIREBASE_HOST "smart-irrigation-system-1cf35-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "z96GHkoEVPsFhLuHXYtrmaZazHFQn4pFBLL0fPDl"  // Lấy từ: Firebase Console → Project Settings → Service accounts → Database secrets
#define DEVICE_ID "device_001"                // ID thiết bị

// ============================================
// GPIO Pin Configuration
// ============================================
// DHT Sensor
#define DHT_PIN 4
#define DHT_TYPE DHT11  // Hoặc DHT22

// Soil Moisture Sensor
#define SOIL_MOISTURE_PIN 34  // ADC pin

// Light Sensor (LDR)
#define LIGHT_SENSOR_PIN 35   // ADC pin

// Relay Controls
#define PUMP_RELAY_PIN 26     // Relay điều khiển máy bơm
#define LED_RELAY_PIN 27      // Relay điều khiển đèn LED

// LCD I2C
#define LCD_ADDRESS 0x27      // Địa chỉ I2C của LCD (thường là 0x27 hoặc 0x3F)
#define LCD_COLS 16           // Số cột LCD
#define LCD_ROWS 2            // Số hàng LCD

// ============================================
// Sensor Thresholds
// ============================================
#define SOIL_MOISTURE_THRESHOLD 30    // Độ ẩm đất < 30% → Bật máy bơm
#define LIGHT_THRESHOLD 200           // Ánh sáng < 200 → Bật đèn LED
#define PUMP_DURATION 5000            // Thời gian tưới mỗi lần (ms)

// ============================================
// Timing Configuration
// ============================================
#define SENSOR_READ_INTERVAL 2000     // Đọc cảm biến mỗi 2 giây
#define FIREBASE_UPDATE_INTERVAL 5000 // Gửi dữ liệu lên Firebase mỗi 5 giây
#define LCD_UPDATE_INTERVAL 1000      // Cập nhật LCD mỗi 1 giây

// ============================================
// ADC Calibration (for ESP32)
// ============================================
#define ADC_MAX_VALUE 4095            // ESP32 ADC 12-bit
#define ADC_VOLTAGE 3.3               // Điện áp tham chiếu

#endif // CONFIG_H
