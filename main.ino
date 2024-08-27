#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NewPing.h>

#define SOIL_SENSOR_PIN 32
#define AIR_VALUE 560
#define WATER_VALUE 440
#define RELAY_PIN 18
#define TRIG_PIN 4
#define ECHO_PIN 4

int soilMoistureRaw = 0;
int soilMoisturePercent = 0;

Adafruit_SSD1306 display(128, 64, &Wire, -1);
NewPing sonar(TRIG_PIN, ECHO_PIN, 15);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Start with the relay off
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Initialize with the I2C addr 0x3C (128x64)
  display.clearDisplay();
}

void loop() {
  soilMoistureRaw = analogRead(SOIL_SENSOR_PIN);
  soilMoisturePercent = constrain(map(soilMoistureRaw, AIR_VALUE, WATER_VALUE, 0, 100), 0, 100);

  // Control the relay based on soil moisture level
  if (soilMoisturePercent < 40) {
    digitalWrite(RELAY_PIN, LOW);  // Activate relay (assumes LOW activates the relay)
  } else {
    digitalWrite(RELAY_PIN, HIGH);  // Deactivate relay
  }

  display.clearDisplay();
  display.setCursor(10, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.println("Soil Moisture");

  display.setCursor(30, 40);
  display.setTextSize(3);
  display.print(soilMoisturePercent);
  display.println(" %");

  display.display();
  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisturePercent);
  Serial.println(" %");

  delay(250);  // Adjusted delay for more responsive readings
}
