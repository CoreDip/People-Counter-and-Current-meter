String msg0 = "testmessage0";
String msg1 = "testmessage1";
String msg2 = "testmessage2";
String reply_markup = "";
bool tg_must_send0 = false;
bool tg_must_send1 = false;
bool tg_must_send2 = false;

void Task1code( void * parameter) {
  for (;;) {
    UniversalTelegramBot bot(token, secured_client);
    delay(5);
    if (tg_must_send0 == true) {
      bot.sendMessage(chatid, msg0, "");
      delay(5);
      tg_must_send0 = false;
    }
    if (tg_must_send1 == true) {
      bot.sendMessage(chatid, msg1, "");
      delay(5);
      tg_must_send1 = false;
    }

    if (tg_must_send2 == true) {
      bot.sendMessage(chatid, msg2, "");
      delay(5);
      tg_must_send2 = false;
    }
  }
}

void task_start() {
  xTaskCreatePinnedToCore(
    Task1code, /* Функция, содержащая код задачи */
    "Task1", /* Название задачи */
    100000, /* Размер стека в словах */
    NULL, /* Параметр создаваемой задачи */
    1, /* Приоритет задачи */
    &Task1, /* Идентификатор задачи */
    0); /* Ядро, на котором будет выполняться задача */
}
