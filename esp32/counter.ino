bool IR1_state_old = 0;
bool IR2_state_old = 0;
bool IR1_state_new = 0;
bool IR2_state_new = 0;
int checkin = 0;
int checkout = 0;

void counter() {
  if (analogRead(PHOTO1_PIN) < 1000) {
    IR1_state_new = false;
  }
  else {
    IR1_state_new = true;
  }
  if (analogRead(PHOTO2_PIN) < 1000) {
    IR2_state_new = false;
  }
  else {
    IR2_state_new = true;
  }

  if (IR1_state_new != IR1_state_old || IR2_state_new != IR2_state_old) {
    if (IR1_state_new != IR1_state_old && IR2_state_new != IR2_state_old) {
      // одновременно изменилось состояние у обоих датчиков
      // определить направление невозможно
    }
    else {
      if (IR1_state_new == 1 && IR2_state_new == 0) {
        checkin = 1;
      }
      if (IR1_state_new == 0 && IR2_state_new == 1) {
        checkout = 1;
      }
    }
    IR1_state_old = IR1_state_new;
    IR2_state_old = IR2_state_new;
  }
  if ( Human_Counter_New < 0 ) {
    Human_Counter_New = 0;
  }


  if (checkin == 1) {
    if (IR1_state_new == 1 && IR2_state_new == 1) {
      checkin = 2;
    }
  }

  if (checkout == 1) {
    if (IR1_state_new == 1 && IR2_state_new == 1) {
      checkout = 2;
    }
  }

  if (checkin == 2) {
    if (IR1_state_new < IR2_state_new) {
      checkin = 3;
      checkout = 0;
    }
  }

  if (checkout == 2) {
    if (IR1_state_new > IR2_state_new) {
      checkout = 3;
      checkin = 0;
    }
  }

  if (checkin == 3) {
    if (IR1_state_new == 0 && IR2_state_new == 0) {
      Human_Counter_New++;
      checkin = 0;
      checkout = 0;
    }
  }

  if (checkout == 3) {
    if (IR1_state_new == 0 && IR2_state_new == 0) {
      if (Human_Counter_New > 0) {
        Human_Counter_New--;
      }
      checkout = 0;
      checkin = 0;
    }
  }

}
