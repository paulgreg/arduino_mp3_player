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


#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


/*
#define OLED_DC     7
#define OLED_CS     11
#define OLED_RESET  13
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);
*/

#define UP 1
#define LEFT 2
#define DOWN 4
#define RIGHT 8
#define PRESS 16

#define PAUSE_AFTER_ACTION 250

#define SERIAL true
#define OLED true

uint8_t volume = 20;

void setup() {

#ifdef SERIAL
  Serial.begin(9600);
#endif

#ifdef OLED
  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(500);
  display.clearDisplay();
#endif

  if (!musicPlayer.begin()) {
    show("VS1053 not found");
    while (1); // don't do anything more
  }

  if (!SD.begin(VS1053_CARDCS)) {
    show("SD card not found");
    while (1); // don't do anything more
  }
  show("SD card found");

  // list files
  printDirectory(SD.open("/"), 0);

  musicPlayer.setVolume(volume, volume); // Set volume for left, right channels. lower numbers == louder volume!

  if (! musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    show("DREQ pin not irq");
  }

  musicPlayer.GPIO_pinMode(UP, INPUT);
  musicPlayer.GPIO_pinMode(DOWN, INPUT);
  musicPlayer.GPIO_pinMode(LEFT, INPUT);
  musicPlayer.GPIO_pinMode(RIGHT, INPUT);
  musicPlayer.GPIO_pinMode(PRESS, INPUT);

  // Launch music at first
  musicPlayer.startPlayingFile("track002.mp3");
  show("Playing track002");
}

boolean last_action;

void loop() {

    // Check inputs
    uint16_t action = musicPlayer.GPIO_digitalRead();

    if (action & UP) {
      showAtXY("UP   ", 10, 1);

      if (volume > 0) {
        volume--;
      }
      musicPlayer.setVolume(volume,volume);

      afterAction();

    }
    if (action & DOWN) {
      showAtXY("DOWN ", 10, 1);

      if (volume < 20) {
        volume++;
      }
      musicPlayer.setVolume(volume,volume);

      afterAction();
    }
    if (action & LEFT) {
      showAtXY("LEFT ", 10, 1);

      musicPlayer.stopPlaying();
      delay(50);
      musicPlayer.startPlayingFile("track002.mp3");
      show("Playing track002");
      hidePause();

      afterAction();
    }
    if (action & RIGHT) {
      showAtXY("RIGHT", 10, 1);

      musicPlayer.stopPlaying();
      delay(50);
      musicPlayer.startPlayingFile("track001.mp3");
      show("Playing track001");
      hidePause();

      afterAction();
    }
    if (action & PRESS) {
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
    }
    if  (action == 0) {
      if (last_action == true) {
        showAtXY("     ", 10, 1);
        last_action = false;
      }
    }
  
  displayElapsedTime();
  delay(100);
}

void afterAction() {
  last_action = true;
  delay(PAUSE_AFTER_ACTION);
}













