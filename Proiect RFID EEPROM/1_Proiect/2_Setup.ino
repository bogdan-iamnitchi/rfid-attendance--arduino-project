void setup() {
  Serial.begin(9600);
  
  // declarare pini
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);
  pinMode(wipeB, INPUT); 

  // initializare leduri si buton
  digitalWrite(wipeB, HIGH);
  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);

  // initializare LCD
  lcd.begin();
  lcd.backlight();

  // initializare RFIS si ServoMotor
  SPI.begin();
  mfrc522.PCD_Init();
  myServo.attach(ServoPin);
  myServo.write(90);
  ShowReaderDetails();

  // verificam daca este un master card definit, 
  // iar daca nu este asteptam sa se defineasca unul
  if (EEPROM.read(1) != 143) {
    defineMaster();
  }

  // luam din memorie id-ul master cardului
  for ( uint8_t i = 0; i < 4; i++ ) {
    masterCard[i] = EEPROM.read(2 + i);
  }

  // apelam metoda care determina lungimea parolei
  determinePass();
  
  showAccessLCD();
  cycleLeds();
}
