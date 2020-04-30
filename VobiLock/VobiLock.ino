/*  Created by iamvobi
 *  15.04.2020
 *  "VobiLock"
 *  simseklifurkan0@gmail.com
*/


//Libraries
#include <EEPROM.h>
#include "Theme.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

//Lcd Display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//FingerPrint Sensor
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

//1x4 Keypad Definitions
#define key1 10 //num1
#define key2 9  //num2
#define key3 12 //num3
#define key4 11 //num4

//Admin Interface Variables//
String admin_password = "14231";                  //Original admin password       ***** I set the default password
String getPass = "";                              //to read from EEPROM           ****  to EEPROM in a blank sketch
String newPass = "";                              //to set a new password         ***   When we run factory reset password will 
                                                  //for password loop             **    everything will be erased and go back to
                                                  //for new password              *     default..
int menu = 1;

boolean logged = false;
boolean chsID = false;
boolean setupS = false;
boolean firstFinger = false;

const int door = 8;

int delete_id = 1;
int id = 1;


void setup(void){

  if(EEPROM.read(5) == 0)
    setupS = false;
  else
    setupS = true;
   
  lcdSetup();
  startSystem();

  pinMode(key1, INPUT_PULLUP);
  pinMode(key2, INPUT_PULLUP);
  pinMode(key3, INPUT_PULLUP);
  pinMode(key4, INPUT_PULLUP);
  pinMode(door, OUTPUT);
  pinMode(speaker, OUTPUT);

  digitalWrite(door,HIGH);                              //When you set HIGH, relay doesn't allow to pass
                                                        //Otherwise when you set LOW, relay allows to pass and door is unlocked
}

void loop(){
  
  if(setupS){
    getFingerprintIDez();
  }
  else{
    setupSystem();
  }
}

void setupSystem(){

  id = 1;

  lcd.clear();
  lcd.print("Welcome to Setup");
  delay(1300);
  
  lcd.clear();
  lcd.print("Please scan");
  lcd.setCursor(0,1);
  lcd.print("for ADMIN");
  delay(1600);

  while(!firstFinger){
    getFingerprintEnroll();
  }
  
  lcd.clear();
  lcd.print("Everything looks");
  lcd.setCursor(0,1);
  lcd.print("fine!");
  delay(1500);

  lcd.clear();
  lcd.print("Admin password: ");
  lcd.setCursor(0,1);
  lcd.print(admin_password);
  delay(8000);
  
  lcd.clear();
  lcd.print("READY!");
  delay(1500);
  
  setupS = true;
  EEPROM.update(5,1);
  setup();
  loop();
    
}

void startSystem(){

  Serial.begin(9600);
  finger.begin(57600);
  delay(5);
  
  if (finger.verifyPassword()) {
    lcd.clear();
    lcd.print("READY!");
    delay(1000);
    
    displayLockedScreen();
  }
  else{
    lcd.clear();
    lcd.println("Sensor Error");
    
    while (1){
      delay(200); 
    }
  }
  getPassword();
  
}

void lcdSetup(){
  
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  lcd.print("Loading");
  delay(250);
  lcd.clear();
  lcd.print("Loading.");
  delay(250);
  lcd.clear();
  lcd.print("Loading..");
  delay(250);
  lcd.clear();
  lcd.print("Loading...");
  delay(450);
  lcd.clear();

}

void displayLockedScreen(){
  
  lcd.clear();
  lcd.setCursor(5,0);
  lcd.print("LOCKED");
  
}

void displayUnlockedScreen(){
  
  lcd.clear();
  lcd.print("Door closing in");
  lcd.setCursor(8,1);
  lcd.print("sec");
  
  for(int i = 5; i > 0; i--){
    lcd.setCursor(6,1);
    lcd.print(i);
    delay(1000);
  }

  displayLockedScreen();
}

int getFingerprintID(){
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  controlID();
  return finger.fingerID;
}

int getFingerprintIDez(){
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  controlID();
  
  return finger.fingerID; 
}

