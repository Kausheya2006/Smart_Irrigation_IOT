#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

#define DHTPIN 4        
#define DHTTYPE DHT11    
#define SOIL_PIN 34      
#define RELAY_PIN 15     

// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// ThingSpeak Settings
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "YOUR_API_KEY"; 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    dht.begin();

    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, LOW);

    // Connect to WiFi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected!");
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        
        float humidity = dht.readHumidity();
        int soil_moisture = analogRead(SOIL_PIN);
        int moisture_percent = map(soil_moisture, 4095, 1500, 0, 100);

        // Switching the Pump
        bool pump = (moisture_percent < 30 && humidity < 40);
        digitalWrite(RELAY_PIN, pump ? HIGH : LOW);

        // Create the ThingSpeak URL
        String url = server;
        url += "?api_key=";
        url += apiKey;
        url += "&field1=" + String(humidity);
        url += "&field2=" + String(moisture_percent);
        url += "&field3=" + String(pump);

        // Sending data to ThingSpeak
        Serial.println("Sending Data to ThingSpeak...");
        Serial.println(url);

        http.begin(url);
        int httpResponseCode = http.GET();
        
        if (httpResponseCode > 0) {
            Serial.println("Data Sent Successfully!");
        } else {
            Serial.println("Error Sending Data.");
        }
        http.end();
    }

    delay(15000);  // Send data every 15 seconds
}
