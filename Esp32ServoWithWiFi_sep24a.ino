#include <WiFi.h>
#include <WebServer.h>

const int servoPin = 13;

// PWM Settings
const int pwmFreq = 50;        // Servo အတွက် 50Hz
const int pwmResolution = 14;  // 14-bit resolution (0 မှ 16383 အထိ)
const int minDuty = 410;       // 0 ဒီဂရီအတွက် pulse (~0.5ms)
const int maxDuty = 2048;      // 180 ဒီဂရီအတွက် pulse (~2.5ms)

const char *ssid = "ESP32-Servo-Control";
const char *password = "MyPassword?";

WebServer server(80);

const char HTML_CONTENT[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Servo Control</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 50px; background-color: #f2f4f8; }
    .slider-box { background: white; max-width: 350px; margin: auto; padding: 25px; border-radius: 12px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }
    .slider { width: 100%; height: 15px; border-radius: 5px; background: #ddd; outline: none; }
    #angleValue { font-size: 28px; font-weight: bold; color: #007bff; margin: 15px 0; }
  </style>
</head>
<body>
  <div class="slider-box">
    <h2>Servo Controller</h2>
    <p>Degree:</p>
    <div id="angleValue">90°</div>
    <input type="range" min="0" max="180" value="90" class="slider" id="servoSlider" oninput="updateServo(this.value)">
  </div>
  <script>
    function updateServo(val) {
      document.getElementById('angleValue').innerText = val + '°';
      fetch('/setServo?value=' + val);
    }
  </script>
</body>
</html>
)rawliteral";

// From Degree (0-180) to PWM Duty Cycle calculating Function
void setServoAngle(int angle) {
  angle = constrain(angle, 0, 180);
  int duty = map(angle, 0, 180, minDuty, maxDuty);
  ledcWrite(servoPin, duty); // IF it is use ESP32 core v3.x, direct write with pin number
}

void handleRoot() {
  server.send(200, "text/html", HTML_CONTENT);
}

void handleSetServo() {
  if (server.hasArg("value")) {
    int angle = server.arg("value").toInt();
    setServoAngle(angle);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing Value");
  }
}

void setup() {
  Serial.begin(115200);

  // PWM Pin connection
  ledcAttach(servoPin, pwmFreq, pwmResolution);
  setServoAngle(90); // Start at 90 degree

  // Wi-Fi Access Point ဖွင့်ခြင်း
  WiFi.softAP(ssid, password);
  Serial.println("\n--- Wi-Fi Hotspot စတင်ပါပြီ ---");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/setServo", handleSetServo);
  server.begin();
}

void loop() {
  server.handleClient();
}