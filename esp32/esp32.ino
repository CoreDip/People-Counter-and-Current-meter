#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "AsyncUDP.h"
#include <WebServer.h>
#include <EEPROM.h>

#define BIP_PIN 14
#define PHOTO1_PIN 33
#define PHOTO2_PIN 32
#define RESET_PIN 25

double Imax = 2; //допустиме значення струму в амперахдля пустого приміщення.
double Ivalue;
int Human_Counter_New;
int bip1 = 0;
int bip2 = 0;
const char * mode = "STA";
const char *token = "";
const char *chatid = "";

AsyncUDP udp;
TaskHandle_t Task1;
WiFiClientSecure secured_client;
WiFiClient client;
WebServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Пристрій ввімкнено!.");
  esp_mode();
  if (mode == "STA") {
    udp_start();
    UniversalTelegramBot bot(token, secured_client);
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
    bot.sendMessage(chatid, "Плата esp32 запущена!\nМоніторинг: monitoring-esp.ml", "");
    task_start();
  }
  pinMode(BIP_PIN, OUTPUT);
  bip1 = 1;
}

void loop() {
  if (mode == "STA") {
    telegram();
    bip();
    counter();
  }
  change_settings();
}
