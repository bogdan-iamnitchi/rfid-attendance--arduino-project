
void checkCardPresent() {

  //RFID---------------------------------------------
   //look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // go to start of loop if there is no card present
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // if read card serial(0) returns 1, the uid struct contians the ID of the read card.
  }

  String CardID ="";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    CardID += mfrc522.uid.uidByte[i];
  }
  if( CardID == OldCardID ){
    return;
  }
  else{
    OldCardID = CardID;
  }

  //---------------------------------------------
  Serial.println("Card UID: " + CardID);
  sendCardID(CardID);
}