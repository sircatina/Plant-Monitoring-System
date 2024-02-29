#include <WiFi.h>
#include <WiFiAP.h>
#include <WebServer.h>

const char *SSID = "camiTina";
WebServer server(80);

String sensorData = "";
String humidity = "N/A", temperature = "N/A", soilMoisture = "N/A", waterLevel = "N/A";

#define RX2 16
#define TX2 17
 
int humidityOK = 0;
int temperatureOK = 0;
int soilMoistureOK = 0;
int waterLevelOK = 0;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  if (!WiFi.softAP(SSID)) {
    Serial.println("Unable to start SoftAP mode");
    while (1);
  }

  IPAddress IP = WiFi.softAPIP();
  Serial.print("HTTP server started --> IP addr: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", generateHTML());
  });

  server.begin();
}

void loop() {
  static int readState = 0;

  if (Serial2.available()) {
    sensorData = Serial2.readStringUntil('\n');
    sensorData.trim();

    if (sensorData == "Transmit:") {
      readState = 1;
    } else {
      switch (readState) {
        case 1: 
          humidity = sensorData;
          readState++;
          break;
        case 2:
          temperature = sensorData;
          readState++;
          break;
        case 3:
          soilMoisture = sensorData;
          readState++;
          break;
        case 4:
          waterLevel = sensorData;
          readState = 0;
          plantState();
          break;
      }
    }
  }
  server.handleClient();
}

void plantState()
{
  float floatHumidity = humidity.toFloat();
  float floatTemperature = temperature.toFloat();
  int intSoil = soilMoisture.toInt();
  int intWater = waterLevel.toInt();
  Serial.println(floatHumidity);
  Serial.println(floatTemperature);
  Serial.println(intSoil);
  Serial.println(intWater);
  
  if (floatHumidity  >= 60.00 && floatHumidity  <= 70.00)
  {
    humidityOK = 1;
  }
  else 
  {
    humidityOK = 0;
  }

  if (floatTemperature >= 20.00 && floatTemperature <= 30.00)
  {
    temperatureOK = 1;
  }
  else 
  {
    temperatureOK = 0;
  }

  if (intSoil >= 200 && intSoil <= 300)
  {
    soilMoistureOK = 1;
  }
  else 
  {
    soilMoistureOK = 0;
  }

  if (intWater >= 0 && intWater <= 500)
  {
    waterLevelOK = 1;
  }
  else 
  {
    waterLevelOK = 0;
  }
  
}

String generateHTML() {
  String html = "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Plant Monitoring System</title></head><body>";
  html += "<h1>Welcome to your plant monitoring system!</h1>";
  html += "<p>Current Humidity: " + humidity + "%</p>";
  html += "<p>Current Temperature: " + temperature + "°C</p>";
  html += "<p>Current Soil Moisture: " + soilMoisture + "</p>";
  html += "<p>Current Water Level: " + waterLevel + "</p>";
  
  html += "<button onclick=\"location.reload();\">Update Information</button>";

  // Humidity
  html += "<p>";
  if (humidityOK == 1) {
    html += "<span style='color: green;'>Humidity: Plant is happy with this humidity</span>";
  } else {
    html += "<span style='color: red;'>Humidity: Not good for the plant</span>";
  }
  html += "</p>";

  // Temperature
  html += "<p>";
  if (temperatureOK == 1) {
    html += "<span style='color: green;'>Temperature: Plant is comfortable with this temperature</span>";
  } else {
    html += "<span style='color: red;'>Temperature: Not ideal for the plant</span>";
  }
  html += "</p>";

  // Soil Moisture
  html += "<p>";
  if (soilMoistureOK == 1) {
    html += "<span style='color: green;'>Soil Moisture: Plant is happy with this moisture level</span>";
  } else {
    int intSoil = soilMoisture.toInt();
    if (intSoil > 300)
    {
      html += "<span style='color: red;'>Soil Moisture: The soil is too dry! The plant needs some water. </span>";
    } 
    else if (intSoil < 200)
    {
      html += "<span style='color: red;'>Soil Moisture: The soil is too moist, you put too much water. </span>";
    }
  }
  html += "</p>";

  // Water Level
  html += "<p>";
  if (waterLevelOK == 1) {
    html += "<span style='color: green;'>Water Level: Sufficient for the plant</span>";
  } else {
    html += "<span style='color: red;'>Water Level: Too much water in the plant bowl</span>";
  }
  html += "</p>";

  html += "</body></html>";
  return html;
}
