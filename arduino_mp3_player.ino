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
#define NO_ACTION 5

#define PAUSE_AFTER_ACTION 250

#define LCD true

void setup() {
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  lcd.begin(16, 2);

#ifdef SERIAL
  Serial.begin(9600);
#endif

  if (!musicPlayer.begin()) {
    show("VS1053 not found");
    while (1); // don't do anything more
  }

  if (!SD.begin(CARDCS)) {
    show("SD card not found");
    while (1); // don't do anything more
  }
  show("SD card found");

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

  // Launch music at first
  musicPlayer.startPlayingFile("track001.mp3");
  show("Playing track001");
}

uint8_t action;
boolean last_action;

void loop() {

  action = NO_ACTION;
  // Check inputs
  for (action = 0; action < 5; action++) { 
    delay(50); // Wait a little bit between digital pin read
    if (musicPlayer.GPIO_digitalRead(action) == 1) {    
      break;
    }
  }



  switch(action) {
  case UP:
    showAtXY("UP   ", 10, 1);
    afterAction();
    break;

  case DOWN:
    showAtXY("DOWN ", 10, 1);
    afterAction();
    break;

  case LEFT:
    showAtXY("LEFT ", 10, 1);
    afterAction();
    break;

  case RIGHT:
    showAtXY("RIGHT", 10, 1);
    afterAction();
    break;

  case PRESS:
    showAtXY("PRESS", 10, 1);
    if (!musicPlayer.paused()) {
      musicPlayer.pausePlaying(true);
      displayPause();
      } 
    else {
      musicPlayer.pausePlaying(false);
      hidePause();
      }

      afterAction();
    break;

  case NO_ACTION:
    if (last_action == true) {
      showAtXY("     ", 10, 1);
      last_action = false;
    }
    break;
  }

  displayElapsedTime();
}

void afterAction() {
  last_action = true;
  delay(PAUSE_AFTER_ACTION);
}





