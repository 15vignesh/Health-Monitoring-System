#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include "DHT.h"
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
//#include <WiFi.h>
#define BLYNK_TEMPLATE_ID "TMPLKldSDqbi"
#define BLYNK_TEMPLATE_NAME "HEALTH MONITERING SYSTEM"
#define BLYNK_AUTH_TOKEN "bb9a8NvsBbdByH7OhhhuRSpe47K0kLSr"
char auth[] = "bb9a8NvsBbdByH7OhhhuRSpe47K0kLSr";  // Blynk auth token
char ssid[] = "Galaxy M31EA8A";         // WiFi SSID
char pass[] = "13345678";     // WiFi password

#define REPORTING_PERIOD_MS 1000
#define DHTPIN D5
#define DHTTYPE DHT11

//PulseSensorHW627 pulseSensor;
PulseOximeter pox;
DHT dht(DHTPIN, DHTTYPE);

uint32_t lastReportTime = 0;

void onBeatDetected()
{
  Serial.println("Beat Detected!");
}

void setup()
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth,ssid, pass); // Connect to Blynk server

  // Initialize the MAX30100 module
  if (!pox.begin()) {
    Serial.println("MAX30100 was not found. Please check wiring/power.");
    while (1);
  }

  // Attach the beat detection callback function
  pox.setOnBeatDetectedCallback(onBeatDetected);

  // Initialize the pulse sensor
 // pulseSensor.begin();

  // Initialize the DHT11 sensor
  dht.begin();
}

void loop()
{
   Blynk.run(); // Run Blynk service
  // Update the pulse oximeter
  pox.update();

  // Calculate the heart rate and SpO2 levels
  int heartRate = pox.getHeartRate();
  int spO2 = pox.getSpO2();

  // Read the temperature and humidity from the DHT11 sensor
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read the pulse from the HW-627 sensor
  //int pulse = pulseSensor.getBeatsPerMinute();

  // Print the readings on the serial monitor
  if (millis() - lastReportTime > REPORTING_PERIOD_MS) {
    Serial.print("Heart rate: ");
    Serial.print(heartRate);
    Serial.print(" bpm / SpO2: ");
    Serial.print(spO2);
    Serial.print(" % / Temperature: ");
    Serial.print(temperature);
    Serial.print(" *C / Humidity: ");
    Serial.print(humidity);
    //Serial.print(" % / Pulse: ");
    //Serial.print(pulse);
    //Serial.println(" bpm");
    Blynk.virtualWrite(V1, temperature);
  //Blynk.virtualWrite(V2, pulse);
  Blynk.virtualWrite(V3, heartRate);
  Blynk.virtualWrite(V4, spO2);

    lastReportTime = millis();
  }
}
