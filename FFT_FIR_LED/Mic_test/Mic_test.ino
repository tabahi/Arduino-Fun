const int mic = A0;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  Serial.println(analogRead(mic), DEC);
  delay(100);
}
