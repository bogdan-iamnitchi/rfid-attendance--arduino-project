//Acest fisier contine: includerea bibliotecilor, definirea unor constante / variabile 
//pe care le voi folosi in implementare și constructori pentru diferite obiecte

#include <EEPROM.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <SPI.h>
#include <Wire.h>
#include <Keypad.h>

//-------------------------PINI------------------------------

const uint8_t greenLed = 6;
const uint8_t blueLed = 7;
const uint8_t redLed = 8;
const uint8_t ServoPin = 3;
const uint8_t BuzzerPin = 11;
const uint8_t wipeB = 10;
const uint8_t joy = A15; 

//-----------------------VARIABILE---------------------------

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


//--KEYPAD

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


//---------------------CONSTRUCTORI----------------------------
MFRC522 mfrc522(53, 5); // ss_pin rst_in
LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
Servo myServo;
