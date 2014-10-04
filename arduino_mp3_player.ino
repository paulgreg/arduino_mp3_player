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
#define FILES 25

// Globals
boolean playing = false;
uint8_t offset = 0;
uint8_t selection = 0;
char* fileList[FILES];
boolean directoryList[FILES];
uint8_t fileNumber = 0;

struct StateStruct {
  uint8_t volume;
  char* path;
} state = {
  25,
  "/"
};

void setup() {
  Serial.begin(9600);

  pinMode(A0, INPUT); // 5 Way button

  display.begin(SSD1306_SWITCHCAPVCC);
  display.display();
  delay(500);
  display.clearDisplay();

  if (!musicPlayer.begin() || !SD.begin(VS1053_CARDCS)) {
    show("VS1053 or SDCARD not found");
    while (1); // don't do anything more
  }
  if (!musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT)) {
    show("DREQ pin not irq");
  }

  musicPlayer.setVolume(state.volume, state.volume); // Set volume for left, right channels. lower numbers == louder volume!

  fillFileList(state.path);
}

void loop() {
  // Check inputs
  byte action = readButton(analogRead(A0));

  if (!playing) {

    if (action == UP) {
      if (selection > 0) {
        selection--;
        if (selection < offset) {
          offset--;
        }
      }
    } else if (action == DOWN) {
      if (selection < fileNumber - 2) {
        selection++;
        if (selection >= offset + ROWS) {
          offset = selection - ROWS + 1;
        }
      }
    } else if (action == LEFT) {
      // Go back
    } else if (action == RIGHT || action == PRESS) {

      if (directoryList[selection]) { // Directory
        //fillFileList(fileList[selection]);
        //offset = selection = 0;
      } else { // File
        musicPlayer.startPlayingFile(fileList[selection]);
        playing = true;
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

  Serial.println(path);
  File dir = SD.open(path);

  for (uint8_t i = 0; i < FILES; i++)  {
    Serial.print("> ");
    Serial.print(i);
    Serial.print("> ");
    File entry = dir.openNextFile();
    if (!entry) break;
    fileList[i] = strdup(entry.name());
    directoryList[i] = entry.isDirectory();
    Serial.println(fileList[i]);

    fileNumber = i;
    entry.close();
  }
}