void controlID(){                            //I'll use this function to know the fingerprint belongs to whom
  
  if(finger.fingerID == 1){
    digitalWrite(door,LOW
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ADMIN");
    lcd.setCursor(7,1);
    lcd.print("INTERFACE");
    delay(700);
    digitalWrite(door,HIGH);
    
    delay(900);

    Admin_Interface();
  }
  else{
    digitalWrite(door,LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("WELCOME!");
    digitalWrite(door,HIGH);
    delay(700);
    WelcomeTune();
    
    delay(1900);
  }

  displayUnlockedScreen();
}


/////////////////////////////////////////
//////////////Admin UI///////////////////
/////////////////////////////////////////


void Admin_Interface(){
  
  for(int i = 0; i < 5;){
    int key1S = digitalRead(key1);
    int key2S = digitalRead(key2);
    int key3S = digitalRead(key3);
    int key4S = digitalRead(key4);
    
    if(!key1S){
      getPass = getPass + "1";
      
      i++;
    }
    if(!key2S){
      getPass = getPass + "2";
  
      i++;
    }
    if(!key3S){      
      getPass = getPass + "3";
  
      i++;
    }
    if(!key4S){
      getPass = getPass + "4";
  
      i++;
    }          
    
    lcd.clear();
    lcd.print("Password: ");
    lcd.setCursor(11,0);
    lcd.print(getPass);
    delay(300);
    
  }
  
  checkPass();
}


void checkPass(){
  
  if(getPass == admin_password){
    lcd.clear();
    lcd.print("You're in, sir!");
    logged = true;
    getPass = "";
    delay(2000);
    Admin_Menu();
  }
  else{
    lcd.clear();
    lcd.print("Try again later");
    getPass = "";
    delay(1500);
  }
  
}

void Admin_Menu(){

  lcd.clear();                                         // It's a reminder to control menu
  lcd.print("1 --> DOWN");
  lcd.setCursor(0,1);
  lcd.print("2 --> UP");
  delay(1400);
  lcd.clear();
  lcd.print("2 --> UP");
  lcd.setCursor(0,1);
  lcd.print("3 --> SELECT");
  delay(1400);

  updateMenu();
  Menu();
  
}

void Menu(){
  
  while(logged){
    int key1S = digitalRead(key1);
    int key2S = digitalRead(key2);
    int key3S = digitalRead(key3);
    int key4S = digitalRead(key4);
    
    if (!key1S){
      menu++;
      updateMenu();
      delay(250);
    }
    if (!key2S){
      menu--;
      updateMenu();
      delay(250);
    }
    if (!key3S){
      executeAction();
      updateMenu();
      delay(250);
    }
  }
  
}

void updateMenu(){                                  // I'll use this one for displaying the Menu screen on LCD

  switch (menu){
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Enroll");
      lcd.setCursor(0, 1);
      lcd.print(" Delete");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Enroll");
      lcd.setCursor(0, 1);
      lcd.print(">Delete");
      break;
    case 3:
      lcd.clear();
      lcd.print(">Change Password");
      lcd.setCursor(0, 1);
      lcd.print(" Factory Reset");
      break;
    case 4:
      lcd.clear();
      lcd.print(" Change Password");
      lcd.setCursor(0, 1);
      lcd.print(">Factory Reset");
      break;  
    case 5:
      lcd.clear();
      lcd.print(" Factory Reset");
      lcd.setCursor(0, 1);
      lcd.print(">Quit");
      break;  
    case 6:
      menu = 5;
      break;
  
  }
}

void executeAction(){ 
  
  switch (menu) {
    case 1:
      enroll_Page();
      break;
    case 2:
      delete_Page();
      break;
    case 3:
      changePass_Page();
      break;
    case 4:
      reset_Page();
    case 5:
      quit_Page();
      displayLockedScreen();
      break;
  }
  
}

void enroll_Page(){

  chsID = true;
  delay(500);                                      //There is a delay to prevent missclick
  
  while(chsID){
    int key1S = digitalRead(key1);
    int key2S = digitalRead(key2);
    int key3S = digitalRead(key3);
    int key4S = digitalRead(key4);
    
    if (!key1S){
      id++;
      delay(150);
    }
    if (!key2S){
      id--;
      delay(150);
    }
    if (!key3S){
      chsID = false;
      lcd.clear();
      lcd.print("Enrolling ID #");
      lcd.setCursor(15,0);
      lcd.print(id);
      delay(750);
    
      getFingerprintEnroll();
    }
    if (!key4S){
      id += 10;
      delay(150);
    }
    if(id < 1){
      id = 1;
    }
    if(id > 127){
      id = 127;
    }
    
    lcd.clear();  
    lcd.print("Choose ID #");
    lcd.setCursor(11,0);
    lcd.print(id);
    if(id == 1){
      lcd.setCursor(10,1);
      lcd.print("ADMIN");
    }
    delay(300);
  }
    
}

void delete_Page(){

  chsID = true;
  deleteFinger();

}

void changePass_Page(){

  delay(500);
  
  for(int i = 0; i < 5;){
    int key1S = digitalRead(key1);
    int key2S = digitalRead(key2);
    int key3S = digitalRead(key3);
    int key4S = digitalRead(key4);
    
    if(!key1S){
      newPass = newPass + "1";
      i++;
    }
    if(!key2S){
      newPass = newPass + "2";
      i++;
    }
    if(!key3S){      
      newPass = newPass + "3";
      i++;
    }
    if(!key4S){
      newPass = newPass + "4";
      i++;
    }          

    lcd.clear();
    lcd.print("New Password: ");
    lcd.setCursor(0,1);
    lcd.print(newPass);
    delay(300);
    
  }

  for(int i=0 ; i < newPass.length() ; i++){
    EEPROM.update( i,newPass[i] );
  }
  
  admin_password = newPass;
  newPass = "";
  
  lcd.clear();
  lcd.print("SAVED!");
  delay(1500);
  
}

void about_Page(){
  
  lcd.clear();
  lcd.setCursor(4,0);
  lcd.print("VobiLock");
  delay(2000);
  lcd.clear();
  lcd.setCursor(8,1);
  lcd.print("@iamvobi");
  delay(1800);
  
}

void reset_Page(){

  lcd.clear();
  lcd.print("Are you sure?");
  lcd.setCursor(0,1);    
  lcd.print("You can't undo");
  delay(5000);
  
  lcd.clear();
  lcd.print("1 to cancel");
  lcd.setCursor(0,1);
  lcd.print("4 to continue");
  delay(150);
  
  for(int i = 0; i < 2;){
    int key1S = digitalRead(key1);
    int key2S = digitalRead(key2);
    int key3S = digitalRead(key3);
    int key4S = digitalRead(key4);
    
    if(!key1S){
      lcd.clear();
      lcd.print("CANCELLED!");
      delay(1400);
      i++;
      updateMenu();
      Menu();
    }
    if(!key4S){
      lcd.clear();
      lcd.print("Resetting will");
      lcd.setCursor(0,1);
      lcd.print("start soon");
      delay(1400);
      i++;
      resetAll();
    }
  }
    
}

void quit_Page(){

  logged = false;
  menu = 1;
  lcd.clear();
  lcd.print("Logged Out");
  delay(1500);
}


/////////////////////////////////////////
////////////////Enroll///////////////////
/////////////////////////////////////////

uint8_t getFingerprintEnroll() {
  
  int p = -1;
  
  lcd.clear();
  lcd.print("Waiting as #");
  lcd.setCursor(12,0);
  lcd.print(id);
  delay(1500);
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image taken");
      delay(600);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print(".");
      delay(300);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Communication E");
      delay(600);
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Imaging E");
      delay(600);
      break;
    default:
      lcd.clear();
      lcd.print("Unknown E");
      delay(600);
      break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(600);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image too messy");
      delay(600);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Communication E");
      delay(600);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("No Fingerprint");
      delay(600);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("No Fingerprint");
      delay(600);
      return p;
    default:
      lcd.clear();
      lcd.print("Unknown E");
      delay(600);
      return p;
  }
  
  lcd.clear();
  lcd.print("Remove finger");
  delay(2000);
  
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  
  lcd.clear();
  lcd.print("ID ");
  lcd.setCursor(4,0);
  lcd.print(id);
  delay(750);
  
  p = -1;
  
  lcd.clear();
  lcd.print("Place again");
  delay(750);
  
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image taken");
      delay(600);
      break;
    case FINGERPRINT_NOFINGER:
      lcd.clear();
      lcd.print(".");
      delay(300);
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Communication E");
      delay(600);
      break;
    case FINGERPRINT_IMAGEFAIL:
      lcd.clear();
      lcd.print("Imaging E");
      delay(600);
      break;
    default:
      lcd.clear();
      lcd.print("Unknown E");
      delay(600);
      break;
    }
  }

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      lcd.clear();
      lcd.print("Image converted");
      delay(600);
      break;
    case FINGERPRINT_IMAGEMESS:
      lcd.clear();
      lcd.print("Image too messy");
      delay(600);
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      lcd.clear();
      lcd.print("Communication E");
      delay(600);
      return p;
    case FINGERPRINT_FEATUREFAIL:
      lcd.clear();
      lcd.print("No Fingerprint");
      delay(600);
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      lcd.clear();
      lcd.print("No Fingerprint");
      delay(600);
      return p;
    default:
      lcd.clear();
      lcd.print("Unknown E");
      delay(600);
      return p;
  }
  
  lcd.clear();
  lcd.print("Creating for  #");
  lcd.setCursor(15,0);
  lcd.println(id);
  delay(1500);
  
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Prints matched!");
    delay(600);
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("Communication E");
    delay(600);
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    lcd.clear();
    lcd.print("Did not match");
    delay(600);
    return p;
  } else {
    lcd.clear();
    lcd.print("Unknown E");
    delay(600);
    return p;
  }   

  lcd.clear();
  lcd.print("ID ");
  lcd.setCursor(4,0);
  lcd.print(id);
  delay(1500);
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Stored!");
    delay(750);
    id = 1;
    firstFinger = true;
    finished();
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();
    lcd.print("Communication E");
    delay(600);
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();
    lcd.print("Store E");
    delay(600);
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();
    lcd.print("Flash E");
    delay(600);
    return p;
  } else {
    lcd.clear();
    lcd.print("Unknown E");
    delay(600);
    return p;
  }   
}

