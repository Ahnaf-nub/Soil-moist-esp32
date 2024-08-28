#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NewPing.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// IFTTT Webhook URL
const char* iftttWebhookUrl = "https://maker.ifttt.com/trigger/water_tank_low/with/key/IFTTT_KEY";

#define SOIL_SENSOR_PIN 32
#define AIR_VALUE 560
#define WATER_VALUE 440
#define RELAY_PIN 18
#define TRIG_PIN 4
#define ECHO_PIN 4
#define MAX_DISTANCE 100  // Maximum distance in cm (height of the water tank)
#define servo 18

int soilMoistureRaw = 0;
int soilMoisturePercent = 0;
int waterLevelPercent = 0;
int pos = 0;
bool emailSent = false;

Adafruit_SSD1306 display(128, 64, &Wire, -1);
NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
Servo myservo;

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Start with the relay off
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");
}

void sweep() {
  for (pos = 0; pos <= 180; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(15);           // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                  // tell servo to go to position in variable 'pos'
    delay(15);                           // waits 15ms for the servo to reach the position
  }
}

void loop() {
  // Soil Moisture Calculation
  soilMoistureRaw = analogRead(SOIL_SENSOR_PIN);
  soilMoisturePercent = constrain(map(soilMoistureRaw, AIR_VALUE, WATER_VALUE, 0, 100), 0, 100);

  // Water Level Calculation
  int distanceToWater = sonar.ping_cm();
  waterLevelPercent = constrain(map(distanceToWater, MAX_DISTANCE, 0, 0, 100), 0, 100);

  // Control the relay based on soil moisture level
  if (soilMoisturePercent < 40) {
    sweep();
    digitalWrite(RELAY_PIN, LOW);  // Activate relay
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Deactivate relay
  }

  // Send email if water level is below 20%
  if (waterLevelPercent < 20 && !emailSent) {
    sendEmailNotification();
    emailSent = true;  // Ensure the email is sent only once until water is refilled
  } else if (waterLevelPercent >= 20) {
    emailSent = false;  // Reset the flag if the water level is refilled
  }

  // Display Soil Moisture
  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Soil Moisture");

  display.setCursor(30, 40);
  display.setTextSize(3);
  display.print(soilMoisturePercent);
  display.println(" %");

  // Display Water Level
  display.setCursor(10, 80);
  display.setTextSize(2);
  display.println("Water Level");

  display.setCursor(30, 120);
  display.setTextSize(3);
  display.print(waterLevelPercent);
  display.println(" %");

  display.display();

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println(" %");

  Serial.print("Water Level: ");
  Serial.print(waterLevelPercent);
  Serial.println(" %");

  delay(500);  // Adjusted delay for more responsive readings
}

void sendEmailNotification() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(iftttWebhookUrl);           // Specify the URL
    int httpResponseCode = http.POST("");  // Send the request

    if (httpResponseCode > 0) {
      Serial.print("Email sent, response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending email, code: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Free resources
  } else {
    Serial.println("Wi-Fi Disconnected. Unable to send email.");
  }
}
