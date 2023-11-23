#include <EEPROM.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Keypad.h>


MFRC522 mfrc522(53, 5); // ss_pin rst_in
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myServo;


uint8_t greenLed = 6;
uint8_t blueLed = 7;
uint8_t redLed = 8;
uint8_t ServoPin = 33;
constexpr uint8_t BuzzerPin = 11;
constexpr uint8_t wipeB = 10;
constexpr uint8_t joy = A15; 

int password = 0;
int passLenght = 0;
int scanari[300] = {0};


boolean match = false;
boolean programMode = false;
boolean replaceMaster = false;

uint8_t successRead;
byte storedCard[4];
byte readCard[4];
byte masterCard[4];

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {24, 25, 26, 27}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {28, 29, 30, 31}; //connect to the column pinouts of the keypad

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);


///////////////////////////////////////// Setup ///////////////////////////////////
void setup() {
  Serial.begin(9600);
  //Arduino Pin Configuration
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(wipeB, INPUT_PULLUP); 

  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
  
  lcd.init();
  lcd.backlight();
  
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(ServoPin);
  myServo.write(90);

  ShowReaderDetails();
  
  if (EEPROM.read(1) != 143) {
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
    do {
      successRead = getID();
      digitalWrite(blueLed, HIGH);
      digitalWrite(BuzzerPin, HIGH);
      delay(200);
      digitalWrite(BuzzerPin, LOW);
      digitalWrite(blueLed, LOW);
      delay(200);
    }
    while (!successRead);
    for ( uint8_t j = 0; j < 4; j++ ) {
      EEPROM.write( 2 + j, readCard[j] );
    }
    EEPROM.write(1, 143);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Master Defined");
    delay(2000);
  }
  
  for ( uint8_t i = 0; i < 4; i++ ) {
    masterCard[i] = EEPROM.read(2 + i);
  }
  
  int k = 0, x=masterCard[0];
  password = x;
  while(x)
  {
    passLenght++;
    x /= 10;
  }
  x = masterCard[1];
  while(x)
  {
    k++;
    passLenght++;
    x /= 10;
  }
  while(k)
  {
    password = password *10;
    k--;
  }
  password += masterCard[1];
  
  ShowOnLCD();
  cycleLeds();
}

///////////////////////////////////////// Main Loop ///////////////////////////////////
void loop () {
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
      if(customKey == 'A')
      {
        lcd.clear();
        lcd.print("Manual mode");
        lcd.setCursor(0,1);
        lcd.print("Insert password");
        delay(1500);
        lcd.clear();
        lcd.print("Password:");
        lcd.setCursor(0,1);
        manualMode = 1;
      }
      if(customKey == 'B')
      {
        manualMode = 0;
      }

      if(manualMode == -2)
      {
        myServo.write(val);
      }
      
      if(manualMode == -1){
        cycleLeds();
      }

      if(manualMode == 0)
      {
        lcd.clear();
        lcd.print("Scan a Tag to ");
        lcd.setCursor(0, 1);
        lcd.print("ADD/REMOVE");
        manualMode = -1;
      }
      
      if(manualMode == 1)
      {
        if(customKey >= '0' && customKey <='9')
        {
          rez = rez*10 + (customKey - 48);
          lcd.print(customKey);
          cnt++;
        }
        if(cnt == passLenght)
        {
          if(rez == password)
          {
            manualMode = 3;
            rez = 0;
            cnt = 0;
          }
          else
          {
            manualMode = 2;
            rez = 0;
            cnt = 0;
          }  
        }
      }
 
      if(manualMode == 2)
      {
        lcd.clear();
        lcd.print("   PASSWORD:    ");
        lcd.setCursor(0, 1);
        lcd.print("   INCORRECT   ");
        delay(1500);
        manualMode = 0;
      }

      if(manualMode == 3)
      {
        lcd.clear();
        lcd.print("   PASSWORD:    ");
        lcd.setCursor(0, 1);
        lcd.print("   CORRECT    ");
        delay(1500);

        lcd.clear();
        lcd.print("MANUAL MODE:");
        lcd.setCursor(0,1);
        lcd.print("<-- drag to open");
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
      lcd.clear();
      lcd.print("Exiting Program Mode");
      lcd.setCursor(0, 1);
      lcd.print("--------------------");
      digitalWrite(BuzzerPin, HIGH);
      delay(1000);
      digitalWrite(BuzzerPin, LOW);
      ShowOnLCD();
      programMode = false;
      return;
    }
    else {
      if ( findID(readCard) ) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Already there");
        deleteID(readCard);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tag to ADD/REM");
        lcd.setCursor(0, 1);
        lcd.print("Master to Exit");
      }
      else {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("New Tag,adding...");
        writeID(readCard);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Scan to ADD/REM");
        lcd.setCursor(0, 1);
        lcd.print("Master to Exit");
      }
    }
  }
  else {
    if ( isMaster(readCard)) {
      programMode = true;
      Serial.println("------------------------------------------------");
      for(int i=0; i<256; i++)
        {
          if(scanari[i]){
           
           
            Serial.print(" - a intrat in cladire de ");
            if(scanari[i] % 2){
              Serial.print(scanari[i]/2 + 1); Serial.println(" ori");
            }
            else if(scanari[i] % 2 == 0){
              Serial.print(scanari[i]/2); Serial.println(" ori");
            }
            Serial.print("- a parasit cladirea de ");
            Serial.print(scanari[i]/2); Serial.println(" ori");
            
            Serial.print("Angajatul cu codul : ");
            Serial.println(i);
          }
        }
        Serial.println("------------------------------------------------");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PROGRAM MODE:");
      uint8_t count = EEPROM.read(0);
      lcd.setCursor(0, 1);
      lcd.print("I have ");
      lcd.print(count);
      lcd.print(" records");
      digitalWrite(BuzzerPin, HIGH);
      delay(2000);
      digitalWrite(BuzzerPin, LOW);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scan a Tag to ");
      lcd.setCursor(0, 1);
      lcd.print("ADD/REMOVE");
    }
    else {
      if ( findID(readCard) ) {
        lcd.clear();
        lcd.print("Access granted");
        lcd.setCursor(0, 1);
        lcd.print("----------------");
        
        scanari[readCard[0]]++;
        Serial.print("Angajatul cu codul : ");
        Serial.print(readCard[0]);
        if(scanari[readCard[0]] % 2 == 0)
          Serial.println(" a parasit cladirea");
        else
          Serial.println(" a intrat in cladire");
        
        granted();
        myServo.write(90);
        ShowOnLCD();
      }
      else {
        lcd.clear();
        lcd.print(" Access Denied ");
        lcd.setCursor(0, 1);
        lcd.print("----------------");

        Serial.print("CARD NECUNOSCUT: ");
        Serial.print(readCard[0], HEX);
        Serial.print(":");
        Serial.print(readCard[1], HEX);
        Serial.print(":");
        Serial.print(readCard[2], HEX);
        Serial.print(":");
        Serial.println(readCard[3], HEX);

        denied();
        ShowOnLCD();
      }
    }
  }
}