/////////////////////////////////////////
////////////////Delete///////////////////
/////////////////////////////////////////

uint8_t deleteFingerprint(uint8_t delete_id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(delete_id);

  if (p == FINGERPRINT_OK) {
    lcd.clear();
    lcd.print("Deleted!");
    delay(600);
    finished();
    chsID = false;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    lcd.clear();    
    lcd.print("Communication E");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    lcd.clear();    
    lcd.print("Location E");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    lcd.clear();    
    lcd.print("Flash E");
    return p;
  } else {
    lcd.clear();    
    lcd.print("Unknown E"); 
    return p;
  }
     
}

void deleteFinger(){
  
  delay(500);                                        //There is a delay to prevent missclick
  
  while(chsID){
    int key1S = digitalRead(key1);
    int key2S = digitalRead(key2);
    int key3S = digitalRead(key3);
    int key4S = digitalRead(key4);
    
    if (!key1S){
      delete_id++;
      delay(150);
    }
    if (!key2S){
      delete_id--;
      delay(150);
    }
    if (!key3S){
      if(delete_id == 1){
        lcd.clear();
        lcd.print("Admin cannot be");
        lcd.setCursor(0,1);
        lcd.print("deleted");
        delay(1500);
      }
      else{
        chsID = false;
        lcd.clear();
        lcd.print("Deleting ID");
        lcd.setCursor(13,0);
        lcd.println(delete_id);
        delay(600);
    
        deleteFingerprint(delete_id);
        delete_id = 1;
      }
    }
    if (!key4S){
      id += 10;
      delay(150);
    }
    if(delete_id < 1){
      delete_id = 1;
    }
    if(id > 127){
      id = 127;
    }
    
    lcd.clear();  
    lcd.print("Choose ID #");
    lcd.setCursor(11,0);
    lcd.print(delete_id);
    delay(300);
  }
  
}

