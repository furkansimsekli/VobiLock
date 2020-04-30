/* 
*  
*        Hedwig's theme - Harry Potter 
*        Connect a piezo buzzer or speaker to pin 11 or select a new pin.
*        More songs available at https://github.com/robsoncouto/arduino-songs                                            
*
                                              
                                                    Robson Couto, 2019

* 
*        Edited by iamvobi for VobiLock project
* 
*/


///// NOTES /////   
#define NOTE_D4  294
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_C5  523
#define NOTE_D5  587
#define REST 0

int tempo = 250;                                                //If you increase it tune will be faster

int melody[] = {
  
  REST, 2, NOTE_D4, 4,
  NOTE_G4, -4, NOTE_AS4, 8, NOTE_A4, 4,
  NOTE_G4, 2, NOTE_D5, 4,
  NOTE_C5, -2,

};

int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0, noteDuration = 0;

const int speaker = 13;


void WelcomeTune(){
  
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    
    divider = melody[thisNote + 1];
    if (divider > 0) {
      
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; 
    }

    tone(speaker, melody[thisNote], noteDuration*0.9);

    delay(noteDuration);
    
    noTone(speaker);
  }
}
