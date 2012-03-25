void setup()
{
  //Start Serial
  Serial.begin(9600);
  //establish sensor-pins
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
//Start Loop
void loop()
{
  //Make a new empty string of 12 characters
  char sense_status[12];
  //Start a counter
  int count = 0;
  //For loop for reading sensors
  for(int n=1; n<13; n++)
  {
    //Reading sensors and putting value in the string in the place corresponding to sensor #
    sense_status[count]=digitalRead(n);
    count+=1;
  } 
  //Print string in serial prompt to communicate with other Arduino
  Serial.println(int(sense_status));
}
