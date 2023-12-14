// ------------------------------------SEND CARD ID TO WEB------------------------------------------------
void sendCardID(String Card_uid) {
  Serial.println("Sending the Card ID");
  if(WiFi.isConnected()) {
    HTTPClient http;    //Declare object of class HTTPClient
    //GET Data
    getData = "?card_uid=" + String(Card_uid) + "&device_token=" + String(device_token); // Add the Card ID to the GET array in order to send it
    //GET methode
    Link = URL + getData;
    http.begin(Link); //initiate HTTP request   //Specify content-type header
    
    int httpCode = http.GET();   //Send the request
    String payload = http.getString();    //Get the response payload

    Serial.println(Link);   //Print HTTP return code
    Serial.println(httpCode);   //Print HTTP return code
    Serial.println(Card_uid);     //Print Card ID
    Serial.println(payload);    //Print request response payload

    if (httpCode == 200) {
      //ATENDANCE ---------------------------------------------------------
      // when enter the building
      if (payload.substring(0, 5) == "login") {
        String user_name = payload.substring(5);
        Serial.println(user_name);

        accesGranted(user_name, "login");
      }
      // when exit the building
      else if (payload.substring(0, 6) == "logout") {
        String user_name = payload.substring(6);
        Serial.println(user_name);

        accesGranted(user_name, "logout");
      }
      // card is added to app but not register by admin
      else if(payload == "Not registerd!") {
        Serial.println("Not registerd!");
        accesDenied(payload.substring(0, payload.length() - 1));
      }
      // card not found because is new and we are in attendance mode
      else if(payload == "Not found!"){
        Serial.println("Not found!");
        accesDenied(payload.substring(0, payload.length() - 1));
      }
      //ENROLLMENT ---------------------------------------------------------
      // new card scaned -> added to the app
      else if (payload == "succesful") {
        Serial.println("New Card");

        enrollmentNewTag();
      }
      // an existing card scaned -> register/update by admin
      else if (payload == "available") {
        Serial.println("Free Card");

        enrollmentSameTag();
      }

      //Close connection
      delay(100);
      http.end();
    }
  }
}