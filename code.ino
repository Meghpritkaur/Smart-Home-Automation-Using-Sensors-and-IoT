#define BLYNK_TEMPLATE_ID "TMPL35Qcw0CgQ"
#define BLYNK_TEMPLATE_NAME "Practical15"
#define BLYNK_AUTH_TOKEN "jxoFaPNc0jsBuNDatATkJq7z4l-gg_GN"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <BlynkTimer.h>

// Auth and Wi-Fi
char auth[] = "jxoFaPNc0jsBuNDatATkJq7z4l-gg_GN";
char ssid[] = "OnePlus";
char pass[] = "12345";

// GPIO Definitions
#define FAN_PIN 25
#define TUBELIGHT_PIN 26
#define LDR_PIN 36      // Analog pin for LDR (GPIO 36)
#define DHT_PIN 4       // Digital pin for DHT11

// Blynk Virtual Pins
#define FAN_LED_VPIN V3
#define TUBELIGHT_LED_VPIN V4
#define FAN_OFF_LED_VPIN V5
#define TUBELIGHT_OFF_LED_VPIN V6

// Thresholds
#define TEMP_THRESHOLD 25.0  // Temp in °C
#define LDR_THRESHOLD 1500   // Lower value = darker (you may calibrate this)

// Sensor setup
DHT dht(DHT_PIN, DHT11);
BlynkTimer timer;

void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

  pinMode(FAN_PIN, OUTPUT);
  pinMode(TUBELIGHT_PIN, OUTPUT);
  digitalWrite(FAN_PIN, HIGH);       // Fan OFF
  digitalWrite(TUBELIGHT_PIN, HIGH); // Tubelight OFF

  timer.setInterval(3000L, checkSensors); // Every 3 seconds
}

void checkSensors() {
  // Read DHT11 temperature
  float temp = dht.readTemperature();
  Serial.print("Temperature: ");
  Serial.println(temp);

  // Control Fan
  if (!isnan(temp) && temp > TEMP_THRESHOLD) {
    digitalWrite(FAN_PIN, LOW); // Fan ON
    Blynk.virtualWrite(FAN_LED_VPIN, 255);
    Blynk.virtualWrite(FAN_OFF_LED_VPIN, 0);
  } else {
    digitalWrite(FAN_PIN, HIGH); // Fan OFF
    Blynk.virtualWrite(FAN_LED_VPIN, 0);
    Blynk.virtualWrite(FAN_OFF_LED_VPIN, 255);
  }

  // Read LDR
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Control Tubelight
  if (ldrValue < LDR_THRESHOLD) { // Dark
    digitalWrite(TUBELIGHT_PIN, LOW); // Tubelight ON
    Blynk.virtualWrite(TUBELIGHT_LED_VPIN, 255);
    Blynk.virtualWrite(TUBELIGHT_OFF_LED_VPIN, 0);
  } else {
    digitalWrite(TUBELIGHT_PIN, HIGH); // Tubelight OFF
    Blynk.virtualWrite(TUBELIGHT_LED_VPIN, 0);
    Blynk.virtualWrite(TUBELIGHT_OFF_LED_VPIN, 255);
  }
}

void loop() {
  Blynk.run();
  timer.run();
}
