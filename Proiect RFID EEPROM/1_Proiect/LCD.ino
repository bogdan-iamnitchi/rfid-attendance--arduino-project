////////////////////// ACCESS CONTROL  ////////////////////////////////
void showAccessLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ACCESS CONTROL");
  lcd.setCursor(0, 1);
  lcd.print("   Scan a Tag");
}

///////////////////////// PROGRAM MODE //////////////////////////////
void showProgramMode(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PROGRAM MODE:");
  uint8_t count = EEPROM.read(0);
  lcd.setCursor(0, 1);
  lcd.print("I have ");
  lcd.print(count);
  lcd.print(" records");
}

//////////////////// BUTTON PRESSED //////////////////////////////////
void showButtonLCD(){
  lcd.clear(); 
  lcd.setCursor(0, 0);
  lcd.print("Button Pressed");
  
}

//////////////////// REMOVE RECORDS //////////////////////////////////
void showRemoveLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("This will remove");
  lcd.setCursor(0, 1);
  lcd.print("all records");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You have 5 ");
  lcd.setCursor(0, 1);
  lcd.print("secs to Cancel");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Unpres to cancel");
  lcd.setCursor(0, 1);
  lcd.print("Counting: ");
}

///////////////////// WIPING DONE ///////////////////////////
void showWipingDone(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wiping Done!");
}

///////////////////// WIPING FAIL ///////////////////////////
void showWipingFail(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Wiping Cancelled");
}

//////////////////// RESET //////////////////////////////////
void showResetLCD(){
  lcd.setCursor(0, 0);
  lcd.print("Please reset");
  lcd.setCursor(0, 1);
  lcd.print("the system!");
}


////////////////// SCAN A TAG //////////////////////////////
void showScanTag(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan a Tag to ");
  lcd.setCursor(0, 1);
  lcd.print("ADD/REMOVE");
}

////////////////// NO MASTER CARD DEFINED ////////////////////
void showNoMastercard(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No Master Card ");
  lcd.setCursor(0, 1);
  lcd.print("Defined");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Scan A Tag to ");
  lcd.setCursor(0, 1);
  lcd.print("Define as Master");
}

////////////////// MASTER CARD DEFINED ////////////////////
void showMastercard(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Admin defined");
  delay(2000);
}

////////////////// ERROR ////////////////////
void showError(){
  lcd.setCursor(0, 0);
  lcd.print("FAILED!");
  lcd.setCursor(0, 1);
  lcd.print("wrong ID or EEPROM error");
}

////////////////// MANUAL MODE ////////////////////
void showManualMode(){
  lcd.clear();
  lcd.print("Manual mode");
  lcd.setCursor(0,1);
  lcd.print("Insert password");
  delay(1500);
  lcd.clear();
  lcd.print("Password:");
  lcd.setCursor(0,1);
}


////////////////// INCORRECT PASSWORD ////////////////////
void showIncorrectPass(){
  lcd.clear();
  lcd.print("   PASSWORD:    ");
  lcd.setCursor(0, 1);
  lcd.print("   INCORRECT   ");
  delay(1500);
}

////////////////// CORRECT PASSWORD //////////////////////
void showCorrectPass(){
  lcd.clear();
  lcd.print("   PASSWORD:    ");
  lcd.setCursor(0, 1);
  lcd.print("   CORRECT    ");
  delay(1500);
  lcd.clear();
  lcd.print("MANUAL MODE:");
  lcd.setCursor(0,1);
  lcd.print("<-- drag to open");
}

////////////////// MASTER EXIT //////////////////////
void showExit(){
  lcd.clear();
  lcd.print("Exiting Program Mode");
  lcd.setCursor(0, 1);
  lcd.print("--------------------");
}

////////////////// ALREADY THERE //////////////////////
void alreadyThere(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Already there");
}

//////////////////////// ADDING ////////////////////////////
void adding(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("New Tag,adding...");
}

////////////////////// ACCESS GRANTED  ///////////////////////////
void accessGranted(){
  lcd.clear();
  lcd.print("Access granted");
  lcd.setCursor(0, 1);
  lcd.print("----------------");
}

////////////////////// ACCESS DENIED  ///////////////////////////
void accessDenied(){
  lcd.clear();
  lcd.print(" Access Denied ");
  lcd.setCursor(0, 1);
  lcd.print("----------------");
}
