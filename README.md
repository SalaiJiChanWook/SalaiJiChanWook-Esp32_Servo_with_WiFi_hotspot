# ESP32 Servo Controller via Wi-Fi Access Point (AP Mode)

[![Platform: ESP32](https://img.shields.io/badge/Platform-ESP32-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![Environment: Arduino IDE](https://img.shields.io/badge/Environment-Arduino%2520IDE-orange.svg)](https://www.arduino.cc/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

An intuitive IoT project that transforms an **ESP32 microcontroller** into a standalone Wi-Fi Access Point (Hotspot). It hosts a lightweight, responsive web server allowing users to seamlessly control servo motor angles wirelessly via a smartphone, tablet, or PC browser—**no external router or internet connection required!**

---

## 🚀 Features

* **Standalone Access Point (AP Mode):** The ESP32 broadcasts its own local Wi-Fi network, ensuring portability and plug-and-play operation anywhere.
* **Embedded Web Dashboard:** Features a clean, responsive HTML/JavaScript control interface embedded directly into the ESP32 flash memory or served dynamically.
* **Precise Servo Positioning:** Real-time angle adjustments (0° to 180°) via interactive sliders or buttons.
* **Low Latency:** Direct socket/HTTP communication ensures instantaneous response times from device command to physical motion.

---

## 🛠️ Hardware Requirements

* **ESP32 Development Board** (NodeMCU-32S, ESP32-WROOM-32, etc.)
* **Servo Motor** (e.g., SG90 or MG996R)
* **External Power Supply** (Recommended: 5V source for the servo motor to prevent ESP32 brownouts and voltage drops)
* **Jumper Wires**
* **Micro-USB Cable** (for programming/powering the ESP32)

---

## 📌 Pinout & Wiring Configuration

| ESP32 Pin | Servo Motor Pin | Description |
| :--- | :--- | :--- |
| **GPIO 13** (configurable) | Signal (Orange / Yellow) | PWM control signal |
| **GND** | Ground (Brown / Black) | Common ground |
| **External 5V / 3.3V** | VCC (Red) | Servo power supply *(Note: High-torque servos require an independent 5V supply with shared grounds)* |

---

## ⚙️ Software & Dependencies

Ensure you have the following installed in your **Arduino IDE**:

1. **ESP32 Board Support Package** via Board Manager (`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`).
2. **ESP32 Servo Library** (Standard `ESP32Servo` library compatible with the ledc architecture).

---

## 📥 Getting Started & Installation

1. **Clone the Repository:**
   ```bash
   git clone [https://github.com/SalaiJiChanWook/Esp32_Servo_with_WiFi_hotspot.git](https://github.com/SalaiJiChanWook/Esp32_Servo_with_WiFi_hotspot.git)
