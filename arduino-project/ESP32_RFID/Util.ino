
void showScanTag() {
  lcd.clear();
  
  onlyLedHIGH(YELLOW_LED);

  lcd.print("   SCAN YOUR    ");
  lcd.setCursor(0, 1);
  lcd.print("      TAG       ");

  actionTriggered = false;
  showScanTagDisplay = false;
}

void accesGranted(String user_name, String login) {
  lcd.clear();

  onlyLedHIGH(GREEN_LED);
  myservo.write(180);

  lcd.print(" Access Granted! ");
  lcd.setCursor(0, 1);
  if(login == "login"){
    lcd.print(String("-> hi, " + user_name));
  } else {
    lcd.print(String("-> bye, " + user_name));
  }

  actionTriggered = true;
  showScanTagDisplay = false;
  previousMillisAction = millis();
}

void accesDenied(String error) {
  lcd.clear();

  onlyLedHIGH(RED_LED);

  lcd.print(" Access Denied! ");
  lcd.setCursor(0, 1);
  lcd.print(String("-> " + error));

  actionTriggered = true;
  showScanTagDisplay = false;
  previousMillisAction = millis();
}

void enrollmentSameTag() {
  lcd.clear();

  allLedsHIGH();

  lcd.print(" Already there! ");
  lcd.setCursor(0, 1);
  lcd.print(String("-> ") + String("register/up"));

  actionTriggered = true;
  showScanTagDisplay = false;
  previousMillisAction = millis();
}

void enrollmentNewTag() {
  lcd.clear();

  allLedsHIGH();

  lcd.print(" New tag found! ");
  lcd.setCursor(0, 1);
  lcd.print(String("-> ") + String("card added "));

  actionTriggered = true;
  showScanTagDisplay = false;
  previousMillisAction = millis();
}
