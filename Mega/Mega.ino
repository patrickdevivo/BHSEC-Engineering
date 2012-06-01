#include <FatReader.h>
#include <SdReader.h>
#include <avr/pgmspace.h>
#include "WaveUtil.h"
#include "WaveHC.h"


SdReader card;    // This object holds the information for the card
FatVolume vol;    // This holds the information for the partition on the card
FatReader root;   // This holds the information for the filesystem on the card
FatReader f;      // This holds the information for the file we'll play

WaveHC wave;      // This is the only wave (audio) object, since we will only play one at a time

//INIT ###############

int freeRam(void) // this handy function will return the number of bytes currently free in RAM, great for debugging!
{
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 

void sdErrorCheck(void)
{
  if (!card.errorCode()) return;
  putstring("\n\rSD I/O error: ");
  Serial.print(card.errorCode(), HEX);
  putstring(", ");
  Serial.println(card.errorData(), HEX);
  while(1);
}

int count_button=0;

void setup()
{
  Serial.begin(9600);
  for (int pins=23; pins<=45; pins+=2) {
    pinMode(pins, OUTPUT);
    digitalWrite(pins, LOW);
    pinMode(pins, INPUT);
  }

  pinMode(14, OUTPUT);
  putstring("Free RAM: ");       // This can help with debugging, running out of RAM is bad
  Serial.println(freeRam());      // if this is under 150 bytes it may spell trouble!
 
  //  if (!card.init(true)) { //play with 4 MHz spi if 8MHz isn't working for you
  if (!card.init()) {         //play with 8 MHz spi (default faster!)  
    putstring_nl("Card init. failed!");  // Something went wrong, lets print out why
    sdErrorCheck();
    while(1);                            // then 'halt' - do nothing!
  }
  
  // enable optimize read - some cards may timeout. Disable if you're having problems
  card.partialBlockRead(true);
 
// Now we will look for a FAT partition!
  uint8_t part;
  for (part = 0; part < 5; part++) {     // we have up to 5 slots to look in
    if (vol.init(card, part)) 
      break;                             // we found one, lets bail
  }
  if (part == 5) {                       // if we ended up not finding one  :(
    putstring_nl("No valid FAT partition!");
    sdErrorCheck();      // Something went wrong, lets print out why
    while(1);                            // then 'halt' - do nothing!
  }
  
  // Lets tell the user about what we found
  putstring("Using partition ");
  Serial.print(part, DEC);
  putstring(", type is FAT");
  Serial.println(vol.fatType(),DEC);     // FAT16 or FAT32?
  
  // Try to open the root directory
  if (!root.openRoot(vol)) {
    putstring_nl("Can't open root dir!"); // Something went wrong,
    while(1);                             // then 'halt' - do nothing!
  }
  
  // Whew! We are past the tough parts.
  putstring_nl("Ready!");
}

//END OF INIT ################################################################################################



int instrumentNumber=0;
int lastButton=0;
String previous;
void loop()

{
char file[12];

String message;
  for (int count=23; count<=43; count+=4)
    {
    int n=digitalRead(count);
    int m=digitalRead(count+2);
    if(m==1) m=2;
    message+=(n+m);

    pinMode(count, OUTPUT); digitalWrite(count, LOW); pinMode(count, INPUT); 
    pinMode(count+2, OUTPUT); digitalWrite(count+2, LOW); pinMode(count+2, INPUT);
    }

  //BUTTON #####
  int button = digitalRead(14);

  if (button != lastButton) instrumentNumber++;
  message += interpret();
  
  lastButton=button;
  //BUTTON END #####

  message += ".WAV";
  if(!(message.equals(previous))) {
      if(!(message.equals ("000000.WAV")||message.equals ("0000005.WAV")||message.equals ("0000006.WAV")||message.equals ("0000007.WAV")||message.equals ("0000008.WAV")||message.equals ("0000009.WAV"))) {
      message.toCharArray(file, 12);
      Serial.print("message:");Serial.print(file);Serial.print("  previous: "); Serial.println(previous);
      playfile(file);
    } 
  }
  
previous=message;
 

}



void playfile(char *name) {

  // see if the wave object is currently doing something
  if (wave.isplaying) {               // already playing something, so stop it!
    wave.stop();                  // stop it
  }

  // look in the root directory and open the file
  if (!f.open(root, name)) {
    putstring("Couldn't open file "); Serial.println(name); return;
  }

  // OK read the file and turn it into a wave object
  if (!wave.create(f)) {
    putstring_nl("Not a valid WAV"); return;
  }
  
  // ok time to play! start playback
  wave.play();
  
}
int interpret(){
  instrumentNumber %= 5;
  switch (instrumentNumber){
    //sine
    case 0:
      return 5;
    //piano
    case 1:
      return 6;
    //flute
    case 2:
      return 7;
    //trumpet
    case 3:
      return 8;
    //violen
    case 4:
      return 9;
      }
}
