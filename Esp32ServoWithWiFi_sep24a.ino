#include <WiFi.h>
#include <WebServer.h>

// Pin များ သတ်မှတ်ခြင်း
const int servoPin = 13;
const int trigPin = 5;
const int echoPin = 18;

// Servo PWM ဆက်တင်များ (Library မလိုဘဲ ထိန်းချုပ်ခြင်း)
const int pwmFreq = 50;
const int pwmResolution = 14;
const int minDuty = 410;   // 0 ဒီဂရီ (ဘူးပိတ်သည့်အနေအထား)
const int maxDuty = 2048;  // 180 ဒီဂရီ (ဘူးပွင့်သည့်အနေအထား)

// ဘူးဖွင့်/ပိတ် ဒီဂရီများ (သင့်ဘူးအဖုံး အနေအထားအရ လိုအပ်သလို ပြောင်းလဲနိုင်သည်)
const int CLOSE_ANGLE = 0;   // ပိတ်ထားချိန်
const int OPEN_ANGLE = 90;   // ဖွင့်ထားချိန် (သို့မဟုတ် 180)

// အာရုံခံ အကွာအဝေး သတ်မှတ်ချက် (15 cm အောက်ရောက်လျှင် ပွင့်မည်)
const int TRIGGER_DISTANCE = 15; 
const unsigned long OPEN_DURATION = 3000; // ဘူးပွင့်နေမည့်ကြာချိန် (၃ စက္ကန့်)

bool isOpen = false;
unsigned long openTimer = 0;

// Wi-Fi Access Point အချက်အလက်များ
const char *ssid = "ESP32-Smart-Box";
const char *password = "p@$$Words26";

WebServer server(80);

// Servo လှည့်သည့် Function
void setServoAngle(int angle) {
  angle = constrain(angle, 0, 180);
  int duty = map(angle, 0, 180, minDuty, maxDuty);
  ledcWrite(servoPin, duty);
}

// Ultrasonic အကွာအဝေးတိုင်းသည့် Function (cm)
float readDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (duration == 0) return 999.0; // အရာဝတ္ထုမရှိလျှင်
  return (duration * 0.0343) / 2.0;
}

// Web Page HTML
const char HTML_CONTENT[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Smart Auto Box</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin-top: 40px; background-color: #f0f2f5; }
    .box { background: white; max-width: 340px; margin: auto; padding: 25px; border-radius: 12px; box-shadow: 0 4px 10px rgba(0,0,0,0.1); }
    h2 { color: #2c3e50; }
    .status { font-size: 26px; font-weight: bold; margin: 15px 0; }
    .open { color: #27ae60; }
    .close { color: #e74c3c; }
    .dist { font-size: 18px; color: #555; margin-bottom: 20px; }
    button { background: #3498db; color: white; border: none; padding: 12px 24px; font-size: 16px; border-radius: 8px; cursor: pointer; }
  </style>
</head>
<body>
  <div class="box">
    <h2>Smart Box Controller</h2>
    <div id="statusText" class="status close">CLOSED</div>
    <div class="dist">Distance: <span id="distVal">--</span> cm</div>
    <button onclick="manualOpen()">Manual Open</button>
  </div>
  <script>
    function manualOpen() {
      fetch('/open');
    }
    setInterval(function() {
      fetch('/status').then(r => r.json()).then(data => {
        document.getElementById('distVal').innerText = data.distance;
        let s = document.getElementById('statusText');
        if(data.isOpen) {
          s.innerText = 'OPEN';
          s.className = 'status open';
        } else {
          s.innerText = 'CLOSED';
          s.className = 'status close';
        }
      });
    }, 1000);
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", HTML_CONTENT);
}

void handleOpen() {
  isOpen = true;
  openTimer = millis();
  setServoAngle(OPEN_ANGLE);
  server.send(200, "text/plain", "Opened");
}

void handleStatus() {
  float d = readDistanceCM();
  String json = "{\"distance\":" + String(d, 1) + ",\"isOpen\":" + (isOpen ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Servo ချိတ်ဆက်ခြင်း
  ledcAttach(servoPin, pwmFreq, pwmResolution);
  setServoAngle(CLOSE_ANGLE); // အစပိုင်းတွင် ဘူးအဖုံးပိတ်ထားမည်

  // Wi-Fi Access Point ဖွင့်ခြင်း
  WiFi.softAP(ssid, password);
  Serial.println("\n--- Smart Box Ready ---");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.on("/status", handleStatus);
  server.begin();
}

void loop() {
  server.handleClient();

  float distance = readDistanceCM();

  // အရာဝတ္ထု/လက် နီးလာပါက (15 cm အောက်) အလိုအလျောက်ဖွင့်မည်
  if (distance > 0 && distance <= TRIGGER_DISTANCE) {
    if (!isOpen) {
      isOpen = true;
      setServoAngle(OPEN_ANGLE);
      Serial.println("Hand detected -> Box Opened");
    }
    openTimer = millis(); // လက်ရှိနေသရွေ့ အချိန် timer ကို reset လုပ်နေမည်
  }

  // ၃ စက္ကန့်ပြည့်ပါက အလိုအလျောက် ပြန်ပိတ်မည်
  if (isOpen && (millis() - openTimer >= OPEN_DURATION)) {
    isOpen = false;
    setServoAngle(CLOSE_ANGLE);
    Serial.println("Timer elapsed -> Box Closed");
  }

  delay(60); // sensor အလုပ်လုပ်ရန် သင့်တင့်သော delay
}