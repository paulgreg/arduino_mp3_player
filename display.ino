
void show(char * m) {
  showAtXY(m, 0, 0);
}

void showatY(char * m, uint8_t y) {
  showAtXY(m, 0, y);
}

void showAtXY(char * m, uint8_t x, uint8_t y) {
#ifdef OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.print(m);
  display.display();
#endif
#ifdef SERIAL
  Serial.println(m);
#endif
}

char* formatTime(char* buffer, uint16_t time) {
  uint8_t hours = (time / 3600);
  uint8_t minutes = ((time % 3600) / 60);
  uint8_t seconds = ((time % 3600) % 60);

  snprintf(buffer, 10, "%02i:%02i:%02i", hours, minutes, seconds);
}

void displayElapsedTime() {
  char timebuffer[10];
  formatTime(timebuffer, musicPlayer.decodeTime());
  showAtXY(timebuffer, 1, 1);
}


void displayPause() {
  showAtXY("\"", 0, 1);
}

void hidePause() {
  showAtXY(" ", 0, 1);
}