void checkButton()
{
  if (digitalRead(wipeB) == HIGH) {
    digitalWrite(redLed, HIGH);
    lcd.clear(); 
    lcd.setCursor(0, 0);
    lcd.print("Button Pressed");
    digitalWrite(BuzzerPin, HIGH);
    delay(1000);
    digitalWrite(BuzzerPin, LOW);
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
    bool buttonState = monitorWipeButton(5000);
    if (buttonState == true && digitalRead(wipeB) == LOW) {
      lcd.print("Wiping EEPROM...");
      for (uint16_t x = 0; x < EEPROM.length(); x = x + 1) {
        if (EEPROM.read(x) == 0) {
     
        }
        else {
          EEPROM.write(x, 0);
        }
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wiping Done!");

      digitalWrite(redLed, LOW);
      digitalWrite(BuzzerPin, HIGH);
      delay(200);
      digitalWrite(redLed, HIGH);
      digitalWrite(BuzzerPin, LOW);
      delay(200);
      digitalWrite(redLed, LOW);
      digitalWrite(BuzzerPin, HIGH);
      delay(200);
      digitalWrite(redLed, HIGH);
      digitalWrite(BuzzerPin, LOW);
      delay(200);
      digitalWrite(redLed, HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Please reset");
      lcd.setCursor(0, 1);
      lcd.print("the system!");
      while(1);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wiping Cancelled");
      digitalWrite(redLed, LOW);
      delay(1500);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Scan a Tag to ");
      lcd.setCursor(0, 1);
      lcd.print("ADD/REMOVE");
    }
  }
}

/////////////////////////////////////////  Access Granted    ///////////////////////////////////
void granted () {
  digitalWrite(blueLed, LOW);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, HIGH);
  myServo.write(180);
  delay(1000);
}
///////////////////////////////////////// Access Denied  ///////////////////////////////////
void denied() {
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);
  digitalWrite(redLed, HIGH);
  digitalWrite(BuzzerPin, HIGH);
  delay(1000);
  digitalWrite(BuzzerPin, LOW);
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
///////////////////////////////////////// Cycle Leds (Program Mode) ///////////////////////////////////
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
    lcd.setCursor(0, 0);
    lcd.print("FAILED!");
    lcd.setCursor(0, 1);
    lcd.print("wrong ID or bad EEPROM");
    delay(2000);
  }
}
///////////////////////////////////////// Remove ID from EEPROM   ///////////////////////////////////
void deleteID( byte a[] ) {
  if ( !findID( a ) ) {
    BlinkLEDS(redLed);
    lcd.setCursor(0, 0);
    lcd.print("FAILED!");
    lcd.setCursor(0, 1);
    lcd.print("wrong ID or bad EEPROM");
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
///////////////////////////////////////// Check Bytes   ///////////////////////////////////
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
////////////////////// Check readCard IF is masterCard   ///////////////////////////////////
boolean isMaster( byte test[] ) {
  if ( checkTwo( test, masterCard ) )
    return true;
  else
    return false;
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
////////////////////// Print Info on LCD   ///////////////////////////////////
void ShowOnLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" ACCESS CONTROL");
  lcd.setCursor(0, 1);
  lcd.print("   Scan a Tag");
}
