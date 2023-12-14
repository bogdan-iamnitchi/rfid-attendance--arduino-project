// ------------------------------------CONECT TO WIFI------------------------------------------------
void connectToWiFi() {
    WiFi.mode(WIFI_OFF); //Prevents reconnection issue (taking too long to connect)

    delay(1000);

    Serial.print("Connecting to ");
    Serial.println(ssid);
    lcd.print("Connecting to:  ");
    lcd.setCursor(0, 1);
    lcd.print(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("Connected");
    
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
    
    delay(1000);
}