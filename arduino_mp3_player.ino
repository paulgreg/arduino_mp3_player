#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
#include <LiquidCrystal.h>

#define BREAKOUT_RESET 13 // VS1053 reset pin (output)
#define BREAKOUT_CS 10 // VS1053 chip select pin (output)
#define BREAKOUT_DCS 8 // VS1053 Data/command select pin (output)
#define DREQ 3 // VS1053 Data request pin (into Arduino)
#define CARDCS 6 // Card chip select pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(BREAKOUT_RESET, BREAKOUT_CS, BREAKOUT_DCS, DREQ, CARDCS);
LiquidCrystal lcd(A0, A1, A5, A4, A3, A2);

#define UP 0
#define LEFT 1
#define DOWN 2
#define RIGHT 3
#define PRESS 4

#define INTERVAL 100
#define SERIAL true
#define LCD true

void show(char * m) {
  showAtXY(m, 0, 0);
}

void showatY(char * m, uint8_t y) {
  showAtXY(m, 0, y);
}

void showAtXY(char * m, uint8_t x, uint8_t y) {
  if (LCD) {
    lcd.setCursor(x, y);
    lcd.print(m);
  }
  if (SERIAL) {
    Serial.println(m);
  }
}


void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  lcd.begin(16, 2);

  if (LCD) {
    Serial.begin(9600);
  }

  if (!musicPlayer.begin()) {
    show("VS1053 not found");
    while (1);
  }

  show("VS1053 found");

  if (!SD.begin(CARDCS)) {
    show("SD not found");
    while (1);  // don't do anything more
  }
  show("SD found");

  // list files
  printDirectory(SD.open("/"), 0);

  musicPlayer.setVolume(40,40); // Set volume for left, right channels. lower numbers == louder volume!  

  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    show("DREQ pin not irq");
  }

  musicPlayer.GPIO_pinMode(0, INPUT);
  musicPlayer.GPIO_pinMode(1, INPUT);
  musicPlayer.GPIO_pinMode(2, INPUT);
  musicPlayer.GPIO_pinMode(3, INPUT);
  musicPlayer.GPIO_pinMode(4, INPUT);


  musicPlayer.startPlayingFile("track001.mp3");
  show("Playing track001");
}


void loop() {

  showAtXY("     ", 10, 1);

  for (uint8_t i=0; i<5; i++) { 
    delay(10);
    if (musicPlayer.GPIO_digitalRead(i) == 1) {    
      switch(i) {
      case UP:
        showAtXY("UP   ", 10, 1);
        break;
      case DOWN:
        showAtXY("DOWN ", 10, 1);
        break;
      case LEFT:
        showAtXY("LEFT ", 10, 1);
        break;
      case RIGHT:
        showAtXY("RIGHT", 10, 1);
        break;
      case PRESS:
        showAtXY("PRESS", 10, 1);
        if (!musicPlayer.paused()) {
          showAtXY("\"", 0, 1);
          musicPlayer.pausePlaying(true);
        } 
        else {
          showAtXY(" ", 0, 1);
          musicPlayer.pausePlaying(false);
        }
        break;
      }
    }
  }

  char timebuffer[8];
  formatTime(timebuffer, musicPlayer.decodeTime());
  showAtXY(timebuffer, 1, 1);

  delay(INTERVAL);
}

char* formatTime(char* buffer, uint16_t time) {
  int hours = time / 3600;
  int minutes = (time % 3600) / 60;
  int seconds = (time% 3600) % 60; 

  sprintf(buffer, "%02i:%02i:%02i", hours, minutes, seconds);
}

/// File listing helper
void printDirectory(File dir, int numTabs) {
  while(true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      //Serial.println("**nomorefiles**");
      break;
    }
    for (uint8_t i=0; i<numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs+1);
    } 
    else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}










