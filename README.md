# SmartCare_Glove_Patient_Assist_using_esp8266


SmartGlove-PatientAssist is an ESP8266-based wireless monitoring system for bedridden or elderly patients. Using physical buttons and an OLED screen, it allows patients to send immediate requests like "Need Water", "Need Food", or "Emergency" to caregivers through a local web page.

## 🚀 Features
- Four physical buttons for different patient requests
- Real-time message display on OLED
- Web-based interface showing live patient status
- Responsive UI with animations
- Wi-Fi-based local server using ESP8266

## 🔧 Hardware Used
- NodeMCU ESP8266
- 0.96" OLED Display (SSD1306)
- 4 Push Buttons
- Breadboard and jumper wires

## 🔌 Pin Configuration
| Button          | GPIO Pin | NodeMCU Pin |
|-----------------|----------|-------------|
| Need Water      | GPIO14   | D5          |
| Need Food       | GPIO12   | D6          |
| Emergency       | GPIO13   | D7          |
| Call Family     | GPIO0    | D3          |

## 📦 Required Libraries
- ESP8266WiFi
- ESP8266WebServer
- Adafruit GFX
- Adafruit SSD1306

Install these libraries via **Library Manager** in Arduino IDE.

## 🌐 How It Works
1. Connect NodeMCU to Wi-Fi.
2. OLED displays IP address.
3. Pressing a button updates message on OLED and sends it to the web page.
4. Caregiver views live messages on the browser.

## 📡 Access Webpage
- Connect to the same Wi-Fi network.
- Enter the IP shown on OLED in your browser.


## 💡 Future Enhancements
- Add audio output for alerts
- Integrate with IoT cloud (e.g., Blynk, MQTT)
- Add patient ID and logging features

---
