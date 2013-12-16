void sr04GetDistance() {
  long d;
  delay(2);
  digitalWrite(2, HIGH);
  delay(4);
  digitalWrite(2, LOW);
  d = pulseIn(3, HIGH, 1000);
  if(d > 0 and d < 11640) {
    distance = d / 58.2;
  }
}
