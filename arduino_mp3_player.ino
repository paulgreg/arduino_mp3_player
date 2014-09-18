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

#define INTERVAL 100
#define SERIAL true
#define LCD true

void show(char * m) {
  showAt(m, 0);
}

void showAt(char * m, int row) {
  if (LCD) {
    lcd.setCursor(0, row);
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

  Serial.begin(9600);

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
}

boolean playing;
unsigned long time;
char buf[32];

void loop() {
  playing = musicPlayer.playingMusic;

  if (!playing) {
    musicPlayer.startPlayingFile("track001.mp3");
    show("Playing track001");
    time = 0;
  } 
  else {
    delay(INTERVAL);
    time += 1;

    if (time % 10) {
      showAt(itoa(time/10, buf, 10), 1);
    }
  }
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




