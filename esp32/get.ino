//const char* host = "32.monitoring-esp.ml";
const char* key = "PaSW0rD";
int offwebsend = 0;
void sendtoweb(int people) {
  if (host != "localhost") {
    if (client.connect(host, 80, 25)) {
      String url = "/add.php";
      url += "?people=";
      if (people > 0) {
        url += people;
      }
      if (people == 0) {
        url += "00";
      }
      url += "&key=";
      url += key;
      Serial.print("Запит: ");
      Serial.println(url);
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                   "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
      Serial.println("Дані передано на сайт");
    }
    else {
      Serial.println("Данні на сайт статисктики НЕ відправлено!");
    }
  }
  else {
    Serial.println("Дані не передано на сайт, оскільки хост вказано не коректно!");
  }
}
