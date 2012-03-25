void setup()
{
  Serial.begin(9600);

  pinMode(1, INPUT); 
  pinMode(2, INPUT); 
  pinMode(3, INPUT); 
  pinMode(4, INPUT); 
  pinMode(5, INPUT); 
  pinMode(6, INPUT); 
  pinMode(7, INPUT); 
  pinMode(8, INPUT); 
  pinMode(9, INPUT); 
  pinMode(10, INPUT); 
  pinMode(11, INPUT); 
  pinMode(12, INPUT);
} 

void loop()
{
  char sense_status[12];
  int count = 0;
  for(int n=1; n<13; n++)
  {
    sense_status[count]=digitalRead(n);
    count+=1;
  } 
  Serial.println(sense_status);
}
