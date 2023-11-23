//This function sends the data to the EspSerial until timeout
String sendData(String command, const int timeout, boolean debug) {
  String response = "";
  EspSerial.print(command); // trimite comanda la esp8266
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (EspSerial.available()) {
      char c = EspSerial.read(); // citeste caracter urmator
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