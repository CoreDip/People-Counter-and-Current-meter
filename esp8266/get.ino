const char* key = "PaSW0rD";
void sendtoweb(double Current, double Wh) {
  if (host != "localhost") {
    if (client.connect(host, 80)) { //works!
      String url = "/add.php";
      url += "?I=";
      url += Current;
      url += "&Wh=";
      url += Wh;
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
