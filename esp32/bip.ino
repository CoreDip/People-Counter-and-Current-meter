unsigned long bipstart;
unsigned long bipbipstart;

void bip() {
  if (bip1 == 1) {
    bipstart = millis();
    digitalWrite(BIP_PIN, HIGH);
    bip1 = 2;
  }
  if ((millis() - bipstart) > 100 && bip1 == 2) {
    digitalWrite(BIP_PIN, LOW);
    bip1 = 0;
  }

  if (bip2 == 1) {
    bipbipstart = millis();
    digitalWrite(BIP_PIN, HIGH);
    bip2 = 2;
  }
  if ((millis() - bipbipstart) > 100 && bip2 == 2) {
    digitalWrite(BIP_PIN, LOW);
    bip2 = 3;
  }
  if ((millis() - bipbipstart) > 120 && bip2 == 3) {
    digitalWrite(BIP_PIN, HIGH);
    bip2 = 4;
  }
  if ((millis() - bipbipstart) > 220 && bip2 == 4) {
    digitalWrite(BIP_PIN, LOW);
    bip2 = 0;
  }
}

void bip_change_state() {
  digitalWrite(BIP_PIN, !digitalRead(BIP_PIN));
}
