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

// Software SPI - screen shows snow if sharing hardware SPI with SDCARD/DSP
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


#define NOACTION 0
#define UP       1
#define DOWN     2
#define LEFT     3
#define RIGHT    4
#define PRESS    5

#define PAUSE_AFTER_ACTION 250

#define VOL_MAX 49
#define ROWS 4
#define MAX_FILES 25
#define MAX_DEPTH 25

// Globals
boolean playing = false;
uint8_t offset = 0;
uint8_t selection = 0;
char* fileList[MAX_FILES];
boolean directoryList[MAX_FILES];
uint8_t fileNumber = 0;

struct StateStruct {
  uint8_t volume;
  char path[MAX_DEPTH];
} state = {
  30,
  "/"
};

void setup() {
  //Serial.begin(9600);

  pinMode(A0, INPUT); // 5 Way button

  display.begin(SSD1306_SWITCHCAPVCC);

  if (!musicPlayer.begin() || !SD.begin(VS1053_CARDCS)) {
    show("VS1053 or SDCARD not found");
    while (1); // don't do anything more
  }
  if (!musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    show("DREQ pin not irq");
    while (1); // don't do anything more
  }

  musicPlayer.setVolume(state.volume, state.volume); // Set volume for left, right channels. lower numbers == louder volume!

  fillFileList(state.path);
}

void loop() {
  // Check inputs
  uint8_t action = readButton(analogRead(A0));

  if (!playing) {

    if (action == UP) {
      if (selection > 0) {
        selection--;
        if (selection < offset) {
          offset--;
        }
      }
    } else if (action == DOWN) {
      if (selection < fileNumber) {
        selection++;
        if (selection >= offset + ROWS) {
          offset = selection - ROWS + 1;
        }
      }
    } else if (action == LEFT) {
      // Go back
      uint8_t l = strlen(state.path) - 2;
      while (l > 1 && state.path[l] != '/')  {
        l--;
      }
      state.path[l] = '\0';

      fillFileList(state.path);
      offset = selection = 0;

    } else if (action == RIGHT) {

      if (directoryList[selection]) { // Directory

        strcat(state.path, fileList[selection]);
        strcat(state.path, "/");

        fillFileList(state.path);
        offset = selection = 0;
      }
      delay(PAUSE_AFTER_ACTION);

    } else if (action == PRESS) {

      if (!directoryList[selection]) { // File
        char song[MAX_DEPTH] = "\0";
        strcat(song, state.path);
        strcat(song, fileList[selection]);
        musicPlayer.startPlayingFile(song);
        playing = true;
        free(song);
      }

      delay(PAUSE_AFTER_ACTION);
    }

  } else {

    if (action == UP) {
      if (state.volume > 0) {
        state.volume--;
      }
      musicPlayer.setVolume(state.volume, state.volume);

    } else if (action == DOWN) {
      if (state.volume < VOL_MAX) {
        state.volume++;
      }
      musicPlayer.setVolume(state.volume, state.volume);

    } else if (action == LEFT || action == RIGHT) {

      musicPlayer.stopPlaying();
      playing = false;

      delay(PAUSE_AFTER_ACTION);

    } else if (action == PRESS) {
      if (!musicPlayer.paused()) {
        musicPlayer.pausePlaying(true);
      } else {
        musicPlayer.pausePlaying(false);
      }

      delay(PAUSE_AFTER_ACTION);
    }
  }

  updateDisplay();
  delay(50);
}


void fillFileList(char* path) {

  for (uint8_t i = 0; i < fileNumber; i++)  {
    free(fileList[i]);
  }

  File dir = SD.open(path);

  for (uint8_t i = 0; i < MAX_FILES; i++)  {
    File entry = dir.openNextFile();
    if (!entry) break;
    fileList[i] = strdup(entry.name());
    directoryList[i] = entry.isDirectory();
    fileNumber = i;
    entry.close();
  }
}

