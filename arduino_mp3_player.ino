#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define VS1053_RESET 13 // VS1053 reset pin (output)
#define VS1053_CS 7 // 10 // VS1053 chip select pin (output)
#define VS1053_DCS 8 // VS1053 Data/command select pin (output)
#define VS1053_DREQ 3 // VS1053 Data request pin (into Arduino)
#define VS1053_CARDCS 6 // Card chip select pin

Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, VS1053_CARDCS);

// Software SPI
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

/*
// Hardware SPI
#define OLED_DC     7
#define OLED_CS     11
#define OLED_RESET  13
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
*/

#define NOACTION 0
#define UP       1
#define DOWN     2
#define LEFT     3
#define RIGHT    4
#define PRESS    5

#define PAUSE_AFTER_ACTION 250

#define OLED true

// Globals
uint8_t volume = 20;
char* song;

void setup() {

  //Serial.begin(9600);

  // 5 Way button
  pinMode(A0, INPUT);

#ifdef OLED
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(500);
  display.clearDisplay();
#endif

  if (!musicPlayer.begin() || !SD.begin(VS1053_CARDCS)) {
    show("VS1053 or SDCARD not found");
    while (1); // don't do anything more
  }
  if (!musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    show("DREQ pin not irq");
  }

  musicPlayer.setVolume(volume, volume); // Set volume for left, right channels. lower numbers == louder volume!

  // list files
  //printDirectory(SD.open("/"), 0);

  // Launch music at first
  song = "track002.mp3";
  musicPlayer.startPlayingFile(song);
}


void loop() {

  // Check inputs
  uint8_t action = readButton(analogRead(A0));

  if (action == UP) {
    if (volume < 20) {
      volume++;
    }
    musicPlayer.setVolume(volume, volume);

    afterAction();
    
  } else if (action == DOWN) {
    if (volume > 0) {
      volume--;
    }
    musicPlayer.setVolume(volume, volume);
    afterAction();
    
  } else if (action == LEFT) {
    musicPlayer.stopPlaying();
    delay(50);
    song = "track002.mp3";
    musicPlayer.startPlayingFile(song);

    afterAction();
    
  } else if (action == RIGHT) {
    musicPlayer.stopPlaying();
    delay(50);
    song = "track001.mp3";
    musicPlayer.startPlayingFile(song);

    afterAction();
    
  } else if (action == PRESS) {
    if (!musicPlayer.paused()) {
      musicPlayer.pausePlaying(true);
    }
    else {
      musicPlayer.pausePlaying(false);
    }

    afterAction();
  }

  updateDisplay();
  delay(50);
}

void afterAction() {
  delay(PAUSE_AFTER_ACTION);
}

uint8_t readButton(uint16_t action) {
  Serial.println(action);

  if (action > 940 && action < 960) {
    return UP;
  } else if (action > 750 && action < 780) {
    return DOWN;
  } else if (action > 910 && action < 940) {
    return LEFT;
  } else if (action > 1000 && action < 1040) {
    return RIGHT;
  } else if (action > 820 && action < 860) {
    return PRESS;
  } else {
    return NOACTION;
  }
}














