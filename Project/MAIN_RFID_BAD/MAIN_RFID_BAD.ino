#include <Wire.h> //for I2C protocol
#include <LiquidCrystal_I2C.h>
#define LCD_ADDR 0x27
#define LCD_COLS 16
#define LCD_ROWS 2

#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN  53  //D53
#define RST_PIN 5  //D5

//#include <Servo.h>
//#include <Keypad.h>

#include <ESP8266_Lib.h>
#define EspSerial Serial1 //rename Serial1 as EspSerial
#define ESP8266_BAUD 115200

#define DEBUG true
#define SERIAL_BAUD 115200

// INSTANCE OBJECTS
//------------------------------------------------------------------------------
LiquidCrystal_I2C lcd(LCD_ADDR, LCD_COLS, LCD_ROWS); //adress, cols, rows
MFRC522 mfrc522(SS_PIN, RST_PIN); //SS_PIN RST_PIN
ESP8266 wifi(&EspSerial);

// PINS VARIABLES
//------------------------------------------------------------------------------
const int greenLed = 6;
const int blueLed = 7;
const int redLed = 8;

// RFID VARIABLES
//------------------------------------------------------------------------------
String OldCardID = "";

// ESP8266 VARIABLES
//------------------------------------------------------------------------------
const char SSID_ESP[] = "UPCF11B4DD";         //Give EXACT name of your WIFI
const char SSID_KEY[] = "jwdxw8fbbmvV"; 
const char* host = "smartatendpro.000webhostapp.com"; //yourdomain.com or localhost
String NOOBIX_id = "99999"; //This is the ID you have on your database, I've used 99999 becuase there is a maximum of 5 characters
String NOOBIX_password = "12345"; //Add the password from the database, also maximum 5 characters and only numerical values
String location_url = "/TX.php?id="; //location of your PHP file on the server
const char* device_token  = "Device Token";

String URL = "http://www.smartatendpro.000webhostapp.com"; //computer IP or the server domain
String URL_withPacket = ""; 

unsigned long timeout_start_val;
char scratch_data_from_ESP[20];//first byte is the length of bytes
char payload[200];
byte payload_size=0, counter=0;
char ip_address[16];

//DEFINE KEYWORDS HERE
const char keyword_OK[] = "OK";
const char keyword_Ready[] = "Ready";
const char keyword_no_change[] = "no change";
const char keyword_blank[] = "#&";
const char keyword_ip[] = "192.";
const char keyword_rn[] = "\r\n";
const char keyword_quote[] = "\"";
const char keyword_carrot[] = ">";
const char keyword_sendok[] = "SEND OK";
const char keyword_linkdisc[] = "Unlink";
String received_text = "";

const char CWMODE = '1';//CWMODE 1=STATION, 2=APMODE, 3=BOTH
const char CIPMUX = '1';//CWMODE 0=Single Connection, 1=Multiple Connections

// PROTOTYPES
//------------------------------------------------------------------------------
boolean setup_ESP();
boolean connect_ESP();
void serial_dump_ESP();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT); 
  pinMode(redLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
  pinMode(blueLed, OUTPUT);

  digitalWrite(redLed, LOW);
  digitalWrite(greenLed, LOW);
  digitalWrite(blueLed, LOW);

  lcd.init();
  lcd.backlight();
  lcd.print("mamamamama");

  SPI.begin();
  mfrc522.PCD_Init();

  EspSerial.begin(ESP8266_BAUD);//default baudrate for ESP
  Serial.begin(SERIAL_BAUD); //for status and debug
  
  lcd.print("Connecting...");
  setup_ESP();
  lcd.clear();
  lcd.print("Done!"););

}

void loop() {
  // put your main code here, to run repeatedly:

}
