void loop() {
  int val=0;
  int manualMode = 0;
  int rez = 0;
  int cnt = 0;
  char customKey;
  do { 
    customKey = customKeypad.getKey();
    val = analogRead(joy);
    val = map(val, 0, 1023, 0, 180);
    
    successRead = getID();
    if (programMode) {
      checkButton();

      // intra in modul manual
      if(customKey == 'A'){
        showManualMode();
        manualMode = 1;
      }

      // iese din modul manual
      if(customKey == 'B'){
        manualMode = 0;
      }

      // deshide usa
      if(manualMode == -2){
        myServo.write(val);
      }

      // cycle leds
      if(manualMode == -1){
        cycleLeds();
      }

      // asteapta sa se apese A
      if(manualMode == 0){
        showScanTag();
        manualMode = -1;
      }

      // s-a apasat A si se asteapta parola
      if(manualMode == 1){
        if(customKey >= '0' && customKey <='9'){
          rez = rez*10 + (customKey - 48);
          lcd.print(customKey);
          cnt++;
        }
        if(cnt == passLenght){
          if(rez == password){
            manualMode = 3;
            rez = 0;
            cnt = 0;
          }
          else{
            manualMode = 2;
            rez = 0;
            cnt = 0;
          }  
        }
      }

      // parola incorecta
      if(manualMode == 2)
      {
        showIncorrectPass();
        manualMode = 0;
      }

      // parola corecta --> redirect
      if(manualMode == 3){
        showCorrectPass();
        manualMode = -2;
      }
    }
    else {
      normalModeOn(); 
    }
  }
  while (!successRead);
  
  //-------------------------------------------------------------------------------------------
  if (programMode) {
    if ( isMaster(readCard) ) {
      showExit();
      showAccessLCD();
      programMode = false;
      return;
    }
    else {
      if ( findID(readCard) ) {
        alreadyThere();
        deleteID(readCard);
        showScanTag();
      }
      else {
        adding();
        writeID(readCard);
        showScanTag();
      }
    }
  }
  else {
    if ( isMaster(readCard)) {
      programMode = true;

      for(int i=0; i<256; i++)
        {
          if(scanari[i]){
            Serial.println("------------------------------------------------");
            Serial.print("Angajatul cu codul : ");
            Serial.println(i);
            Serial.print("a intrat in cladire de ");
            if(scanari[i] % 2){
              Serial.print(scanari[i]/2 + 1); Serial.println(" ori");
            }
            else if(scanari[i] % 2 == 0){
              Serial.print(scanari[i]/2); Serial.println(" ori");
            }
            Serial.print("a parasit cladirea de ");
            Serial.print(scanari[i]/2); Serial.println(" ori");
            Serial.println("------------------------------------------------");
          }
        }
      
      showProgramMode();
      showScanTag();
    }
    else {
      if ( findID(readCard) ) {
        accessGranted();
        
        scanari[readCard[0]]++;
        Serial.println("------------------------------------------------");
        Serial.print("Angajatul cu codul : ");
        Serial.print(readCard[0]);
        if(scanari[readCard[0]] % 2 == 0)
          Serial.println(" a parasit cladirea");
        else
          Serial.println(" a intrat in cladire");
        Serial.println("------------------------------------------------");
        
        granted();
        myServo.write(90);
        showAccessLCD();
      }
      else {
        accessDenied();
        denied();
        showAccessLCD();
      }
    }
  }
}
