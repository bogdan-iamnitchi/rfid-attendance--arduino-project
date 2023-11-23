/////////////////////// Check if RFID Reader is correctly initialized or not /////////////////////
void ShowReaderDetails() {
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  if ((v == 0x00) || (v == 0xFF)) {
    lcd.setCursor(0, 0);
    lcd.print("Communication Failure");
    lcd.setCursor(0, 1);
    lcd.print("Check Connections");
    digitalWrite(BuzzerPin, HIGH);
    delay(2000);

    digitalWrite(greenLed, LOW);
    digitalWrite(blueLed, LOW);
    digitalWrite(redLed, HIGH);
    digitalWrite(BuzzerPin, LOW);
    while (true);
  }
}


///////////////////// Define master card ///////////////////////
void defineMaster(){
    showNoMastercard();
    do {
      successRead = getID();
      digitalWrite(blueLed, HIGH);
      delay(200);
      digitalWrite(blueLed, LOW);
      delay(200);
    }
    while (!successRead);
    for ( uint8_t j = 0; j < 4; j++ ) {
      EEPROM.write( 2 + j, readCard[j] );
    }
    EEPROM.write(1, 143);
}


///////////////////// Determine password and password length ///////////////////////////////
void determinePass(){
  int x=masterCard[0];
  password = x;
  while(x)
  {
    passLenght++;
    x /= 10;
  }
  x = masterCard[1];
  while(x)
  {
    password = password *10;
    passLenght++;
    x /= 10;
  }
  password += masterCard[1];
}


////////////////////// Check if readCard is master card   ///////////////////////////////////
boolean isMaster( byte test[] ) {
  if ( checkTwo( test, masterCard ) )
    return true;
  else
    return false;
}


///////////////////////////////////////// Get Tag's UID ///////////////////////////////////
uint8_t getID() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  
  for ( uint8_t i = 0; i < 4; i++) {
    readCard[i] = mfrc522.uid.uidByte[i];
  }
  mfrc522.PICC_HaltA();
  return 1;
}


//////////////////////////////////////// Read an ID from EEPROM //////////////////////////////
void readID( uint8_t number ) {
  uint8_t start = (number * 4 ) + 2;
  for ( uint8_t i = 0; i < 4; i++ ) {
    storedCard[i] = EEPROM.read(start + i);
  }
}
///////////////////////////////////////// Add ID to EEPROM   ///////////////////////////////////
void writeID( byte a[] ) {
  if ( !findID( a ) ) {
    uint8_t num = EEPROM.read(0);
    uint8_t start = ( num * 4 ) + 6;
    num++;
    EEPROM.write( 0, num );
    for ( uint8_t j = 0; j < 4; j++ ) {
      EEPROM.write( start + j, a[j] );
    }
    BlinkLEDS(greenLed);
    lcd.setCursor(0, 1);
    lcd.print(" ---> ADDED");
    delay(1000);
  }
  else {
    BlinkLEDS(redLed);
    showError();
    delay(2000);
  }
}
///////////////////////////////////////// Remove ID from EEPROM   ///////////////////////////////////
void deleteID( byte a[] ) {
  if ( !findID( a ) ) {
    BlinkLEDS(redLed);
    showError();
    delay(2000);
  }
  else {
    uint8_t num = EEPROM.read(0);
    uint8_t slot;
    uint8_t start;
    uint8_t looping;
    uint8_t j;
    uint8_t count = EEPROM.read(0);
    slot = findIDSLOT( a );
    start = (slot * 4) + 2;
    looping = ((num - slot) * 4);
    num--;
    EEPROM.write( 0, num );
    for ( j = 0; j < looping; j++ ) {
      EEPROM.write( start + j, EEPROM.read(start + 4 + j));
    }
    for ( uint8_t k = 0; k < 4; k++ ) {
      EEPROM.write( start + j + k, 0);
    }
    BlinkLEDS(blueLed);
    lcd.setCursor(0, 1);
    lcd.print(" ---> REMOVED");
    delay(1000);
  }
}
///////////////////////////////////////// Check Bytes ///////////////////////////////////
boolean checkTwo ( byte a[], byte b[] ) {
  if ( a[0] != 0 )
    match = true;
  for ( uint8_t k = 0; k < 4; k++ ) {
    if ( a[k] != b[k] )
      match = false;
  }
  if ( match ) {      
    return true;
  }
  else  {
    return false;
  }
}


///////////////////////////////////////// Find Slot   ///////////////////////////////////
uint8_t findIDSLOT( byte find[] ) {
  uint8_t count = EEPROM.read(0);
  for ( uint8_t i = 1; i <= count; i++ ) {
    readID(i);
    if ( checkTwo( find, storedCard ) ) {
      return i;
      break;
    }
  }
}


///////////////////////////////////////// Find ID From EEPROM   ///////////////////////////////////
boolean findID( byte find[] ) {
  uint8_t count = EEPROM.read(0);
  for ( uint8_t i = 1; i <= count; i++ ) {
    readID(i);
    if ( checkTwo( find, storedCard ) ) {
      return true;
      break;
    }
    else {
    }
  }
  return false;
}
