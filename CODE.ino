#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <MFRC522.h> // for the RFID
#include <SPI.h> // for the RFID and SD card module
#include <SD.h> // for the SD card
#include <RTClib.h> // for the RTC
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SS_PIN 10
#define RST_PIN 9
#define CS_SD 4 
 
File myFile;
MFRC522 mfrc522(SS_PIN, RST_PIN); 
RTC_DS1307 rtc;

int flagDataIn = 0;
String data = "";
String content = "";
String NAMA = "";
int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
String STATUS = "";
void setup() {
  Serial.begin(9600);
  lcd.begin();
  lcd.backlight();
  while(!Serial);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.print("Initializing SD card...");
  lcd.print("Initializing ");
  lcd.setCursor(0, 1);
  lcd.print("SD card...");
  delay(3000);
  lcd.clear();
  if(!SD.begin(CS_SD)) {
    Serial.println("initialization failed!");
    lcd.print("Initializing ");
    lcd.setCursor(0, 1);
    lcd.print("failed!");
    return;
  }
  Serial.println("initialization done.");
  lcd.print("Initialization ");
  lcd.setCursor(0, 1);
  lcd.print("Done...");
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("  PLEASE SCAN   ");
  lcd.setCursor(0, 1);
  lcd.print("   CARD HERE    ");
  if(!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    lcd.clear();
    lcd.print("Couldn't find RTC");
    while(1);
  }
  else {
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("time done");
  }
  if(!rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    lcd.clear();
    lcd.print("RTC Not Running!");
  }
}
 
void loop() {
  readRFID();
  if (flagDataIn == 1) {
    flagDataIn = 0;
    if(data == "83-8A-CE-9A" && flag1 == 0){
      flag1 = 1;
      data = "";
      NAMA = "IMRAN";
      STATUS = "IN";
      lcd.setCursor(0, 0);
      lcd.print("    WELCOME    ");
      lcd.setCursor(0, 1);
      lcd.print("NAMA:" + String(NAMA) + " S:" + String(STATUS) + "");
      delay(2000);
      Serial.println("NAMA:" + String(NAMA) + " STATUS :" + String(STATUS) + "");
      logCard();
    }
    else if(data == "83-8A-CE-9A" && flag1 == 1){
      flag1 = 0;
      data = "";
      NAMA = "IMRAN";
      STATUS = "OUT";
      lcd.setCursor(0, 0);
      lcd.print("    GOODBYE    ");
      lcd.setCursor(0, 1);
      lcd.print("NAMA:" + String(NAMA) + " S:" + String(STATUS) + "");
      delay(2000);
      Serial.println("NAMA:" + String(NAMA) + " STATUS :" + String(STATUS) + "");
      logCard();
    }
    else if(data == "2-15-49-1C" && flag2 == 0){
      flag2 = 1;
      data = "";
      NAMA = "KAMAL";
      STATUS = "IN";
      lcd.setCursor(0, 0);
      lcd.print("    WELCOME    ");
      lcd.setCursor(0, 1);
      lcd.print("NAMA:" + String(NAMA) + " S:" + String(STATUS) + "");
      delay(2000);
      Serial.println("NAMA:" + String(NAMA) + " STATUS :" + String(STATUS) + "");
      logCard();
    }
    else if(data == "2-15-49-1C" && flag2 == 1){
      flag2 = 0;
      data = "";
      NAMA = "KAMAL";
      STATUS = "OUT";
      lcd.setCursor(0, 0);
      lcd.print("    GOODBYE    ");
      lcd.setCursor(0, 1);
      lcd.print("NAMA:" + String(NAMA) + " S:" + String(STATUS) + "");
      delay(2000);
      Serial.println("NAMA:" + String(NAMA) + " STATUS :" + String(STATUS) + "");
      logCard();
    }
    else if(data == "9A-A2-DF-B" && flag3 == 0){
      flag3 = 1;
      data = "";
      NAMA = "HAKEM";
      STATUS = "IN";
      lcd.setCursor(0, 0);
      lcd.print("    WELCOME    ");
      lcd.setCursor(0, 1);
      lcd.print("NAMA:" + String(NAMA) + " S:" + String(STATUS) + "");
      delay(2000);
      Serial.println("NAMA:" + String(NAMA) + " STATUS :" + String(STATUS) + "");
      logCard();
    }
    else if(data == "9A-A2-DF-B" && flag3 == 1){
      flag3 = 0;
      data = "";
      NAMA = "HAKEM";
      STATUS = "OUT";
      lcd.setCursor(0, 0);
      lcd.print("    GOODBYE    ");
      lcd.setCursor(0, 1);
      lcd.print("NAMA:" + String(NAMA) + " S:" + String(STATUS) + "");
      delay(2000);
      Serial.println("NAMA:" + String(NAMA) + " STATUS :" + String(STATUS) + "");
      logCard();
    }
  }
  delay(10);
}
 
void logCard() {
  digitalWrite(CS_SD,LOW);
  
  // Open file
  myFile=SD.open("DATA.txt", FILE_WRITE);
 
  // If the file opened ok, write to it
  if (myFile) {
    Serial.println("File opened ok");
    lcd.clear();
    lcd.print("File opened ok");
    delay(2000);
    myFile.print(NAMA);
    myFile.print(", ");   
    myFile.print(STATUS);
    myFile.print(", "); 
    NAMA = "";
    STATUS = "";
    // Save time on SD card
    DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.println(now.minute(), DEC);
    
    // Print time on Serial monitor
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.println(now.minute(), DEC);
    Serial.println("sucessfully written on SD card");
 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.setCursor(5, 0);
    lcd.print("        ");
    lcd.setCursor(0, 1);
    lcd.print("Written on SD...");
    delay(2000); 
    myFile.close();
  }
  else {
    
    Serial.println("error opening data.txt");  
    lcd.clear();
    lcd.print("error opening data.txt");
  }
  digitalWrite(CS_SD,HIGH);
  lcd.setCursor(0, 0);
  lcd.print("  PLEASE SCAN   ");
  lcd.setCursor(0, 1);
  lcd.print("   CARD HERE    ");
}
 void readRFID() {
  if (flagDataIn == 0){
    //Look for new cards
    if ( !mfrc522.PICC_IsNewCardPresent() ) {
      return;
    }
    //Select one of the cards
    if ( !mfrc522.PICC_ReadCardSerial() ) {
      return;
    }

    byte letter;
    for ( byte i = 0; i < mfrc522.uid.size; i++ ) {
      content.concat(String(mfrc522.uid.uidByte[i], HEX));
      if ( i < mfrc522.uid.size - 1 ) content += "-";
    }
    content.toUpperCase();
    Serial.println();
    Serial.println("UID tag :'" + content + "'");
    data = content;
    content = "";
    flagDataIn = 1;
  }
  }
