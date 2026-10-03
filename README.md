# ESP32 Smart Box & Servo Controller (v2.0)

A dual-mode IoT project featuring an automatic contactless opening lid using an ultrasonic sensor (HC-SR04) and a built-in ESP32 Web Server interface for real-time distance telemetry and manual servo angle override.

![Project Status](https://img.shields.io/badge/status-active-brightgreen)
![Version](https://img.shields.io/badge/version-2.0-blue)
![Platform](https://img.shields.io/badge/platform-ESP32-red)
![Framework](https://img.shields.io/badge/framework-Arduino-teal)

---

## What's New in Version 2.0

- **Dual-Mode Operation:** 
  - **Auto Sensor Mode:** Detects objects/hands within 15 cm and automatically opens the lid for 3 seconds before closing.
  - **Manual Web Mode:** Provides a clean mobile-friendly web interface with a real-time distance gauge and a manual trigger button.
- **Pure LEDC PWM (No External Library):** Eliminates dependencies on third-party servo libraries; runs natively on ESP32 Core PWM hardware timers.
- **Standalone Access Point (SoftAP):** Operates independently without requiring a home Wi-Fi router.

---

## Hardware Requirements

| Component | Quantity | Purpose |
|---|---|---|
| ESP32 Dev Board (30-pin / 38-pin) | 1 | Main micro-controller & Wi-Fi server |
| Micro Servo (SG90 / MG90S) | 1 | Lid actuation mechanism |
| Ultrasonic Sensor (HC-SR04 / HC-SR04P) | 1 | Distance detection |
| Breadboard & Jumper Wires | 1 Set | Interfacing connections |
| 5V Power Supply / USB Cable | 1 | System power |

---

## Wiring Diagram

Both the Servo Motor and the Ultrasonic Sensor share the **VIN (5V)** and **GND** rails via a breadboard.
<img width="385" height="278" alt="image" src="https://github.com/user-attachments/assets/98b11919-45c2-405d-8aed-e25733802f83" />

[ Servo SG90 ]                         |       |
- Signal (Yellow/Orange) ------------+       |
|
[ HC-SR04 Sensor ]                             |
- Trig --------------------------------------+
- Echo ----------------------------------------------+

### Pin Assignment Table

| Peripheral | Peripheral Pin | ESP32 Pin | Logic Level |
|---|---|---|---|
| **Servo Motor** | VCC (Red) | **VIN** | 5V |
| | GND (Brown/Black) | **GND** | 0V |
| | Signal (Yellow/Orange) | **GPIO 13 (D13)** | 3.3V PWM |
| **HC-SR04 Sensor** | VCC | **VIN** | 5V |
| | GND | **GND** | 0V |
| | Trig | **GPIO 4 (D4)** | 3.3V Output |
| | Echo | **GPIO 19 (D19)** | Input |

## Getting Started

### 1. Prerequisites
- [Arduino IDE](https://www.arduino.cc/en/software) (version 2.0+ recommended)
- ESP32 Board package installed via Board Manager (`esp32` by Espressif Systems)

### 2. Flashing the Code
1. Open the project sketch (`.ino` file) in Arduino IDE.
2. Under **Tools > Board**, select your target (e.g., `ESP32 Dev Module`).
3. Connect your ESP32 to the PC via USB and select the corresponding **COM Port**.
4. Click **Upload**.

---

## Usage Instructions

1. **Power On:** Supply power to the ESP32 via USB (5V 2A adapter recommended).
2. **Automatic Operation:** Place your hand within **15 cm** of the sensor. The servo will swing to 90° (open position), hold for 3 seconds, and return to 0° (closed position).
3. **Web Interface Access:**
   - On your phone or laptop, connect to the Wi-Fi network:
     - **SSID:** `ESP32-Smart-Box`
     - **Password:** `12345678`
   - Open any browser and navigate to:
     ```
     [http://192.168.4.1](http://192.168.4.1)
     ```
   - Monitor live distance readings and trigger the manual open action directly from the web dashboard.

---

## Configuration Variables

You can customize operational parameters directly in the code:

```cpp
const int TRIGGER_DISTANCE = 15;     // Detection threshold in centimeters
const unsigned long OPEN_DURATION = 3000; // Lid hold time in milliseconds
const int CLOSE_ANGLE = 0;          // Closed position (degrees)
const int OPEN_ANGLE = 90;          // Open position (degrees)
