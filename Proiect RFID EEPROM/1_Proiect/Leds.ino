//////////////////////////////////////// Cycle Leds (Program Mode) ///////////////////////////////////
void cycleLeds() {
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  digitalWrite(blueLed, LOW);
  delay(100);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, HIGH);
  delay(100);
  digitalWrite(redLed, HIGH);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
  delay(100);
}


//////////////////////////////////////// Normal Mode Led  ///////////////////////////////////
void normalModeOn () {
  digitalWrite(blueLed, HIGH);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
}


///////////////////////////////////////// Blink LED's For Indication   ///////////////////////////////////
void BlinkLEDS(int led) {
  digitalWrite(blueLed, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(BuzzerPin, HIGH);
  delay(200);
  digitalWrite(led, HIGH);
  digitalWrite(BuzzerPin, LOW);
  delay(200);
  digitalWrite(led, LOW);
  digitalWrite(BuzzerPin, HIGH);
  delay(200);
  digitalWrite(led, HIGH);
  digitalWrite(BuzzerPin, LOW);
  delay(200);
  digitalWrite(led, LOW);
  digitalWrite(BuzzerPin, HIGH);
  delay(200);
  digitalWrite(led, HIGH);
  digitalWrite(BuzzerPin, LOW);
  delay(200);
}


////////////////////// Print Info on LCD   ///////////////////////////////////
void ShowOnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ACCESS CONTROL");
  lcd.setCursor(0, 1);
  lcd.print("   Scan a Tag");
}
