void setup()
{
  Serial.begin(9600);
  for(int i=2; i<=13; i++)
  {
    pintMode(i, INPUT);
  }
}

void loop()
{
  for(int p=2; p<=13; p++)
  {
    Serial.print(digitalRead(p));
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    pinMode(p, INPUT):
  }
  Serial.println(".WAV!");
}