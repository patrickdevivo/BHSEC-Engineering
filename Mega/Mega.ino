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

int count=0;

void setup()
{
  Serial.begin(9600);
  for (int pins=23; pins<=45; pins+=2) {
    pinMode(pins, INPUT);   
  }

  pinMode(21, OUTPUT);
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

//END OF INIT ###############

void loop()
{

  String message;
  char file[12];

  for (int count=23; count<=43; count+=4)
    {
    int n=digitalRead(count);
    int m=digitalRead(count+2);
    if(m==1) m=2;
    message+=(n+m);                   //math then string concatenation

    //do these lines reset both pins or smthng?
    pinMode(count, OUTPUT); digitalWrite(count, LOW); pinMode(count, INPUT); 
    pinMode(count+2, OUTPUT); digitalWrite(count+2, LOW); pinMode(count+2, INPUT);
    }

  //BUTTON #####
  int button= digitalRead (21); 
  int stat;

  /*
  I am confused about this entire section. The switch block using count is after the for loop where count exists.
  Or at least, that's true if I'm correct about for loop variables expiring after use.
  Furthermore, stat has only just been initialized; it will always execute at least once unless button is null.
  If this is the desired effect, why not just use a do-while statement?
  */

  if (button != stat) 
  {
    switch (count) {
      case 0:
        message += 'p';
        break;
      case 1:
        message += 's';
        break;
      case 2:
        message += 'f';
        break;
      default:
        break;
    }

    if (count !=2)      //this doesn't make any sense. Why are you incrementing count? This isn't a loop,
                        //it's an if statement. And why does this only happen if it's not a flute?
    count++; 
    else
    {
     count=0;           //why set count to zero? Aren't we exiting the if statement? This is still a part of the
                        //larger if statement, yet its action with count has nothing to do with the initial logic
                        //of said if statment.
    }
  }

  button=stat;          //I don't understand why we have a stat variable or what it is, but I think you just set
                        //button to null.
  //BUTTON END #####

  message += ".WAV";
  message.toCharArray(file, 12);
  if(file == "000000p.WAV"||file == "000000s.WAV"||file == "000000f.WAV") return;
  Serial.println(file);
  //playfile(file);     //is this supposed to be commented?
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
