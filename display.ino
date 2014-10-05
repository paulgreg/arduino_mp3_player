void show(char * m) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(m);
  display.display();
}

void updateDisplay() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);

  if (!playing) {

    for (uint8_t i = offset; i < (offset + ROWS) && i <= fileNumber; i++) {
      display.print((selection == i) ? "> " : "  ");
      display.print(fileList[i]);
      display.println(directoryList[i] ? "/" : "");
    }

  } else {
    display.print((!musicPlayer.paused()) ? " > " : " \" ");

    display.setCursor(15, 0);
    display.print(fileList[selection]);

    char timebuffer[9];
    formatTime(timebuffer, musicPlayer.decodeTime());
    display.setCursor(0, 10);
    display.print(timebuffer);

    char volbuffer[3];
    snprintf(volbuffer, 3, "%02i", VOL_MAX  + 1 - state.volume);
    display.setCursor(110, 10);
    display.print(volbuffer);
  }
  display.display();
}

char* formatTime(char* buffer, uint16_t time) {
  uint8_t hours = (time / 3600);
  uint8_t minutes = ((time % 3600) / 60);
  uint8_t seconds = ((time % 3600) % 60);

  snprintf(buffer, 9, "%02i:%02i:%02i", hours, minutes, seconds);
}
