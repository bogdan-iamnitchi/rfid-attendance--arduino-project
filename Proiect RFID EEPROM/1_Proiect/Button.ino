////////////////////////// Check if reset button is pressed ///////////////
void checkButton()
{
  if (digitalRead(wipeB) == HIGH) {
    digitalWrite(redLed, HIGH);
    showButtonLCD();
    digitalWrite(BuzzerPin, HIGH);
    delay(500);
    digitalWrite(BuzzerPin, LOW);
    showRemoveLCD();
    
    bool buttonState = monitorWipeButton(5000);
    if (buttonState == true && digitalRead(wipeB) == LOW) {
      lcd.print("Wiping EEPROM...");
      for (uint16_t x = 0; x < EEPROM.length(); x = x + 1) {
        if (EEPROM.read(x) != 0) {
          EEPROM.write(x, 0);
        }
      }
      showWipingDone();

      digitalWrite(redLed, LOW);
      digitalWrite(BuzzerPin, HIGH);
      delay(100);
      digitalWrite(redLed, HIGH);
      digitalWrite(BuzzerPin, LOW);
      delay(100);
      digitalWrite(redLed, LOW);
      digitalWrite(BuzzerPin, HIGH);
      delay(100);
      digitalWrite(redLed, HIGH);
      digitalWrite(BuzzerPin, LOW);
      delay(100);
      
      showResetLCD();
      while(1);
    }
    else {
      showWipingFail();
      digitalWrite(redLed, LOW);
      delay(1500);
      showScanTag();
    }
  }
}

/////////////////// Counter to check in reset/wipe button is pressed or not   /////////////////////
bool monitorWipeButton(uint32_t interval) {
  unsigned long currentMillis = millis();
  while (millis() - currentMillis < interval)  {
    int timeSpent = (millis() - currentMillis) / 1000;
    lcd.setCursor(10, 1);
    lcd.print(timeSpent);
    // check on every half a second
    if (((uint32_t)millis() % 10) == 0) {
      if (digitalRead(wipeB) != LOW) {
        return false;
      }
    }
  }
  return true;
}