void finished(){
  
  lcd.clear();
  lcd.print("Finished");
  delay(500);
  
}

void getPassword(){                                        //This function will read the password from EEPROM
                                                           //So if the power supply has been cut, system will remember
  admin_password = "";                                     //the new password
  
  for(byte b=0 ; b < 5 ; b++){
    char f = EEPROM.read(b);
    admin_password = admin_password + (f);
  }

}

void resetAll(){                       //All variables going to be default

  lcd.clear();
  lcd.print("Going defaults");

  admin_password = "14231";                  
  getPass = ""; 
  newPass = "";

  for(int i=0 ; i < admin_password.length() ; i++){             //This is for password
    EEPROM.update( i,admin_password[i] );
  }
  EEPROM.update(5,0);                                           //This will work for opening systemSetup
                      
  menu = 1;
  
  logged = false;
  chsID = false;
  setupS = false;
  firstFinger = false;
  
  delete_id = 1;
  id = 1;
  delay(700);

  lcd.clear();
  lcd.print("Clearing...");
  delay(100);

  finger.emptyDatabase();
  delay(700);

  lcd.clear();
  lcd.print("Cleared");
  delay(1000);
  finished();

  lcd.clear();
  lcd.print("GOOD BYE!");
  delay(750);

  loop();
}
