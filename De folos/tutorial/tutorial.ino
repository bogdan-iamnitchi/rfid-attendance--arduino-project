//*******************************libraries********************************
// RFID ---------------------------
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266_Lib.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Arduino Mega 2560 & ESP8266 ---
//#include <SoftwareSerial.h>
//SoftwareSerial esp8266(10, 11); // RX, TX for ESP8266
#define esp8266 Serial1
ESP8266 wifi(&esp8266);
//************************************************************************
#define SS_PIN 53  // Use any digital pin for SS_PIN
#define RST_PIN 5 // Use any digital pin for RST_PIN
//************************************************************************
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.
LiquidCrystal_I2C lcd(0x27, 16, 2);
//************************************************************************
const char* ssid = "UPCF11B4DD";
const char* password = "jwdxw8fbbmvV";
const char* device_token = "2abde03677c51120";
//************************************************************************
String URL = "http://YourComputerIP/rfidattendance/getdata.php"; // Computer IP or server domain
String getData, Link;
String OldCardID = "";
unsigned long previousMillis = 0;
//************************************************************************
void setup() {
  delay(1000);
  Serial.begin(115200);
  esp8266.begin(115200); // Initialize communication with the ESP8266
  SPI.begin();          // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522 card
  //---------------------------------------------
  connectToWiFi();
}
//************************************************************************
void loop() {
  // Check if there's a connection to Wi-Fi or not
  //---------------------------------------------
  if (millis() - previousMillis >= 15000) {
    previousMillis = millis();
    OldCardID = "";
  }
  delay(50);
  //---------------------------------------------
  // Look for a new card
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return; // Go to the start of the loop if there is no card present
  }
  // Select one of the cards
  if (!mfrc522.PICC_ReadCardSerial()) {
    return; // If ReadCardSerial(0) returns 1, the UID struct contains the ID of the read card.
  }
  String CardID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    CardID += mfrc522.uid.uidByte[i];
  }
  //---------------------------------------------
  if (CardID == OldCardID) {
    return;
  } else {
    OldCardID = CardID;
  }
  //---------------------------------------------
  SendCardID(CardID);
  delay(1000);
}

// ************send the Card UID to the website*************
void SendCardID(String Card_uid) {
  Serial.println("Sending the Card ID");
    esp8266.println("AT+CIPSTART=\"TCP\",\"" + URL + "\",80");
    if (esp8266.find("OK")) {
      String getStr = "GET " + getData + " HTTP/1.0\r\n" +
                      "Host: " + URL + "\r\n" +
                      "Connection: close\r\n\r\n";
      esp8266.print("AT+CIPSEND=");
      esp8266.println(getStr.length());
      if (esp8266.find(">")) {
        esp8266.print(getStr);
        if (esp8266.find("SEND OK")) {
          Serial.println("Request sent.");
          while (esp8266.available()) {
            String response = esp8266.readStringUntil('\r');
            Serial.print(response);
          }
          esp8266.println("AT+CIPCLOSE");
        }
      }
    }
}

// ********************connect to the WiFi******************
void connectToWiFi() {
  esp8266.println("AT+RST"); // Reset ESP8266 module
  delay(1000);
  esp8266.println("AT+CWMODE=1"); // Set Wi-Fi mode to station
  delay(1000);
  esp8266.print("AT+CWJAP=\"");
  esp8266.print(ssid);
  esp8266.print("\",\"");
  esp8266.print(password);
  esp8266.println("\""); // Connect to Wi-Fi network

  while (!esp8266.find("OK")) {
    delay(1000);
  }

  Serial.println("Connected to Wi-Fi");
}
