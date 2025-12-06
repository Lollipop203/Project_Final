Hệ thống tưới cây và chiếu sáng thông minh sử dụng ESP32, cảm biến, và điều khiển từ xa qua web app.
- **Tưới cây tự động**: Bật máy bơm khi độ ẩm đất < ngưỡng
- **Chiếu sáng tự động**: Bật đèn LED khi ánh sáng < ngưỡng
- **Chế độ thủ công**: Điều khiển trực tiếp từ web app

###  Giám sát
- Nhiệt độ và độ ẩm không khí (DHT11/22)
- Độ ẩm đất
- Cường độ ánh sáng
- Biểu đồ theo dõi real-time
- Hiển thị LCD cục bộ

###  Điều khiển từ xa
- Web dashboard responsive
- Điều khiển máy bơm và đèn LED
- Cài đặt ngưỡng cảm biến
- PWA - Cài đặt như app trên điện thoại

##  Linh kiện

| Linh kiện | Số lượng | Mô tả |
|-----------|----------|-------|
| ESP32 DevKit | 1 | Vi điều khiển chính |
| DHT11/DHT22 | 1 | Cảm biến nhiệt độ & độ ẩm |
| Soil Moisture Sensor | 1 | Cảm biến độ ẩm đất |
| Light Sensor (LDR) | 1 | Cảm biến ánh sáng |
| Relay Module 2 kênh | 1 | Điều khiển máy bơm & đèn |
| LCD 16x2 I2C | 1 | Màn hình hiển thị |
| Máy bơm nước mini | 1 | Tưới cây |
| LED | 1 | Chiếu sáng |
| Điện trở 220Ω | 1 | Cho LED |
| Dây nối, breadboard | - | Kết nối mạch |

##  Cấu trúc dự án

project_final/
├── esp32_firmware/          # Code ESP32
│   ├── esp32_main.ino      # File chính
│   ├── config.h            # Cấu hình WiFi, Firebase, pins
│   ├── sensors.h           # Thư viện cảm biến
│   └── actuators.h         # Thư viện điều khiển
├── wokwi/                   # Mô phỏng Wokwi
│   ├── diagram.json        # Sơ đồ mạch
│   ├── wokwi.toml          # Cấu hình
│   └── libraries.txt       # Thư viện
├── web_dashboard/           # Web app
│   ├── index.html          # Giao diện
│   ├── style.css           # Thiết kế
│   ├── app.js              # Logic chính
│   ├── firebase-config.js  # Cấu hình Firebase
│   ├── manifest.json       # PWA manifest
│   └── service-worker.js   # Service worker
└── README.md               # File này


