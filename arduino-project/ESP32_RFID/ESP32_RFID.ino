// --------------------------------LIBRARIES------------------------------------------
//ESP32 ---------------------------
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
//RFID ----------------------------
#include <SPI.h>
#include <MFRC522.h>
//LCD -----------------------------
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//SERVO ----------------------------------
#include <ESP32Servo.h>



// --------------------------------PIN DECLARATION------------------------------------------
//RFID ---------------------------
#define SS_PIN  5
#define RST_PIN 4

#define SERVO_PIN 27
#define BUZZER_PIN 26

#define GREEN_LED 25
#define YELLOW_LED 33
#define RED_LED 32

// --------------------------------INSTANCE OBJECTS------------------------------------------
MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo myservo;

// --------------------------------GLOBAL VARIABLES------------------------------------------
//ESP32 ---------------------------
const char *ssid = "UPCF11B4DD";
const char *password = "jwdxw8fbbmvV";
String URL = "http://192.168.0.108/rfidattendance/getdata.php"; //computer IP or the server domain

// const char *ssid = "iPhone - Bogdan";
// const char *password = "qwerty1234";
// String URL = "http://172.20.10.3/rfidattendance/getdata.php"; //computer IP or the server domain

const char *device_token  = "e7c067f44981320a";
String getData, Link;
String OldCardID = "";

unsigned long previousMillisTimer = 0;
unsigned long previousMillisAction = 0;

int timezone = 2 * 3600;   //2 means UTC+2 timezone
int time_dst = 0;

bool showScanTagDisplay = false;
bool actionTriggered = false;

// ------------------------------------PROTOTYPES------------------------------------------------
void connectToWiFi();
void configureTime();
void sendCardID(String Card_uid);
void checkCardPresent();


// ------------------------------------SETUP------------------------------------------------
void setup() {

  Serial.begin(115200);

  //LEDURI---------------------------------------------
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  //LCD---------------------------------------------
  lcd.begin();
  lcd.backlight();

  //SERVO---------------------------------------------
  ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(SERVO_PIN, 1000, 2000);

  //ESP32---------------------------------------------
  connectToWiFi();
  configTime(timezone, time_dst, "pool.ntp.org","time.nist.gov");
  
  //RFID---------------------------------------------
  SPI.begin();
  mfrc522.PCD_Init();
  showScanTag();
}

// ------------------------------------LOOP------------------------------------------------
void loop() {


  //ESP32---------------------------------------------
  if(!WiFi.isConnected()){
    connectToWiFi();    //Retry to connect to Wi-Fi
  }

  //TIME---------------------------------------------
  if (millis() - previousMillisTimer >= 1000) {
    configureTime();

    //LED Blink
    if(!actionTriggered) {
      digitalWrite(YELLOW_LED, !digitalRead(YELLOW_LED));
    }
  }

  //LCD---------------------------------------------
  if(showScanTagDisplay) {
    showScanTag();
    Serial.println("Display scan tag");
  }

  //RFID---------------------------------------------
  checkCardPresent();

  if(actionTriggered) {
    if (millis() - previousMillisAction >= 2000) {
      myservo.write(0);
      lcd.clear();

      OldCardID="";
      showScanTagDisplay = true;
      Serial.println("Done");
    }
  }
  
}

// ------------------------------------CONFIG TIME------------------------------------------------
void configureTime() {
  previousMillisTimer = millis();
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  Serial.println(p_tm);
  Serial.println(p_tm);

}
