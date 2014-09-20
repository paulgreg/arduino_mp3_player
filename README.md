arduino-mp3-player
==================

A home made mp3/ogg/flac player based on an Ardnuino Micro and a VS1053 DSP.


![With the LCD display](https://pbs.twimg.com/media/Bx1wlU1CIAIJqPz.jpg:small)
![Arduino micro connected to DSP](https://pbs.twimg.com/media/BxwyLgXIAAARbxZ.jpg:small)


## Parts ##

- Arduino Micro
- [VS1053 Codec + MicroSD Breakout](https://www.adafruit.com/products/1381)
- LCD display (from Arduino starter kit)
- [Thru-hole 5-way Navigation switch](https://www.adafruit.com/products/504)
- a micro SD card to store music


## Pin setup ##

To connect the DSP, use the [Frank Cohen’s wiring diagram](http://votsh.files.wordpress.com/2014/02/vs1053-arduino-micro-connections.pdf). 
Beware, there’s a mistake on the DREQ connection from the DSP that should be connected to the pin 3 of the Arduino (and not the RESET).

To connect the LCD, use Arduino micro analog pins, from A0 to A5 : 
- A0 to LCD 4, 
- A1 to LCD 6, 
- A2 to LCD 14,
- A3 to LCD 13,
- A4 to LCD 12,
- A5 to LCD 11.

## Resources ##

- [Adafruit VS1053 library](https://github.com/adafruit/Adafruit_VS1053_Library),
- [VS1053b Datasheet](https://www.adafruit.com/datasheets/vs1053.pdf),
- More resources in that [RSS feed](https://rsstodolist.appspot.com/?name=mp3player&l=100).
