long prew = 0;
double I;
unsigned long last_time = 0;
unsigned long current_time = 0;
double Wh = 0;
double V = 220;

void meter() {
    if (millis() - prew > 5000) {
      prew = millis();
      double Current = emon1.calcIrms(1480);
      double P = V * Current;
      last_time = current_time;
      current_time = millis();
      Wh = P * ((current_time - last_time) / 3600000.0);
      sendtoweb(Current, Wh);
      Serial.println(Current);
      Serial.println(Wh);
      char buffer[15];
      dtostrf(Current, 5, 2, buffer);
      udp.broadcastTo(buffer, 8642);
  }
}
