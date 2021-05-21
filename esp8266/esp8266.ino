#include "EmonLib.h"
#include <ESP8266WiFi.h>
#include "ESPAsyncUDP.h"
#include <ESP8266WebServer.h>
#include <EEPROM.h>

AsyncUDP udp;
EnergyMonitor emon1;
ESP8266WebServer server(80);
WiFiClient client;

#define ADC_INPUT A0
#define RESET_PIN 14
const char * mode = "STA";

void setup()
{
  Serial.begin(115200);
  esp_mode();
  pinMode(ADC_INPUT, INPUT);
  emon1.current(ADC_INPUT, 109);
}

void loop()
{
  if (mode == "STA") {
  meter();
  }
  change_settings();
}
