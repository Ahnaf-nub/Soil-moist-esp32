# Smart Irrigation System with ESP32
This project is a smart irrigation system powered by the ESP32 microcontroller, designed to monitor soil moisture levels and water tank levels, automatically control a relay to manage watering and also servo for scattering the water, and send notifications when action is needed. The system is ideal for small gardens, potted plants, and automated greenhouse setups.

# Features
**Soil Moisture Monitoring: Continuously measures soil moisture using a capacitive soil moisture sensor and displays the percentage on an OLED screen**
**Water Level Monitoring: Uses an ultrasonic sensor to measure the water level in a tank and displays the level as a percentage.**
**Automated Watering: Controls a relay to activate watering when soil moisture falls below a user-defined threshold (e.g., 40%) and also use servo motor to scatter the water.**
**Low Water Level Notification: Sends an email notification when the water level drops below 20%, alerting the user to refill the tank.**

##Getting Started
### Hardware Requirements:
**ESP32 microcontroller**
**Capacitive soil moisture sensor**
**Ultrasonic sensor (e.g., HC-SR04)**
**Servo Motor**
**Relay module**
**OLED display (128x64)**
### Software Setup:
**Arduino IDE with ESP32 board support**
**IFTT Account**
**Required libraries: Adafruit_SSD1306, Adafruit_GFX, NewPing, WiFi, HTTPClient, ESP32Servo**
