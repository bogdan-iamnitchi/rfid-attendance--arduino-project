#define DEBUG true


void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT); 
  digitalWrite(LED_BUILTIN, LOW);
  sendData("AT+RST\r\n", 2000, false); // resetare modul
  sendData("AT+CWMODE=2\r\n", 1000, false); // configurare ca access point
  sendData("AT+CIFSR\r\n", 1000, DEBUG); // citeste adresa IP
  sendData("AT+CWSAP?\r\n", 2000, DEBUG); // citeste informatia SSID (nume retea)
  sendData("AT+CIPMUX=1\r\n", 1000, false); // configurare conexiuni multiple
  sendData("AT+CIPSERVER=1,80\r\n", 1000, false); // pornire server pe port 80
}
void loop() {
  if (Serial1.available()) {
    if (Serial1.find("+IPD,")) {
      delay(500);
      int connectionId = Serial1.read() - 48; // functia read() returneaza valori zecimale ASCII
      // si caracterul ‘0’ are codul ASCII 48
      String webpage = "<h1>Hello World!</h1><a href=\"/l0\"><button>ON</button></a>";
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      webpage += "<a href=\"/l1\"><button>OFF</button></a>";

      if (readSensor() > 0) {
        webpage += "<h2>Millis:</h2>";
        webpage += readSensor();
      }
      cipSend += webpage.length();
      cipSend += "\r\n";
      sendData(cipSend, 100, DEBUG);
      sendData(webpage, 150, DEBUG);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId; //se adauga identificatorul conexiunii
      closeCommand += "\r\n";
      sendData(closeCommand, 300, DEBUG);
    }
  }
}

String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  Serial1.print(command); // trimite comanda la esp8266
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (Serial1.available()) {
      char c = Serial1.read(); // citeste caracter urmator
      response += c;
    }
  }
  if (response.indexOf("/l0") != -1) {
    digitalWrite(LED_BUILTIN, HIGH);
  }
  if (response.indexOf("/l1") != -1) {
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (debug) {
    Serial.print(response);
  }
  return response;
}

unsigned long readSensor() {
  return millis();
} 