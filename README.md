arduino-mp3-player
==================

A home made mp3/ogg/flac player based on an Ardnuino Micro, a VS1053 DSP and a 128x32 SPI OLED graphic display.


![Arduino micro connected to DSP with an OLED display](https://pbs.twimg.com/media/ByUHCnwCcAIrk0C.jpg:large)


## Parts ##

- [Arduino Micro](http://arduino.cc/en/Main/ArduinoBoardMicro)
- [VS1053 Codec + MicroSD Breakout](https://www.adafruit.com/products/1381)
- [128x32 SPI OLED graphic display](https://www.adafruit.com/products/661)
- [Thru-hole 5-way Navigation switch](https://www.adafruit.com/products/504)
- a micro SD card to store music


## Pin setup ##

To connect the DSP, use the [Frank Cohen’s wiring diagram](http://votsh.files.wordpress.com/2014/02/vs1053-arduino-micro-connections.pdf). 
Beware, there’s a mistake on the DREQ connection from the DSP that should be connected to the pin 3 of the Arduino (and not the RESET).

## Resources ##

- [Adafruit VS1053 library](https://github.com/adafruit/Adafruit_VS1053_Library),
- [VS1053b Datasheet](https://www.adafruit.com/datasheets/vs1053.pdf),
- [SSD1306 OLED driver library](https://github.com/adafruit/Adafruit_SSD1306)
- [OLED Datasheet](https://www.adafruit.com/datasheets/UG-2832HSWEG04.pdf),
- More resources in that [RSS feed](https://rsstodolist.appspot.com/?name=mp3player&l=100).
