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
  int sense_status[2];
  for(int n=1; n<13; n++)
  {
    if (digitalRead(n)==1)
    {
      if (n==1) 
      {
        sense_status[0]=n;
      }
      else 
      {
        sense_status[1]=n;
      }
    }
  } 
  Serial.println(sense_status[0]+sense_status[1]);


}

