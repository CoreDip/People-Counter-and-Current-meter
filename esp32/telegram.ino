int prev_count_people = 0;
long prev_seconds;
long prev_seconds2;
long prev_seconds3;
long signalizate = 0;
bool enable_signalizate = false;

void telegram() {
  if (millis() != prev_seconds) {
    prev_seconds = millis();
    if (Human_Counter_New > prev_count_people) {
      bip1 = 1;
      prev_count_people = Human_Counter_New;
      Serial.println("Зайшов 1 чол.");
      Serial.println("Зараз людей: " + String(Human_Counter_New));
      msg0 = "Зайшов 1 чол.\n";
      msg0 += "Вдома: ";
      msg0 += Human_Counter_New;
      msg0 += " чол.\nМоніторинг: monitoring-esp.ml";
      //bot.sendMessage(CHAT_ID, msg0, "");
      tg_must_send0 = true;
      sendtoweb(Human_Counter_New);
    }
    if (Human_Counter_New < prev_count_people) {
      bip2 = 1;
      prev_count_people = Human_Counter_New;
      Serial.println("Вийшов 1 чол.");
      Serial.println("Зараз людей: " + String(Human_Counter_New));
      msg1 = "Вийшов 1 чол.\n";
      msg1 += "Вдома: ";
      msg1 += Human_Counter_New;
      msg1 += " чол.\nМоніторинг: monitoring-esp.ml";
      //bot.sendMessage(CHAT_ID, msg0, "");
      tg_must_send1 = true;
      sendtoweb(Human_Counter_New);
    }
    if (Ivalue > 2 && Human_Counter_New == 0 && ((millis() - prev_seconds2) > 5000)) {
      msg2 = "Вдома нікого немає!\nА споживання струму: ";
      msg2 += Ivalue;
      msg2 += " А.\nВатт: ";
      msg2 += Ivalue * 220;
      msg2 += " Вт/год.\nМоніторинг: monitoring-esp.ml";
      //bot.sendMessage(CHAT_ID, msg0, "");
      tg_must_send2 = true;
      prev_seconds2 = millis();
    }
    if (Ivalue > 2 && Human_Counter_New == 0 && ((millis() - prev_seconds3) > 300) && signalizate < 500) {
      bip_change_state();
      prev_seconds3 = millis();
      signalizate++;
      enable_signalizate = true;
    }
    if (signalizate % 2 == 0 && Human_Counter_New > 0 && enable_signalizate == true) {
      digitalWrite(14, LOW);
      enable_signalizate = false;
      signalizate = 0;
    }
    if (Ivalue < Imax && enable_signalizate == true) {
      digitalWrite(14, LOW);
      enable_signalizate = false;
      signalizate = 0;
    }
  }
}
