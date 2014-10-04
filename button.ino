
byte readButton(uint16_t action) {
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
