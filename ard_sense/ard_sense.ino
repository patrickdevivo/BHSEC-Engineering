void setup()
{
  //Start Serial
  Serial.begin(9600);
  //Establish sensor-pins 1-12
  for(int p=1; p<13; p++)
  {
    pinMode(p, INPUT);
  }
  
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
