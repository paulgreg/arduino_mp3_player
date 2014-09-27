
void show(char * m) {
#ifdef OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(m);
  display.display();
#endif
}

char* formatTime(char* buffer, uint16_t time) {
  uint8_t hours = (time / 3600);
  uint8_t minutes = ((time % 3600) / 60);
  uint8_t seconds = ((time % 3600) % 60);

  snprintf(buffer, 9, "%02i:%02i:%02i", hours, minutes, seconds);
}

void updateDisplay() {
  char timebuffer[9];
  formatTime(timebuffer, musicPlayer.decodeTime());
  show(timebuffer);
}


void displayPause() {
  show("\"");
}

void hidePause() {
  show(" ");
}


