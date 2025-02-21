# Smart Irrigation System using ESP32 & ThingSpeak

## 📌 Project Overview
This project is an IoT-based **Smart Irrigation System** using an **ESP32**, a **DHT11/DHT22 humidity sensor**, and a **capacitive soil moisture sensor**. The system monitors soil moisture and humidity and **automatically turns ON a water pump** when the values fall below a set threshold. The sensor data is also published to **ThingSpeak** for remote monitoring.

---

## 🛠️ Components Used
| Component                  | Quantity | Description |
|----------------------------|----------|-------------|
| **ESP32**                  | 1        | Microcontroller for IoT |
| **DHT22 Sensor**     | 1        | Measures humidity (%) |
| **Capacitive Soil Moisture Sensor** | 1 | Detects soil moisture levels |
| **Relay Module (5V)**      | 1        | Controls the water pump |
| **Water Pump (Miniature)** | 1        | Activated when soil is dry |
| **Jumper Wires**           | As needed | For connections |
| **10kΩ Resistor**          | 1        | Required for DHT sensor pull-up |

---

## ⚡ Circuit Connections
| ESP32 Pin  | Component |
|------------|-----------|
| **GPIO 4** | DHT11/DHT22 Data Pin |
| **GPIO 34** | Soil Moisture Sensor (Analog Out) |
| **GPIO 15** | Relay Module (IN) |
| **3.3V / 5V** | VCC of DHT & Soil Sensor |
| **GND** | GND of all components |

---

## 🚀 Features
✅ **Monitors Soil Moisture & Humidity**  
✅ **Automatically Controls a Water Pump**  
✅ **Uploads Sensor Data to ThingSpeak**  
✅ **Threshold-Based Pump Activation**  

---

## 📌 How to Set Up
### **1️⃣ Install Required Libraries**
Make sure you have installed the following **Arduino libraries**:
- **DHT sensor library** (`DHT.h`)
- **WiFi library** (`WiFi.h`)
- **HTTP Client** (`HTTPClient.h`)

### **2️⃣ Configure ThingSpeak API**
- Create an account on [ThingSpeak](https://thingspeak.com/).
- Create a new channel and note down your **Channel ID** and **Write API Key**.

### **3️⃣ Upload Code to ESP32**
Replace **YOUR_WIFI_SSID**, **YOUR_WIFI_PASSWORD**, and **YOUR_THINGSPEAK_API_KEY** in the Arduino code before uploading.

---

## 📜 Code Example
```cpp
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define SOIL_PIN 34
#define RELAY_PIN 15

#define SOIL_THRESHOLD 30
#define HUMIDITY_THRESHOLD 40

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "YOUR_THINGSPEAK_API_KEY";

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid, password);
    dht.begin();
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected!");
}

void loop() {
    float humidity = dht.readHumidity();
    int soil_moisture = analogRead(SOIL_PIN);
    int moisture_percent = map(soil_moisture, 4095, 1500, 0, 100);

    Serial.print("Humidity: "); Serial.print(humidity); Serial.println("%");
    Serial.print("Soil Moisture: "); Serial.print(moisture_percent); Serial.println("%");
    
    if (moisture_percent < SOIL_THRESHOLD && humidity < HUMIDITY_THRESHOLD) {
        digitalWrite(RELAY_PIN, HIGH);
        Serial.println("Pump Activated!");
    } else {
        digitalWrite(RELAY_PIN, LOW);
        Serial.println("Pump Deactivated!");
    }

    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String url = String(server) + "?api_key=" + apiKey + "&field1=" + moisture_percent + "&field2=" + humidity;
        http.begin(url);
        int httpCode = http.GET();
        http.end();
    }
    delay(15000);
}
```
---

## 📡 How to Monitor Data on ThingSpeak

1. Go to your ThingSpeak channel.
2. Click on Private View → Your Data Fields.
3. Sensor values will update every 15 seconds.

## 📢 Author & License

📍 **Created by** Team: `Hawk Two`

🔗 *Feel free to modify and improve this project!* 🚀
