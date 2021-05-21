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
      <div class=\"t1\">Введіть дані WiFi мережі:</div>\
      <input name=\"ssid\"  placeholder=\"SSID\"></br>\
      <input name=\"pswd\" placeholder=\"Password\"></br>\
      <input name=\"host\" placeholder=\"host\"></br></br>\
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
  unsigned char* buf = new unsigned char[64];

  String str = "";
  str += html_header;
  str += "<body>";

  EEPROM.begin(256);

  ssid_ap = server.arg(0);
  pass_ap = server.arg(1);
  host_ap = server.arg(2);

  if (ssid_ap != "") {
    EEPROM.write(95, host_ap.length());
    EEPROM.write(96, ssid_ap.length());
    EEPROM.write(97, pass_ap.length());

    host_ap.getBytes(buf, host_ap.length() + 1);
    for (byte i = 0; i < host_ap.length(); i++)
      EEPROM.write(i, buf[i]);

    ssid_ap.getBytes(buf, ssid_ap.length() + 1);
    for (byte i = 0; i < ssid_ap.length(); i++)
      EEPROM.write(i + 32, buf[i]);

    pass_ap.getBytes(buf, pass_ap.length() + 1);
    for (byte i = 0; i < pass_ap.length(); i++)
      EEPROM.write(i + 64, buf[i]);



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
    EEPROM.write(96, 255);
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
  byte len_ssid, len_pass, len_host;
  EEPROM.begin(256);
  len_host = EEPROM.read(95);
  len_ssid = EEPROM.read(96);
  len_pass = EEPROM.read(97);

  if (len_pass > 64) len_pass = 0;
  if (len_host > 32) len_host = 0;

  pinMode(RESET_PIN, INPUT_PULLUP);
  if ((len_ssid < 33) && (len_ssid != 0)) {
    // Режим STATION
    WiFi.mode( WIFI_STA);
    unsigned char* buf_ssid = new unsigned char[32];
    unsigned char* buf_pass = new unsigned char[64];
    unsigned char* buf_host = new unsigned char[32];

    for (byte i = 0; i < len_host; i++) buf_host[i] = char(EEPROM.read(i));
    host  = (const char*)buf_host;
    buf_host[len_host] = '\x0';
    //Serial.println(host);
    for (byte i = 0; i < len_ssid; i++) buf_ssid[i] = char(EEPROM.read(i + 32));
    buf_ssid[len_ssid] = '\x0';
    const char *ssid  = (const char*)buf_ssid;
    //Serial.println(ssid);
    for (byte i = 0; i < len_pass; i++) buf_pass[i] = char(EEPROM.read(i + 64));
    const char *pass  = (const char*)buf_pass;
    buf_pass[len_pass] = '\x0';
    //Serial.println(pass);

    WiFi.begin(ssid, pass);
    Serial.println("Пристрій приєднано до мережі WiFi");
  }
  else // Режим SoftAP
  {
    const char *ssid_ap = "ESP8266";
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
