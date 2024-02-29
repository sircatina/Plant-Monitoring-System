#include "DHT.h"
#include <Adafruit_Sensor.h>

#include <Wire.h>
#include <BH1750.h>

#define AOUT_PIN A0
#define THRESHOLD 2000

#define DHTPIN  4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define POWER_PIN  7
#define SIGNAL_PIN A5
int valueWater = 0;

BH1750 lightMeter;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600);
  
  pinMode(POWER_PIN, OUTPUT);
  digitalWrite(POWER_PIN, LOW);
  Wire.begin(); // For BH1750
  dht.begin();
}

void loop() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  int soilMoisture = analogRead(AOUT_PIN);
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoisture);

  digitalWrite(POWER_PIN, HIGH);  
  delay(10);                   
  valueWater = analogRead(SIGNAL_PIN); 
  digitalWrite(POWER_PIN, LOW);   
  Serial.print("Water Level: ");
  Serial.println(valueWater);

  delay(2000); 

  Serial2.println("Transmit: ");

  Serial2.println(humidity);
  Serial2.println(temperature);
  Serial2.println(soilMoisture);
  Serial2.println(valueWater);
}
