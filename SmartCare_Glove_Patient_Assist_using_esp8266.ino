#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Button pin definitions
const int btn1 = D5; // GPIO14 - Need Water
const int btn2 = D6; // GPIO12 - Need Food
const int btn3 = D7; // GPIO13 - Emergency
const int btn4 = D3; // GPIO0  - Call Family (REPLACED D0)

String currentMessage = "No Request";
unsigned long lastPressTime = 0;
const int displayDuration = 3000; // Display message for 3 seconds

// Debounce variables
const unsigned long debounceDelay = 200;
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
unsigned long lastDebounceTime3 = 0;
unsigned long lastDebounceTime4 = 0;

// WiFi credentials
const char* ssid = "your_ssid";
const char* password = "password";

ESP8266WebServer server(80);

// HTML UI
String htmlPage() {
  return R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Smart Glove - Patient Monitor</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <style>
        body {
          background: linear-gradient(to right, #83a4d4, #b6fbff);
          font-family: Arial, sans-serif;
          text-align: center;
          padding-top: 50px;
          color: #333;
        }
        h1 { font-size: 32px; margin-bottom: 20px; }
        #msg {
          font-size: 28px;
          padding: 20px;
          background: white;
          border-radius: 10px;
          box-shadow: 0 0 10px #666;
          animation: pulse 1s infinite alternate;
        }
        @keyframes pulse {
          from { transform: scale(1); }
          to { transform: scale(1.05); background-color: #e0f7fa; }
        }
      </style>
      <script>
        setInterval(function(){
          fetch('/msg').then(res => res.text()).then(data => {
            document.getElementById("msg").innerHTML = data;
          });
        }, 1000);
      </script>
    </head>
    <body>
      <h1>Patient Request</h1>
      <div id="msg">Loading...</div>
    </body>
    </html>
  )rawliteral";
}

void setup() {
  Serial.begin(115200);

  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);
  pinMode(btn3, INPUT_PULLUP);
  pinMode(btn4, INPUT_PULLUP); // Using GPIO0 instead of GPIO16

  // Initialize display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Connecting WiFi...");
  display.display();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("WiFi Connected!");
  display.println(WiFi.localIP());
  display.display();
  delay(1500);

  // Web server setup
  server.on("/", []() {
    server.send(200, "text/html", htmlPage());
  });
  server.on("/msg", []() {
    server.send(200, "text/plain", currentMessage);
  });
  server.begin();
}

void loop() {
  server.handleClient();

  // Button 1: Need Water
  if (digitalRead(btn1) == LOW && millis() - lastDebounceTime1 > debounceDelay) {
    lastDebounceTime1 = millis();
    showMessage("Need Water");
  }

  // Button 2: Need Food
  if (digitalRead(btn2) == LOW && millis() - lastDebounceTime2 > debounceDelay) {
    lastDebounceTime2 = millis();
    showMessage("Need Food");
  }

  // Button 3: Emergency
  if (digitalRead(btn3) == LOW && millis() - lastDebounceTime3 > debounceDelay) {
    lastDebounceTime3 = millis();
    showMessage("Emergency! Help!");
  }

  // Button 4: Call Family (D3)
  if (digitalRead(btn4) == LOW && millis() - lastDebounceTime4 > debounceDelay) {
    lastDebounceTime4 = millis();
    showMessage("Call Family");
  }

  // Clear message after 3 seconds
  if (millis() - lastPressTime > displayDuration && currentMessage != "No Request") {
    currentMessage = "No Request";
    displayMessage(currentMessage);
  }
}

// Update message and display it
void showMessage(String msg) {
  lastPressTime = millis();
  currentMessage = msg;
  Serial.println("Message: " + msg);
  displayMessage(msg);
}

// Display message on OLED
void displayMessage(String msg) {
  display.clearDisplay();
  display.setCursor(0, 10);
  display.setTextSize(1);
  display.println("Patient Message:");
  display.setTextSize(2);
  display.setCursor(0, 30);
  display.println(msg);
  display.display();
}
