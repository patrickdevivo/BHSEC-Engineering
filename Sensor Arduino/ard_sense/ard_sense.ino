void setup()
{
  Serial.begin(9600);
  for(int i=2; i<=13; i++)
  {
    pinMode(i, INPUT);
  }
}

void loop()
{
  int n;
  int m;
  for(int p=2; p<=13; p+=2)
  {
    n = digitalRead(p);
    m = digitalRead(p+1);
    if (m==1)
    {
      m=2;
    }
    Serial.print(n+m);
    pinMode(p, OUTPUT);
    digitalWrite(p, LOW);
    digitalWrite(p+1, LOW);
    pinMode(p, INPUT);
  }
  Serial.print(".WAV!");
}

