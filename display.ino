
void show(char * m) {
  showAtXY(m, 0, 0);
}

void showatY(char * m, uint8_t y) {
  showAtXY(m, 0, y);
}

void showAtXY(char * m, uint8_t x, uint8_t y) {
#ifdef LCD
  lcd.setCursor(x, y);
  lcd.print(m);
#endif
#ifdef SERIAL
  Serial.println(m);
#endif
}

char* formatTime(char* buffer, uint16_t time) {
  int hours = time / 3600;
  int minutes = (time % 3600) / 60;
  int seconds = (time% 3600) % 60; 

  sprintf(buffer, "%02i:%02i:%02i", hours, minutes, seconds);
}

