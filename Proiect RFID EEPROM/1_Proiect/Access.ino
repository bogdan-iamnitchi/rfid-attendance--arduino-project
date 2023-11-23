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
  delay(500);
  digitalWrite(BuzzerPin, LOW);
}
