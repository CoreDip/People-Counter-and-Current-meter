boolean conf = false;
long restarting_time = 0;
bool restarting = false;
String str = "";
const char * host  = "localhost";

String html_header = "<html>\
  <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">\
  <head>\
    <title> Settings</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
      input {width: 100%; height: 10%; font-size: 300%;}\
      .t1 {font-size: 250%;}\
    </style>\
  </head>";

void handleRoot() {
  String str = "";
  str += html_header;
  str += "<body>\
    <form align=\"center\" method=\"POST\" action=\"ok\">\
      <div class=\"t1\">Налаштування WiFi:</div>\
      <input name=\"ssid\"  placeholder=\"SSID\"></br>\
      <input name=\"pswd\" placeholder=\"Password\"></br></br>\
      <div class=\"t1\">Хост сайту виводу статистики:</div>\
      <input name=\"host\" placeholder=\"Host\"></br>\
      <div class=\"t1\">Налаштування Telegram Bot:</div>\
      <input name=\"token\" placeholder=\"Token\"></br>\
      <input name=\"chatid\" placeholder=\"Chat Id\"></br></br>\
      <input type=SUBMIT value=\"Зберегти\">\
    </form>\
  </body>\
</html>";
  server.send ( 200, "text/html", str );
}

void handleOk() {
  String ssid_ap;
  String pass_ap;
  String host_ap;
  String token_ap;
  String chatid_ap;
  unsigned char* buf = new unsigned char[64];

  String str = "";
  str += html_header;
  str += "<body>";

  EEPROM.begin(512);

  ssid_ap = server.arg(0);
  pass_ap = server.arg(1);
  host_ap = server.arg(2);
  token_ap = server.arg(3);
  chatid_ap = server.arg(4);

  if (ssid_ap != "") {
    EEPROM.write(400, ssid_ap.length());
    EEPROM.write(402, pass_ap.length());
    EEPROM.write(404, host_ap.length());
    EEPROM.write(406, token_ap.length());
    EEPROM.write(408, chatid_ap.length());

    ssid_ap.getBytes(buf, ssid_ap.length() + 1);
    for (byte i = 0; i < ssid_ap.length(); i++)
      EEPROM.write(i, buf[i]);

    pass_ap.getBytes(buf, pass_ap.length() + 1);
    for (byte i = 0; i < pass_ap.length(); i++)
      EEPROM.write(i + 32, buf[i]);

    host_ap.getBytes(buf, host_ap.length() + 1);
    for (byte i = 0; i < host_ap.length(); i++)
      EEPROM.write(i + 96, buf[i]);

    token_ap.getBytes(buf, token_ap.length() + 1);
    for (byte i = 0; i < token_ap.length(); i++)
      EEPROM.write(i + 128, buf[i]);

    chatid_ap.getBytes(buf, chatid_ap.length() + 1);
    for (byte i = 0; i < chatid_ap.length(); i++)
      EEPROM.write(i + 192, buf[i]);

    EEPROM.commit();
    EEPROM.end();

    str += "<div class=\"t1\">Дані збережено до пам'яті пристрою!</div></br>\
    <div class=\"t1\">Пристрій перезавантажиться для прийняття нових налаштувань!</div></p></br></br>";
    str += "</body></html>";
    server.send ( 200, "text/html", str );
    restarting_time = millis();
    restarting = true;
  }
  else {
    str += "Нічого не задано</br>\
    <div class=\"t1\"><a href=\"/\">Повернутись</a> на сторінку налаштувань</div></br>";
    str += "</body></html>";
    server.send ( 200, "text/html", str );
  }
}

void change_settings() {
  server.handleClient();
  if ((digitalRead(RESET_PIN) == LOW) && !conf) {
    EEPROM.write(400, 255);
    EEPROM.commit();
    EEPROM.end();
    conf = true;
    Serial.println("Перезавантаження в режим Налаштувань...");
    restarting_time = millis();
    restarting = true;
  }
  if (millis() - restarting_time > 3000 && restarting == true) {
    ESP.restart();
  }
}

void esp_mode() {
  byte len_ssid, len_pass, len_host, len_chatid, len_token;
  EEPROM.begin(512);
  len_ssid = EEPROM.read(400);
  len_pass = EEPROM.read(402);
  len_host = EEPROM.read(404);
  len_token = EEPROM.read(406);
  len_chatid = EEPROM.read(408);

  if (len_pass > 64) len_pass = 0;
  if (len_host > 32) len_host = 0;
  if (len_token > 64) len_token = 0;
  if (len_chatid > 32) len_chatid = 0;

  pinMode(RESET_PIN, INPUT_PULLUP);
  if ((len_ssid < 33) && (len_ssid != 0)) {
    // Режим STATION
    WiFi.mode( WIFI_STA);
    unsigned char* buf_ssid = new unsigned char[32];
    unsigned char* buf_pass = new unsigned char[64];
    unsigned char* buf_host = new unsigned char[32];
    unsigned char* buf_token = new unsigned char[64];
    unsigned char* buf_chatid = new unsigned char[32];

    for (byte i = 0; i < len_ssid; i++) buf_ssid[i] = char(EEPROM.read(i));
    buf_ssid[len_ssid] = '\x0';
    const char *ssid  = (const char*)buf_ssid;
    Serial.println(ssid);

    for (byte i = 0; i < len_pass; i++) buf_pass[i] = char(EEPROM.read(i + 32));
    const char *pass  = (const char*)buf_pass;
    buf_pass[len_pass] = '\x0';
    Serial.println(pass);

    for (byte i = 0; i < len_host; i++) buf_host[i] = char(EEPROM.read(i + 96));
    host  = (const char*)buf_host;
    buf_host[len_host] = '\x0';
    Serial.println(host);

    for (byte i = 0; i < len_token; i++) buf_token[i] = char(EEPROM.read(i + 128));
    token  = (const char*)buf_token;
    buf_token[len_token] = '\x0';
    Serial.println(token);

    for (byte i = 0; i < len_chatid; i++) buf_chatid[i] = char(EEPROM.read(i + 192));
    chatid  = (const char*)buf_chatid;
    buf_chatid[len_chatid] = '\x0';
    Serial.println(chatid);

    WiFi.begin(ssid, pass);
    Serial.println("Пристрій приєднано до мережі WiFi");
  }
  else // Режим SoftAP
  {
    const char *ssid_ap = "ESP32";
    WiFi.mode(WIFI_AP);
    mode = "AP";
    Serial.println("Налаштування параметрів...");
    WiFi.softAP(ssid_ap);
    Serial.println("Точку Доступу створено");
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
    server.on("/", handleRoot);
    server.on("/ok", handleOk);
    server.begin();
    Serial.println("HTTP сервер ввімкнено");
  }
}
